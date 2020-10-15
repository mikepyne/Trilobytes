#include "SenseTraitsTouching.h"

#include "Swimmer.h"

#include <QPainter>

SenseTraitsTouching::SenseTraitsTouching(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, Transform transform, std::vector<TraitNormaliser>&& toDetect)
    : SenseTraitsBase(network, outputConnections, owner, transform, std::move(toDetect))
{
}

void SenseTraitsTouching::Draw(QPainter& paint) const
{
    Point location = GetPoint();
    paint.setPen(QColor::fromRgb(255, 0, 0));
    paint.setBrush(QColor::fromRgb(0, 0, 0, 0));
    paint.drawEllipse(QPointF(location.x, location.y), 1.0, 1.0);
}

Point SenseTraitsTouching::GetPoint() const
{
    return ApplyOffset(owner_.GetLocation(), transform_.rotation + owner_.GetTransform().rotation, GetDistance({ 0, 0 }, { transform_.x, transform_.y }));
}

void SenseTraitsTouching::FilterEntities(const EntityContainerInterface& entities, const std::function<void (const Entity&, const double&)>& forEachEntity) const
{
    Point location = GetPoint();
    entities.ForEachCollidingWith(location, [&](const std::shared_ptr<Entity>& e)
    {
        // don't detect ourself
        if (e.get() != &owner_) {
            // All entities touching are detected at 100%
            forEachEntity(*e, 1.0);
        }
    });
}
