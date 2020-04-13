#include "Egg.h"

#include "Swimmer.h"

#include <QPainter>

Egg::Egg(EnergyPool&& energy, double x, double y, NeuralNetwork&& brain, std::shared_ptr<Genome> genome, unsigned hatchingDelay)
    : Entity(std::move(energy), x, y, 7, QColor::fromRgb(125, 57, 195))
    , genome_(genome)
    , brain_(std::move(brain))
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
            container.AddEntity(std::make_shared<Swimmer>(TakeEnergy(GetEnergy()), GetX(), GetY(), std::move(brain_), genome));
        } else {
            UseEnergy(GetEnergy());
        }
    }
}

void Egg::DrawImpl(QPainter& paint)
{
    paint.drawEllipse(QPointF(GetX(), GetY()), GetRadius() / 2.0, GetRadius() / 3.0);
}
