#ifndef QUADTREE_H
#define QUADTREE_H

#include "Entity.h"
#include "Shape.h"

#include <QPainter>

#include <vector>
#include <memory>
#include <optional>
#include <algorithm>
#include <functional>
#include <assert.h>

class QuadTree {
public:
    QuadTree(const Rect& startingArea)
        : root_(std::make_shared<Quad>(startingArea))
    {
    }

    void Tick()
    {
        root_->TickRecursive();
        root_->ResolveRecursive();
        root_->Rebalance(20, 5);
    }

    void Draw(QPainter& paint)
    {
        root_->DrawRecursive(paint);
    }

    void AddEntity(const std::shared_ptr<Entity>& entity)
    {
        root_->RehomeRecursive(entity, false);
    }

    uint64_t EntityCount()
    {
        return root_->RecursiveEntityCount();
    }

private:
    class Quad : private EntityContainer {
    public:
        Quad(const Rect& area, Quad* parent = nullptr)
            : parent_(parent)
            , rect_(area)
        {
        }

        virtual void AddEntity(const std::shared_ptr<Entity>& entity) override final
        {
            RehomeRecursive(entity, true);
        }

        void TickRecursive()
        {
            if (!children_.empty()) {
                assert(entities_.empty());
                for (auto& child : children_) {
                    child->TickRecursive();
                }
            } else {
                // Tick entities and register moved entities with their new home
                for (auto e : entities_) {
                    if (e->Tick(*this)) {
                        if (!Contains(rect_, e->GetLocation())) {
                            exitingEntities_.push_back(e);
                            RehomeRecursive(e, true);
                        }
                    }
                }
            }
        }

        void ResolveRecursive()
        {
            if (!children_.empty()) {
                // Only leaf nodes contain child entities so we won't have anything to deal with here
                assert(enteringEntities_.empty());
                assert(exitingEntities_.empty());
                assert(entities_.empty());
                for (auto& child : children_) {
                    child->ResolveRecursive();
                }
            } else {
                entities_.erase(std::remove_if(std::begin(entities_), std::end(entities_), [&](auto& e)
                {
                    // remove if has exitied or died
                    bool exited = std::find(std::begin(exitingEntities_), std::end(exitingEntities_), e) != std::end(exitingEntities_);
                    bool died = !e->Alive();
                    return exited || died;
                }), std::end(entities_));
                exitingEntities_.clear();

                // insert if alive
                std::copy_if(std::begin(enteringEntities_), std::end(enteringEntities_), std::back_inserter(entities_), [](auto& e)
                {
                    return e->Alive();
                });
                enteringEntities_.clear();
            }
        }

        void DrawRecursive(QPainter& paint)
        {
            paint.setBrush(QColor(200, 225, 255, 0));
            paint.drawRect(QRectF(QPointF(rect_.left, rect_.top), QPointF(rect_.right, rect_.bottom)));
            if (!children_.empty()) {
                for (auto& child : children_) {
                    child->DrawRecursive(paint);
                }
            } else {
                for (auto e : entities_) {
                    e->Draw(paint);
                }
            }
        }

        void RehomeRecursive(const std::shared_ptr<Entity>& entity, bool delayed)
        {
            if (Contains(rect_, entity->GetLocation())) {
                if (children_.empty()) {
                    if (delayed) {
                        enteringEntities_.push_back(entity);
                    } else {
                        entities_.push_back(entity);
                    }
                } else {
                    for (auto& child : children_) {
                        if (Contains(child->rect_, entity->GetLocation())) {
                            child->RehomeRecursive(entity, delayed);
                        }
                    }
                }
            } else if (parent_) {
                parent_->RehomeRecursive(entity, delayed);
            } else {
                // TODO need to create a new parent, and somehow communictae that root has updated
            }
        }

        \
        /**
         * Applies the action to every entity that is contained within the specified
         * collision rectangle.
         */
        virtual void ForEachIn(const Rect& collide, const std::function<void(Entity&)>& action) const override final
        {
            if (Contains(rect_, collide) || (!parent_ && Collides(rect_, collide))) {
                ForEachInRecursive(collide, action);
            } else if (parent_){
                parent_->ForEachInRecursive(collide, action);
            }
        }

