#include "SenseEntityRaycast.h"

#include <QPainter>

SenseEntityRaycast::SenseEntityRaycast(Entity& owner, double maxDistance, double angle, const std::vector<Trait>&& toDetect)
    : Sense(owner, 1 + toDetect.size(), std::min(toDetect.size(), 3u))
    , rayCastDistance_(maxDistance)
    , rayCastAngle_(angle)
    , toDetect_(toDetect)
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
        inputs[0] = std::sqrt(distanceToNearestSquare) / rayCastDistance_;
        for (unsigned i = 0; i < toDetect_.size(); i++) {
            inputs[i + 1] = nearestEntity->GetTrait(toDetect_[i]);
        }
    }
}

Line SenseEntityRaycast::GetLine() const
{
    Point begin = ApplyOffset(owner_.GetLocation(), rayCastAngle_ + owner_.GetBearing(), owner_.GetRadius());
    Point end = ApplyOffset(begin, rayCastAngle_ + owner_.GetBearing(), rayCastDistance_);
    return { begin, end };
}
