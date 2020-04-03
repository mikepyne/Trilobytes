#include "Seed.h"

#include "Random.h"
#include "FoodPellet.h"

#include <QPainter>

Seed::Seed(EnergyPool&& energy, double x, double y, unsigned germinationDelay)
    : Entity(std::move(energy), x, y, 2.0, QColor::fromRgb(209, 225, 200))
    , germinationDelay_(germinationDelay)
{
}

void Seed::TickImpl(EntityContainerInterface& container)
{
    if (germinationDelay_ > 0) {
        germinationDelay_--;
    } else {
        container.AddEntity(std::make_shared<FoodPellet>(TakeEnergy(GetEnergy()), GetX(), GetY()));
    }
}

void Seed::DrawImpl(QPainter& paint)
{
    paint.drawEllipse(QPointF(GetX(), GetY()), GetRadius() / 2.0, GetRadius());
}
