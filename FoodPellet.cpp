#include "FoodPellet.h"

#include "FeedDispenser.h"

#include <QPainter>

FoodPellet::FoodPellet(FeedDispenser& spawner, Energy energy, const Transform& transform)
    : Entity(energy, transform, 2.5, QColor::fromRgb(15, 235, 15))
    , spawner_(spawner)
{
}

FoodPellet::~FoodPellet()
{
    spawner_.PelletEaten();
}

void FoodPellet::DrawImpl(QPainter& paint)
{
    paint.drawEllipse(QPointF{ GetTransform().x, GetTransform().y }, GetRadius(), GetRadius());
}
