#include "SenseEntityRaycast.h"

#include "Swimmer.h"
#include "EntityContainerInterface.h"

#include <QPainter>

SenseEntityRaycast::SenseEntityRaycast(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, std::vector<TraitNormaliser>&& toDetect, double maxDistance, double angle)
    : SenseTraitsBase(network, outputConnections, owner, { 0, 0, angle }, std::move(toDetect))
    , rayCastDistance_(maxDistance)
{
}

void SenseEntityRaycast::Draw(QPainter& paint) const
{
    auto l = GetLine();
    paint.setPen(QColor::fromRgb(0, 0, 0));
    paint.drawLine(QLineF(l.a.x, l.a.y, l.b.x, l.b.y));
}

Line SenseEntityRaycast::GetLine() const
{
    Transform trans = owner_.GetTransform() + transform_;
    Point begin = { trans.x, trans.y };
    Point end = ApplyOffset(begin, trans.rotation, rayCastDistance_);
    return { begin, end };
}

void SenseEntityRaycast::FilterEntities(const EntityContainerInterface& entities, const std::function<void (const Entity& e, const double& scale)>& forEachEntity) const
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
