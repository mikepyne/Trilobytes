#include "SenseEntityPresence.h"

#include <QPainter>

SenseEntityPresence::CustomFilter SenseEntityPresence::MakeDefaultFilter(double detectionQuantity)
{
    return MakeCustomFilter<Entity>(0, { detectionQuantity });
}

void SenseEntityPresence::Draw(QPainter& paint) const
{
    paint.drawEllipse(QPointF(owner_.GetX() + xOffset_, owner_.GetY() + yOffset_), range_, range_);
}

void SenseEntityPresence::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const Sense::UniverseInfoStructRef& /*environment*/)
{
    for (double& input : inputs) {
        input = 0.0;
    }

    entities.ForEachIn(Circle{ owner_.GetX() + xOffset_, owner_.GetY() + yOffset_, range_ }, [&](Entity& e)
    {
        if (auto optional = entityFilter_(e); optional.has_value()) {
            auto [ index, value ] = optional.value();
            inputs[index] += value;
        }
    });
}
