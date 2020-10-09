#include "SenseEntitiesTouching.h"

#include "Swimmer.h"

#include <QPainter>

SenseEntitiesTouching::SenseEntitiesTouching(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, Transform transform, std::vector<TraitNormaliser>&& toDetect)
    : SenseTraitsBase(network, outputConnections, owner, transform, std::move(toDetect))
{
}

void SenseEntitiesTouching::Draw(QPainter& paint) const
{
    Point location = GetPoint();
    paint.setPen(QColor::fromRgb(255, 0, 0));
    paint.setBrush(QColor::fromRgb(0, 0, 0, 0));
    paint.drawEllipse(QPointF(location.x, location.y), 1.0, 1.0);
}

Point SenseEntitiesTouching::GetPoint() const
{
    Transform t = owner_.GetTransform() + transform_;
    return { t.x, t.y };
}

void SenseEntitiesTouching::FilterEntities(const EntityContainerInterface& entities, const std::function<void (const Entity&, const double&)>& forEachEntity) const
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
