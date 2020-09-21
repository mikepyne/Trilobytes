#ifndef QUADTREE_H
#define QUADTREE_H

#include "Shape.h"
#include "EntityContainerInterface.h"
#include "Entity.h"

#include <memory>
#include <vector>
#include <functional>

class QPainter;

class QuadTree {
public:
    QuadTree(const Rect& startingArea);

    void Tick();
    void Draw(QPainter& paint);

    void AddEntity(const std::shared_ptr<Entity>& entity);
    uint64_t EntityCount();
    void SetEntityCapacity(uint64_t target, uint64_t leeway);
    void ForEach(const std::function<void(const Entity&)>& action) const;

    /**
     * Applies the action to every entity that is contained within the specified
     * collision shape.
     */
    template <typename Shape>
    void ForEachCollidingWith(const Shape& collide, const std::function<void(Entity&)>& action) const
    {
        root_->ForEachCollidingWith(collide, action);
    }

private:
    class Quad : private EntityContainerInterface {
    public:
        Quad(const Rect& area, QuadTree& owner, Quad* parent);
        Quad(const Rect& area, QuadTree& owner, std::vector<std::shared_ptr<Quad>>&& children);

        static std::vector<std::shared_ptr<Quad>> CreateChildren(Rect parentRect, QuadTree& baseTree, Quad* parent);
        virtual void AddEntity(const std::shared_ptr<Entity>& entity) override final;

        void TickRecursive();
        void ResolveRecursive();
        void DrawRecursive(QPainter& paint);
        void RehomeRecursive(const std::shared_ptr<Entity>& entity, bool delayed);
        void ForEachRecursive(const std::function<void(const Entity&)>& action) const;
        uint64_t RecursiveEntityCount() const;

        const Rect& GetRect();

        virtual void ForEachCollidingWith(const Point& collide, const std::function<void (Entity&)>& action) const override { ForEachCollidingWith<Point>(collide, action); }
        virtual void ForEachCollidingWith(const Line& collide, const std::function<void (Entity&)>& action) const override { ForEachCollidingWith<Line>(collide, action); }
        virtual void ForEachCollidingWith(const Rect& collide, const std::function<void (Entity&)>& action) const override final { ForEachCollidingWith<Rect>(collide, action); }
        virtual void ForEachCollidingWith(const Circle& collide, const std::function<void (Entity&)>& action) const override final { ForEachCollidingWith<Circle>(collide, action); }

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
        void ForEachInRecursive(const Shape& collide, const std::function<void(Entity&)>& action) const
        {
            if (!Collides(rect_, collide)) {
                assert(Collides(rect_, collide));
            }
            if (children_.empty()) {
                for (auto& entity : entities_) {
                    if (Collides(collide, Circle{ entity->GetX(), entity->GetY(), entity->GetRadius() })) {
                        action(*entity);
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
        void ForEachCollidingWith(const Shape& collide, const std::function<void(Entity&)>& action) const
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
