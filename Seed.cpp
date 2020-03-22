#include "Seed.h"

#include "Random.h"
#include "FoodPellet.h"

#include <QPainter>

Seed::Seed(EnergyPool&& energy, double x, double y, unsigned germinationDelay)
    : Entity(std::move(energy), x, y, 1.0)
    , germinationDelay_(germinationDelay)
{
}

bool Seed::Tick(EntityContainerInterface& container)
{
    if (germinationDelay_ > 0) {
        germinationDelay_--;
    } else {
        container.AddEntity(std::make_shared<FoodPellet>(energy_.CreateChild(GetEnergy()), GetX(), GetY()));
    }
    return false;
}

void Seed::Draw(QPainter& paint)
{
    paint.save();
    paint.setBrush(QColor(209, 225, 200));
    paint.drawEllipse(QPointF(GetX(), GetY()), GetRadius(), GetRadius());
    paint.restore();
}
