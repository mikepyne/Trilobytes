#include "FoodPellet.h"

#include "FeedDispenser.h"
#include "RangeConverter.h"

#include <QPainter>

double FoodPellet::GetPelletRadius(const Energy& energy)
{
    static Tril::RangeConverter energyToSize({ 1_mj, 30_mj }, { 0.5, 2.5 });
    return energyToSize.Convert(energy);
}

FoodPellet::FoodPellet(const std::shared_ptr<FeedDispenser>& spawner, Energy energy, const Transform& transform)
    : Entity(transform, GetPelletRadius(energy), QColor::fromRgb(15, 235, 15), energy)
    , spawner_(spawner)
{
}

FoodPellet::~FoodPellet()
{
    spawner_->PelletEaten();
}

void FoodPellet::DrawImpl(QPainter& paint)
{
    paint.drawEllipse(QPointF{ GetTransform().x, GetTransform().y }, GetRadius(), GetRadius());
}
