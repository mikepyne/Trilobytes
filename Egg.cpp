#include "Egg.h"

#include "Swimmer.h"

#include <QPainter>

Egg::Egg(EnergyPool&& energy, const Transform& transform, std::shared_ptr<Genome> genome, unsigned hatchingDelay)
    : Entity(std::move(energy), transform, 7, QColor::fromRgb(125, 57, 195))
    , genome_(genome)
    , hatchingDelay_(hatchingDelay)
{
}

void Egg::TickImpl(EntityContainerInterface& container)
{
    if (hatchingDelay_ > 0) {
        hatchingDelay_--;
    } else {
        // cross with self for now
        std::shared_ptr<Genome> genome = Genome::CreateOffspring(*genome_, *genome_);
        if (genome) {
            container.AddEntity(std::make_shared<Swimmer>(TakeEnergy(GetEnergy()), GetTransform(), genome));
        } else {
            UseEnergy(GetEnergy());
        }
    }
}

void Egg::DrawImpl(QPainter& paint)
{
    paint.drawEllipse(QPointF(GetTransform().x, GetTransform().y), GetRadius() / 2.0, GetRadius() / 3.0);
}
