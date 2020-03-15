#include "SenseEntityPresence.h"

#include <QPainter>

SenseEntityPresence::CustomFilter SenseEntityPresence::MakeDefaultFilter(double detectionQuantity)
{
    return MakeCustomFilter<Entity>(0, { detectionQuantity });
}

void SenseEntityPresence::Draw(QPainter& paint) const
{
    double offsetBearing = GetBearing({0, 0}, {xOffset_, yOffset_});
    double offsetDistance = std::sqrt(GetDistanceSquare({0, 0}, {xOffset_, yOffset_}));

    double x = owner_.GetX() + (std::sin(owner_.GetBearing() + offsetBearing) * offsetDistance);
    double y = owner_.GetY() + (-std::cos(owner_.GetBearing() + offsetBearing) * offsetDistance);
    paint.drawEllipse(QPointF(x, y), range_, range_);
}

void SenseEntityPresence::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef& /*environment*/)
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
        if (auto optional = entityFilter_(e); optional.has_value()) {
            auto [ index, value ] = optional.value();
            inputs[index] += value;
        }
    });
}
