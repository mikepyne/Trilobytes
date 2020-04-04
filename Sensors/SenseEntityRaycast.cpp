#include "SenseEntityRaycast.h"

#include <QPainter>

SenseEntityRaycast::SenseEntityRaycast(Entity& owner, double maxDistance, double angle)
    : Sense(owner, 1, 0)
    , rayCastDistance_(maxDistance)
    , rayCastAngle_(angle)
{
}

void SenseEntityRaycast::Draw(QPainter& paint) const
{
    auto l = GetLine();
    paint.drawLine(QLineF(l.a.x, l.a.y, l.b.x, l.b.y));
}

void SenseEntityRaycast::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef&)
{
    Line rayCastLine = GetLine();
    for (double& input : inputs) {
        Entity* nearestEntity = nullptr;
        double distanceToNearestSquare = 0.0;
        entities.ForEachCollidingWith(rayCastLine, [&](Entity& e)
        {
            // don't detect ourself
            if (&e != &owner_) {
                double distanceSquare = GetDistanceSquare(owner_.GetLocation(), e.GetLocation());
                if (nearestEntity == nullptr || distanceSquare < distanceToNearestSquare) {
                    nearestEntity = &e;
                    distanceToNearestSquare = distanceSquare;
                }
            }
        });

        if (nearestEntity != nullptr) {
            input = std::sqrt(distanceToNearestSquare) / rayCastDistance_;
        }
    }
}

Line SenseEntityRaycast::GetLine() const
{
    Point begin = ApplyOffset(owner_.GetLocation(), rayCastAngle_ + owner_.GetBearing(), owner_.GetRadius());
    Point end = ApplyOffset(begin, rayCastAngle_ + owner_.GetBearing(), rayCastDistance_);
    return { begin, end };
}
