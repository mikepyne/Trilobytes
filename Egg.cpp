#include "Egg.h"

#include "Swimmer.h"

#include <QPainter>

Egg::Egg(EnergyPool&& energy, double x, double y, NeuralNetwork&& brain, unsigned hatchingDelay)
    : Entity(std::move(energy), x, y, 7, QColor::fromRgb(125, 57, 195))
    , brain_(std::move(brain))
    , hatchingDelay_(hatchingDelay)
{
}

void Egg::TickImpl(EntityContainerInterface& container)
{
    if (hatchingDelay_ > 0) {
        hatchingDelay_--;
    } else {
        container.AddEntity(std::make_shared<Swimmer>(energy_.CreateChild(GetEnergy()), GetX(), GetY(), std::move(brain_)));
    }
}

void Egg::DrawImpl(QPainter& paint)
{
    paint.drawEllipse(QPointF(GetX(), GetY()), GetRadius() / 2.0, GetRadius() / 3.0);
}
