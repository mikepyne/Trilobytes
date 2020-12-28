#include "MeatChunk.h"

#include <QPainter>

MeatChunk::MeatChunk(const Energy& energy, const Transform& transform, const double& speed)
    : Entity(energy, transform, GetRadius(energy), speed, QColor::fromRgb(185, 48, 49))
{
}

MeatChunk::~MeatChunk()
{
}

void MeatChunk::TickImpl(EntityContainerInterface& /*container*/)
{
    UseEnergy(GetEnergy() / 500.0);
    SetRadius(GetRadius(GetEnergy()));
    if (GetEnergy() < 1_mj) {
        Terminate();
    }
}

void MeatChunk::DrawImpl(QPainter& paint)
{
    paint.drawEllipse(QPointF{ GetTransform().x, GetTransform().y }, GetRadius(GetEnergy()), GetRadius(GetEnergy()));
}

double MeatChunk::GetRadius(const Energy& energy)
{
    // Size range 2.0->6.0 scaling linearly with energy quantity
    static EoBE::RangeConverter energyToSize({ 1_mj, 20_mj }, { 0.5, 3.5 });
    return energyToSize.ConvertAndClamp(energy);
}
