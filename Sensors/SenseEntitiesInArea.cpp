#include "SenseEntitiesInArea.h"

#include <QPainter>

SenseEntitiesInArea::SenseEntitiesInArea(Entity& owner, unsigned outputCount, double maxDistance, double offsetDistance, double offsetAngle, double senseDistanceWeight, const std::vector<std::pair<double, Trait>>&& toDetect)
    : Sense(owner, 1 + toDetect.size(), std::min(toDetect.size(), 3u), outputCount)
    , senseRadius_(maxDistance)
    , offsetDistance_(offsetDistance)
    , offsetAngle_(offsetAngle)
    , senseDistanceWeight_(senseDistanceWeight)
    , toDetect_(std::move(toDetect))
{
}

void SenseEntitiesInArea::Draw(QPainter& paint) const
{
    Circle c = GetArea();
    paint.setPen(QColor::fromRgb(0, 0, 0));
    paint.setBrush(QColor::fromRgb(0, 0, 0, 0));
    paint.drawEllipse(QPointF(c.x, c.y), senseRadius_, senseRadius_);
}

void SenseEntitiesInArea::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef& /*environment*/)
{
    for (auto& input : inputs) {
        input = 0.0;
    }

    Circle senseArea = GetArea();
    Point senseCentre = { senseArea.x, senseArea.y };
    double senseRadiusSquare = std::pow(senseArea.radius, 2.0);
    entities.ForEachCollidingWith(senseArea, [&](Entity& e)
    {
        // don't detect ourself
        if (&e != &owner_) {
            double distanceSquare = GetDistanceSquare(senseCentre, e.GetLocation());
            if (distanceSquare < senseRadiusSquare) {
                double distanceProportion = 1.0 - (distanceSquare / senseRadiusSquare);
                inputs[0] += distanceProportion * senseDistanceWeight_;
                size_t i = 0;
                for (auto& [traitWeight, trait ] : toDetect_) {
                    inputs[++i] += distanceProportion * traitWeight * e.GetTrait(trait);
                }
            }
        }
    });
}

Circle SenseEntitiesInArea::GetArea() const
{
    Point centre = ApplyOffset(owner_.GetLocation(), offsetAngle_ + owner_.GetBearing(), offsetDistance_);
    return { centre.x, centre.y, senseRadius_ };
}
