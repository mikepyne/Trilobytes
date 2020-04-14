#include "FoodPellet.h"

#include "FeedDispenser.h"

#include <QPainter>

FoodPellet::FoodPellet(FeedDispenser& spawner, EnergyPool&& energy, double x, double y)
    : Entity(std::move(energy), x, y, 2.5, QColor::fromRgb(15, 235, 15))
    , spawner_(spawner)
{
}

FoodPellet::~FoodPellet()
{
    spawner_.PelletEaten();
}

void FoodPellet::TickImpl(EntityContainerInterface& /*container*/)
{
}

void FoodPellet::DrawImpl(QPainter& paint)
{
    paint.drawEllipse(QPointF{GetX(), GetY()}, GetRadius(), GetRadius());
}
