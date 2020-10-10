#include "Egg.h"

#include "Swimmer.h"

#include <QPainter>

Egg::Egg(std::shared_ptr<Swimmer>&& mother, Energy energy, const Transform& transform, std::shared_ptr<Genome> genomeOne, std::shared_ptr<Genome> genomeTwo, unsigned hatchingDelay)
    : Entity(energy, transform, 7, mother->GetVelocity(), QColor::fromRgb(125, 57, 195))
    , mother_(std::move(mother))
    , genomeOne_(genomeOne)
    , genomeTwo_(genomeTwo)
    , hatchingDelay_(hatchingDelay)
{
}

Egg::~Egg()
{
}

void Egg::TickImpl(EntityContainerInterface& container)
{
    if (hatchingDelay_ > 0) {
        hatchingDelay_--;
    } else {
        // cross with self for now
        std::shared_ptr<Genome> genome = Genome::CreateOffspring(*genomeOne_, *genomeTwo_);
        if (genome) {
            container.AddEntity(std::make_shared<Swimmer>(GetEnergy(), GetTransform(), genome, std::move(mother_)));
        }
        Terminate();
    }
}

void Egg::DrawImpl(QPainter& paint)
{
    paint.drawEllipse(QPointF(GetTransform().x, GetTransform().y), GetRadius() / 2.0, GetRadius() / 3.0);
}
