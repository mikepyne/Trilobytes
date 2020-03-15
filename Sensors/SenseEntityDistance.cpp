#include "SenseEntityDistance.h"

#include <QPainter>

SenseEntityDistance::CustomFilter SenseEntityDistance::MakeDefaultFilter(double detectionQuantity)
{
    return MakeCustomFilter<Entity>(0, { detectionQuantity });
}

void SenseEntityDistance::Draw(QPainter& paint) const
{
    paint.setBrush(QColor(0, 0, 0, 0));
    double offsetBearing = GetBearing({0, 0}, {xOffset_, yOffset_});
    double offsetDistance = std::sqrt(GetDistanceSquare({0, 0}, {xOffset_, yOffset_}));

    double x = owner_.GetX() + (std::sin(owner_.GetBearing() + offsetBearing) * offsetDistance);
    double y = owner_.GetY() + (-std::cos(owner_.GetBearing() + offsetBearing) * offsetDistance);
    paint.drawEllipse(QPointF(x, y), range_, range_);
}

void SenseEntityDistance::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef& /*environment*/)
{
    for (double& input : inputs) {
        input = 0.0;
    }

    double offsetBearing = GetBearing({0, 0}, {xOffset_, yOffset_});
    double offsetDistance = std::sqrt(GetDistanceSquare({0, 0}, {xOffset_, yOffset_}));

    double x = owner_.GetX() + (std::sin(owner_.GetBearing() + offsetBearing) * offsetDistance);
    double y = owner_.GetY() + (-std::cos(owner_.GetBearing() + offsetBearing) * offsetDistance);

    entities.ForEachIn(Circle{ x, y, range_ }, [&](Entity& e)
    {
        double distanceSquare = GetDistanceSquare(e.GetLocation(), { x, y });
        double smellWeight = 1.0 - (distanceSquare * (1.0 / (std::pow(range_ + 1, 2.0))));

        if (auto optional = entityFilter_(e); optional.has_value()) {
            auto [ index, value ] = optional.value();
            inputs[index] += (value * smellWeight);
        }
    });
}
