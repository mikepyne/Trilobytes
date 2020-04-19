#include "SenseEntityPresence.h"

#include <QPainter>

SenseEntityPresence::CustomFilter SenseEntityPresence::MakeDefaultFilter(double detectionQuantity)
{
    return MakeCustomFilter<Entity>(0, { detectionQuantity });
}

void SenseEntityPresence::Draw(QPainter& paint) const
{
    Point senseCentre = ApplyOffset(owner_.GetLocation(), owner_.GetBearing() + angleOffset_, distanceOffset_);
    auto& [ x, y ] = senseCentre;
    paint.drawEllipse(QPointF(x, y), range_, range_);
}

void SenseEntityPresence::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef& /*environment*/)
{
    for (double& input : inputs) {
        input = 0.0;
    }

    Point senseCentre = ApplyOffset(owner_.GetLocation(), owner_.GetBearing() + angleOffset_, distanceOffset_);
    auto& [ x, y ] = senseCentre;

    entities.ForEachCollidingWith(Circle{ x, y, range_ }, [&](Entity& e)
    {
        if (auto optional = entityFilter_(e); optional.has_value()) {
            auto [ index, value ] = optional.value();
            inputs[index] += value;
        }
    });
}
