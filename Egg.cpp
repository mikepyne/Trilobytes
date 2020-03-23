#include "Egg.h"

#include "Swimmer.h"

#include <QPainter>

Egg::Egg(EnergyPool&& energy, double x, double y, NeuralNetwork&& brain, unsigned hatchingDelay)
    : Entity(std::move(energy), x, y, 7)
    , brain_(std::move(brain))
    , hatchingDelay_(hatchingDelay)
{
}

bool Egg::Tick(EntityContainerInterface& container)
{
    if (hatchingDelay_ > 0) {
        hatchingDelay_--;
    } else {
        container.AddEntity(std::make_shared<Swimmer>(energy_.CreateChild(GetEnergy()), GetX(), GetY(), std::move(brain_)));
    }
    return false;
}

void Egg::Draw(QPainter& paint)
{
    paint.save();
    paint.setBrush(QColor(125, 57, 195));
    paint.drawEllipse(QPointF(GetX(), GetY()), GetRadius() / 2.0, GetRadius() / 3.0);
    paint.restore();
}
