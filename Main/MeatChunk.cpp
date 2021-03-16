#include "MeatChunk.h"

#include "RangeConverter.h"

#include <QPainter>

MeatChunk::MeatChunk(const Energy& energy, const Transform& transform, const double& speed)
    : Entity(transform, GetMeatChunkRadius(energy), QColor::fromRgb(185, 48, 49), energy, speed)
{
}

MeatChunk::~MeatChunk()
{
}

void MeatChunk::TickImpl(EntityContainerInterface& /*container*/, const UniverseParameters& /*universeParameters*/)
{
    UseEnergy(GetEnergy() / 500.0);
    SetRadius(GetMeatChunkRadius(GetEnergy()));
    if (GetEnergy() < 1_mj) {
        Terminate();
    }
}

void MeatChunk::DrawImpl(QPainter& paint)
{
    paint.drawEllipse(QPointF{ GetTransform().x, GetTransform().y }, GetMeatChunkRadius(GetEnergy()), GetMeatChunkRadius(GetEnergy()));
}

double MeatChunk::GetMeatChunkRadius(const Energy& energy)
{
    // Size range 2.0->6.0 scaling linearly with energy quantity
    static Tril::RangeConverter energyToSize({ 1_mj, 20_mj }, { 0.5, 3.5 });
    return energyToSize.ConvertAndClamp(energy);
}
