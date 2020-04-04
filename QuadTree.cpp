#include "QuadTree.h"

#include "Entity.h"

#include <QPainter>

#include <algorithm>
#include <assert.h>

QuadTree::QuadTree(const Rect& startingArea)
    : root_(std::make_shared<Quad>(startingArea, *this, nullptr))
    , targetCount_(1)
    , leewayCount_(0)
    , requiresRebalance_(true)
    , rootExpandedCount_(0)
{
}

void QuadTree::Tick()
{
    if (requiresRebalance_) {
        root_->Rebalance(targetCount_, leewayCount_);
        requiresRebalance_ = false;
    }
    root_->TickRecursive();
    root_->ResolveRecursive();
}

void QuadTree::Draw(QPainter& paint)
{
    root_->DrawRecursive(paint);
}

void QuadTree::AddEntity(const std::shared_ptr<Entity>& entity)
{
    root_->RehomeRecursive(entity, false);
}

uint64_t QuadTree::EntityCount()
{
    return root_->RecursiveEntityCount();
}

void QuadTree::SetEntityCapacity(uint64_t target, uint64_t leeway)
{
    targetCount_ = target;
    leewayCount_ = leeway;
    root_->Rebalance(targetCount_, leewayCount_);
    requiresRebalance_ = false;
}

void QuadTree::ForEach(const std::function<void (const Entity&)>& action) const
{
    if (root_) {
        root_->ForEachRecursive(action);
    }
}

void QuadTree::ExpandRoot()
{
    bool expandOutwards = rootExpandedCount_++ % 2 == 0;
    const Rect& oldRootRect = root_->GetRect();
    double width = oldRootRect.right - oldRootRect.left;
    double height = oldRootRect.bottom - oldRootRect.top;

    Rect newRootRect = {
        oldRootRect.left - (expandOutwards ? 0.0 : width),
        oldRootRect.top - (expandOutwards ? 0.0 : height),
        oldRootRect.right + (expandOutwards ? width : 0.0),
        oldRootRect.bottom + (expandOutwards ? height : 0.0)
    };
    std::vector<std::shared_ptr<Quad>> children = Quad::CreateChildren(newRootRect, *this, nullptr);
    for (auto& child : children) {
        if (root_->GetRect() == child->GetRect()) {
            child.swap(root_);
            break;
        }
    }
    root_ = std::make_shared<Quad>(newRootRect, *this, std::move(children));
}

QuadTree::Quad::Quad(const Rect& area, QuadTree& owner, QuadTree::Quad* parent)
    : baseTree_(owner)
    , parent_(parent)
    , rect_(area)
{
}

QuadTree::Quad::Quad(const Rect& area, QuadTree& owner, std::vector<std::shared_ptr<QuadTree::Quad> >&& children)
    : Quad(area, owner, nullptr)
{
    children_.swap(children);
    for (auto& child : children_) {
        child->parent_ = this;
    }
}

std::vector<std::shared_ptr<QuadTree::Quad> > QuadTree::Quad::CreateChildren(Rect parentRect, QuadTree& baseTree, QuadTree::Quad* parent)
{
    double halfWidth = (parentRect.right - parentRect.left) / 2.0;
    double midX = parentRect.left + halfWidth;
    double midY = parentRect.top + halfWidth;

    return {
        std::make_shared<Quad>(Rect{ parentRect.left, parentRect.top, midX            , midY }             , baseTree, parent),
                std::make_shared<Quad>(Rect{ midX           , parentRect.top, parentRect.right, midY }             , baseTree, parent),
                std::make_shared<Quad>(Rect{ parentRect.left, midY          , midX            , parentRect.bottom }, baseTree, parent),
                std::make_shared<Quad>(Rect{ midX           , midY          , parentRect.right, parentRect.bottom }, baseTree, parent),
    };
}

void QuadTree::Quad::AddEntity(const std::shared_ptr<Entity>& entity)
{
    RehomeRecursive(entity, true);
}

void QuadTree::Quad::TickRecursive()
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