        /**
         * Applies the action to every entity that is contained within the specified
         * collision circle.
         */
        virtual void ForEachIn(const Circle& collide, const std::function<void(Entity&)>& action) const override final
        {
            if (Contains(rect_, collide) || (!parent_ && Collides(rect_, collide))) {
                ForEachInRecursive(collide, action);
            } else if (parent_){
                parent_->ForEachInRecursive(collide, action);
            }
        }

        /**
         * Historesis allows for leeway in count so a single entity repeatedly
         * entering and exiting a near full quad wouldn't cause repeat rebalancing.
         */
        void Rebalance(const uint64_t targetCount, uint64_t historesis)
        {
            assert(enteringEntities_.empty());
            assert(exitingEntities_.empty());
            if (!children_.empty() && RecursiveEntityCount() < targetCount - historesis) {
                entities_ = RecursivelyCollectEntities();
                children_.clear();
            } else if (children_.empty() && RecursiveEntityCount() > targetCount + historesis) {
                double halfWidth = (rect_.right - rect_.left) / 2.0;
                double midX = rect_.left + halfWidth;
                double midY = rect_.top + halfWidth;

                children_ = {
                    std::make_shared<Quad>(Rect{ rect_.left, rect_.top, midX,        midY }, this),
                    std::make_shared<Quad>(Rect{ midX,       rect_.top, rect_.right, midY }, this),
                    std::make_shared<Quad>(Rect{ rect_.left, midY,      midX,        rect_.bottom }, this),
                    std::make_shared<Quad>(Rect{ midX,       midY,      rect_.right, rect_.bottom }, this),
                };

                Quad& topLeft = *children_.at(0);
                Quad& topRight = *children_[1];
                Quad& bottomLeft = *children_[2];
                Quad& bottomRight = *children_[3];

                for (auto entity : entities_) {
                    if (entity->GetX() < midX) {
                        if (entity->GetY() < midY) {
                            topLeft.entities_.push_back(std::move(entity));
                        } else {
                            bottomLeft.entities_.push_back(std::move(entity));
                        }
                    } else {
                        if (entity->GetY() < midY) {
                            topRight.entities_.push_back(std::move(entity));
                        } else {
                            bottomRight.entities_.push_back(std::move(entity));
                        }
                    }
                }
                entities_.clear();
            }

            for (auto& child : children_) {
                child->Rebalance(targetCount, historesis);
            }
        }

        uint64_t RecursiveEntityCount() const
        {
            uint64_t count = 0;
            if (children_.empty()) {
                count = entities_.size();
            } else {
                for (auto& child : children_) {
                    count += child->RecursiveEntityCount();
                }
            }
            return count;
        }

    private:
        Quad* parent_;
        std::vector<std::shared_ptr<Quad>> children_;

        Rect rect_;
        std::vector<std::shared_ptr<Entity>> entities_;
        std::vector<std::shared_ptr<Entity>> enteringEntities_;
        std::vector<std::shared_ptr<Entity>> exitingEntities_;

        /**
         * Applies the action to every entity that is contained within the specified
         * collision shape.
         */
        template <typename Shape>
        void ForEachInRecursive(const Shape& collide, const std::function<void(Entity&)>& action) const
        {
            assert(Collides(rect_, collide));
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

        std::vector<std::shared_ptr<Entity>> RecursivelyCollectEntities()
        {
            std::vector<std::shared_ptr<Entity>> allEntities;
            if (children_.empty()) {
                allEntities.swap(entities_);
            } else {
                for (auto& child : children_) {
                    std::vector<std::shared_ptr<Entity>> entities = child->RecursivelyCollectEntities();
                    std::move(std::begin(entities), std::end(entities), std::back_inserter(allEntities));
                }
            }
            return allEntities;
        }
    };

    std::shared_ptr<Quad> root_;
};


#endif // QUADTREE_H
