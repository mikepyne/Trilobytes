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

void Egg::TickImpl(EntityContainerInterface& container, const UniverseParameters& universeParameters)
{
    // TODO could delay depending on ammount of energy in the egg
    // could delay based on the number of genes / their complexity
    // could add an iterator to genome, and each tich process the next gene, use some energy and hatch with remaining energy once done

    // TODO add a gene to decide how much energy to pass to eggs


    if (hatchingDelay_ > 0) {
        hatchingDelay_--;
    } else {
        // cross with self for now
        std::shared_ptr<Genome> genome = Genome::CreateOffspring(*genomeOne_, *genomeTwo_, universeParameters);
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