void QuadTree::Quad::ResolveRecursive()
{
    if (!children_.empty()) {
        // Only leaf nodes contain child entities so we won't have anything to deal with here
        assert(enteringEntities_.empty());
        assert(exitingEntities_.empty());
        assert(entities_.empty());
        for (auto& child : children_) {
            child->ResolveRecursive();
        }

        if (AreAllChildrenLeafQuads() && RecursiveEntityCount() <= baseTree_.targetCount_ - baseTree_.leewayCount_) {
            baseTree_.requiresRebalance_ = true;
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

        if (entities_.size() >= baseTree_.targetCount_ + baseTree_.leewayCount_) {
            baseTree_.requiresRebalance_ = true;
        }
    }
}

void QuadTree::Quad::DrawRecursive(QPainter& paint)
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

void QuadTree::Quad::RehomeRecursive(const std::shared_ptr<Entity>& entity, bool delayed)
{
    if (Contains(rect_, entity->GetLocation())) {
        if (children_.empty()) {
            if (delayed) {
                enteringEntities_.push_back(entity);
            } else {
                entities_.push_back(entity);
                if (entities_.size() >= baseTree_.targetCount_ + baseTree_.leewayCount_) {
                    baseTree_.requiresRebalance_ = true;
                }
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
        baseTree_.ExpandRoot();
        RehomeRecursive(entity, delayed);
    }
}

void QuadTree::Quad::ForEachRecursive(const std::function<void (const Entity&)>& action) const
{
    for (const auto& child : children_) {
        child->ForEachRecursive(action);
    }
    for (const auto& entity : entities_) {
        action(*entity);
    }
}

uint64_t QuadTree::Quad::RecursiveEntityCount() const
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

const Rect& QuadTree::Quad::GetRect()
{
    return rect_;
}

void QuadTree::Quad::ForEachCollidingWith(const Line& collide, const std::function<void (Entity&)>& action) const
{
    if (Contains(rect_, collide) || (!parent_ && Collides(rect_, collide))) {
        ForEachInRecursive(collide, action);
    } else if (parent_){
        parent_->ForEachCollidingWith(collide, action);
    }
}

void QuadTree::Quad::ForEachIn(const Rect& collide, const std::function<void (Entity&)>& action) const
{
    if (Contains(rect_, collide) || (!parent_ && Collides(rect_, collide))) {
        ForEachInRecursive(collide, action);
    } else if (parent_){
        parent_->ForEachIn(collide, action);
    }
}

void QuadTree::Quad::ForEachIn(const Circle& collide, const std::function<void (Entity&)>& action) const
{
    if (Contains(rect_, collide) || (!parent_ && Collides(rect_, collide))) {
        ForEachInRecursive(collide, action);
    } else if (parent_){
        parent_->ForEachIn(collide, action);
    }
}

void QuadTree::Quad::Rebalance(const uint64_t targetCount, uint64_t historesis)
{
    assert(enteringEntities_.empty());
    assert(exitingEntities_.empty());
    if (!children_.empty() && RecursiveEntityCount() < targetCount - historesis) {
        entities_ = RecursivelyCollectEntities();
        children_.clear();
    } else if (children_.empty() && RecursiveEntityCount() > targetCount + historesis) {
        children_ = CreateChildren(rect_, baseTree_, this);
        for (auto entity : entities_) {
            for (auto& child : children_) {
                if (Contains(child->rect_, Point{ entity->GetX(), entity->GetY() })) {
                    child->entities_.push_back(entity);
                    continue;
                }
            }
        }
        entities_.clear();
    }

    for (auto& child : children_) {
        child->Rebalance(targetCount, historesis);
    }
}



bool QuadTree::Quad::AreAllChildrenLeafQuads()
{
    for (auto& child : children_) {
        if (!child->children_.empty()) {
            return false;
        }
    }
    return true;
}

std::vector<std::shared_ptr<Entity> > QuadTree::Quad::RecursivelyCollectEntities()
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
