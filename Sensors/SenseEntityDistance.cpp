#include "SenseEntityDistance.h"

#include <QPainter>

SenseEntityDistance::CustomFilter SenseEntityDistance::MakeDefaultFilter(double detectionQuantity)
{
    return MakeCustomFilter<Entity>(0, { detectionQuantity });
}

void SenseEntityDistance::Draw(QPainter& paint) const
{
    paint.setBrush(QColor(0, 0, 0, 0));
    Point senseCentre = ApplyOffset(owner_.GetLocation(), owner_.GetBearing() + angleOffset_, distanceOffset_);
    auto& [ x, y ] = senseCentre;
    paint.drawEllipse(QPointF(x, y), range_, range_);
}

void SenseEntityDistance::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef& /*environment*/)
{
    for (double& input : inputs) {
        input = 0.0;
    }

    Point senseCentre = ApplyOffset(owner_.GetLocation(), owner_.GetBearing() + angleOffset_, distanceOffset_);
    auto& [ x, y ] = senseCentre;

    entities.ForEachCollidingWith(Circle{ x, y, range_ }, [&](Entity& e)
    {
        double distanceSquare = GetDistanceSquare(e.GetLocation(), senseCentre);
        double smellWeight = 1.0 - (distanceSquare * (1.0 / (std::pow(range_ + 1, 2.0))));

        if (auto optional = entityFilter_(e); optional.has_value()) {
            auto [ index, value ] = optional.value();
            inputs[index] += (value * smellWeight);
        }
    });
}
