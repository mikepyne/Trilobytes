#include "SenseEntitiesTouching.h"

#include <QPainter>

SenseEntitiesTouching::SenseEntitiesTouching(Entity& owner, double offsetDistance, double offsetAngle, double genericDetectionWeight, const std::vector<std::pair<double, Trait> >&& toDetect)
    : Sense(owner, 1 + toDetect.size(), std::min(toDetect.size(), 3u))
    , offsetDistance_(offsetDistance)
    , offsetAngle_(offsetAngle)
    , genericDetectionWeight_(genericDetectionWeight)
{
}

void SenseEntitiesTouching::Draw(QPainter& paint) const
{
    Point location = ApplyOffset(owner_.GetLocation(), offsetAngle_ + owner_.GetBearing(), offsetDistance_);
    paint.drawEllipse(QPointF(location.x, location.y), 1.0, 1.0);
}

void SenseEntitiesTouching::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef& /*environment*/)
{
    for (auto& input : inputs) {
        input = 0.0;
    }

    Point location = ApplyOffset(owner_.GetLocation(), offsetAngle_ + owner_.GetBearing(), offsetDistance_);
    entities.ForEachCollidingWith(location, [&](Entity& e)
    {
        // don't detect ourself
        if (&e != &owner_) {
            inputs[0] += genericDetectionWeight_;
            size_t i = 0;
            for (auto& [traitWeight, trait ] : toDetect_) {
                inputs[++i] += traitWeight * e.GetTrait(trait);
            }
        }
    });
}
