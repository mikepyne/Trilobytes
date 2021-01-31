#ifndef QUADTREE_H
#define QUADTREE_H

#include "Shape.h"
#include "EntityContainerInterface.h"
#include "Entity.h"
#include "UniverseParameters.h"

#include <memory>
#include <vector>
#include <functional>
#include <type_traits>

class QPainter;

// TODO remove need to ever specify starting area, or track which direction was last expanded into
class QuadTree {
public:
    QuadTree(const Rect& startingArea);

    // Ticks all containing entities and resolves any entities moving from one quad to another
    void Tick(const UniverseParameters& universeParameters);
    void Draw(QPainter& paint, const Rect& renderArea) const;

    const EntityContainerInterface& GetContainerInterface() const { return *root_; }

    void SetEntityTargetPerQuad(uint64_t target, uint64_t leeway);

    void AddEntity(const std::shared_ptr<Entity>& entity) { root_->RehomeRecursive(entity, false); }
    std::shared_ptr<Entity> PickEntity(const Point& location, bool remove) { return root_->PickRecursive(location, remove); }
    void ForEach(const std::function<void(const std::shared_ptr<Entity>&)>& action) const;
    uint64_t EntityCount() { return root_->RecursiveEntityCount(); }

    void Reset(const Rect& area);
    void Clear();
    template<typename... T>
    void Clear() const
    {
        static_assert(sizeof...(T) > 0, "Must have at least one type.");
        static_assert((std::is_base_of<Entity, T>::value,...), "All types must extend Entity");

        (root_->ClearRecursive<T>(),...);
        root_->Rebalance(targetCount_, leewayCount_);
    }

    /**
     * Applies the action to every entity that is contained within the specified
     * collision shape.
     *
     * FIXME entities overlapping quads may not be detected in egde cases, where
     * they do overlap the shape, but are centred in a different quad, which the
     * shape does not touch. Consider allowing entities to exist in more than
     * one quad? (nope would tick multiple times) Then perhaps consider adding
     * another list of entities to each quad, which is entities not within, but
     * colliding with the quad.
     */
    template <typename Shape>
    void ForEachCollidingWith(const Shape& collide, const std::function<void(const std::shared_ptr<Entity>&)>& action) const
    {
        root_->ForEachCollidingWith(collide, action);
    }

    template <typename Shape>
    unsigned CountEntities(const Shape& collide)
    {
        unsigned count = 0;
        ForEachCollidingWith(collide, [&](auto&)
        {
            ++count;
        });
        return count;
    }

private:
    class Quad : public EntityContainerInterface {
    public:
        Quad(const Rect& area, QuadTree& owner, Quad* parent);
        Quad(const Rect& area, QuadTree& owner, std::vector<std::shared_ptr<Quad>>&& children);
        virtual ~Quad() override;

        static std::vector<std::shared_ptr<Quad>> CreateChildren(Rect parentRect, QuadTree& baseTree, Quad* parent);
        virtual void AddEntity(const std::shared_ptr<Entity>& entity) override final;

        void TickRecursive(const UniverseParameters& universeParameters);
        void ResolveRecursive();
        void DrawRecursive(QPainter& paint, const Rect& renderArea) const;
        void RehomeRecursive(const std::shared_ptr<Entity>& entity, bool delayed);
        std::shared_ptr<Entity> PickRecursive(const Point& location, bool remove);
        template <typename T>
        void ClearRecursive()
        {
            entities_.erase(std::remove_if(std::begin(entities_), std::end(entities_), [](const std::shared_ptr<Entity>& entity)
            {
                return dynamic_cast<const T*>(entity.get());
            }), std::end(entities_));
            enteringEntities_.erase(std::remove_if(std::begin(enteringEntities_), std::end(enteringEntities_), [](const std::shared_ptr<Entity>& entity)
            {
                return dynamic_cast<const T*>(entity.get());
            }), std::end(enteringEntities_));
            exitingEntities_.erase(std::remove_if(std::begin(exitingEntities_), std::end(exitingEntities_), [](const std::shared_ptr<Entity>& entity)
            {
                return dynamic_cast<const T*>(entity.get());
            }), std::end(exitingEntities_));

            for (auto& child : children_) {
                child->ClearRecursive<T>();
            }
        }
        void ForEachRecursive(const std::function<void(const std::shared_ptr<Entity>&)>& action) const;
        uint64_t RecursiveEntityCount() const;

        const Rect& GetRect();

        virtual void ForEachCollidingWith(const Point& collide, const std::function<void (const std::shared_ptr<Entity>&)>& action) const override { ForEachCollidingWith<Point>(collide, action); }
        virtual void ForEachCollidingWith(const Line& collide, const std::function<void (const std::shared_ptr<Entity>&)>& action) const override { ForEachCollidingWith<Line>(collide, action); }
        virtual void ForEachCollidingWith(const Rect& collide, const std::function<void (const std::shared_ptr<Entity>&)>& action) const override final { ForEachCollidingWith<Rect>(collide, action); }
        virtual void ForEachCollidingWith(const Circle& collide, const std::function<void (const std::shared_ptr<Entity>&)>& action) const override final { ForEachCollidingWith<Circle>(collide, action); }

        /**
         * Historesis allows for leeway in count so a single entity repeatedly
         * entering and exiting a near full quad wouldn't cause repeat rebalancing.
         */
        void Rebalance(const uint64_t targetCount, uint64_t historesis);

    private:
        QuadTree& baseTree_;
        Quad* parent_;
        std::vector<std::shared_ptr<Quad>> children_;

        Rect rect_;
        std::vector<std::shared_ptr<Entity>> entities_;
        std::vector<std::shared_ptr<Entity>> enteringEntities_;
        std::vector<std::shared_ptr<Entity>> exitingEntities_;

        bool AreAllChildrenLeafQuads();
        std::vector<std::shared_ptr<Entity>> RecursivelyCollectEntities();

        /**
         * Applies the action to every entity that collides with the specified
         * shape.
         */
        template <typename Shape>
        void ForEachInRecursive(const Shape& collide, const std::function<void(const std::shared_ptr<Entity>&)>& action) const
        {
            if (!Collides(rect_, collide)) {
                assert(Collides(rect_, collide));
            }
            if (children_.empty()) {
                for (auto& entity : entities_) {
                    if (Collides(collide, Circle{ entity->GetTransform().x, entity->GetTransform().y, entity->GetRadius() })) {
                        action(entity);
                    }
                }
            } else {
                for (auto& child : children_) {
                    if (Collides(child->rect_, collide)) {
                        child->ForEachInRecursive(collide, action);
                    }
                }
            }
        }

        /**
         * Applies the action to every entity that is contained within the specified
         * collision shape.
         */
        template <typename Shape>
        void ForEachCollidingWith(const Shape& collide, const std::function<void(const std::shared_ptr<Entity>&)>& action) const
        {
            if (Contains(rect_, collide) || (!parent_ && Collides(rect_, collide))) {
                ForEachInRecursive(collide, action);
            } else if (parent_){
                parent_->ForEachCollidingWith(collide, action);
            }
        }
    };

    std::shared_ptr<Quad> root_;

    uint64_t targetCount_;
    uint64_t leewayCount_;
    bool requiresRebalance_;
    uint64_t rootExpandedCount_;

    void ExpandRoot();
};

#endif // QUADTREE_H
