#include "SenseTraitsRaycast.h"

#include "Swimmer.h"
#include "EntityContainerInterface.h"

#include <QPainter>

SenseTraitsRaycast::SenseTraitsRaycast(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, std::vector<TraitNormaliser>&& toDetect, const Transform& transform, double maxDistance, double angle)
    : SenseTraitsBase(network, outputConnections, owner, transform, std::move(toDetect))
    , rayCastDistance_(maxDistance)
    , rayCastAngle_(angle)
{
}

void SenseTraitsRaycast::Draw(QPainter& paint) const
{
    auto l = GetLine();
    paint.setPen(QColor::fromRgb(0, 0, 0));
    paint.drawLine(QLineF(l.a.x, l.a.y, l.b.x, l.b.y));
}

Line SenseTraitsRaycast::GetLine() const
{
    // TODO this is really why I need to get on with implementing a real transform class...
    Point begin = ApplyOffset({ owner_.GetTransform().x, owner_.GetTransform().y }, transform_.rotation + owner_.GetTransform().rotation, GetDistance({0, 0}, {transform_.x, transform_.y}));
    Point end = ApplyOffset(begin, transform_.rotation + owner_.GetTransform().rotation + rayCastAngle_, rayCastDistance_);
    return { begin, end };
}

void SenseTraitsRaycast::FilterEntities(const EntityContainerInterface& entities, const std::function<void (const Entity& e, const double& scale)>& forEachEntity) const
{
    Line rayCastLine = GetLine();
    std::shared_ptr<Entity> nearestEntity;
    double distanceToNearestSquared = 0.0;
    entities.ForEachCollidingWith(rayCastLine, [&](const std::shared_ptr<Entity>& e)
    {
        // don't detect ourself
        if (e.get() != &owner_) {
            // TODO this distance assumes head on detection, not incidental ones
            double distanceSquare = GetDistanceSquare(rayCastLine.a, e->GetLocation());
            if (nearestEntity == nullptr || distanceSquare < distanceToNearestSquared) {
                nearestEntity = e;
                distanceToNearestSquared = distanceSquare;
            }
        }
    });

    if (nearestEntity != nullptr) {
        // Only nearest entity detected, scaled by distance from detector
        forEachEntity(*nearestEntity, ( 1.0 - (std::sqrt(distanceToNearestSquared) / rayCastDistance_)));
    }
}
