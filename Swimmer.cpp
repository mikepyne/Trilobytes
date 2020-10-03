#include "Swimmer.h"

#include "Random.h"
#include "FoodPellet.h"
#include "Utils.h"
#include "Egg.h"
#include "Genome/GeneBrain.h"
#include "Genome/GenePigment.h"
#include "Genome/GeneSenseEntitiesInArea.h"
#include "Genome/GeneSenseRandom.h"
#include "Genome/GeneSenseMagneticField.h"

#include <QPainter>

#include <math.h>

Swimmer::Swimmer(Energy energy, const Transform& transform)
    : Swimmer(energy, transform, std::make_shared<Genome>(CreateDefaultGenome()))
{
}

Swimmer::Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome)
    : Entity(energy, transform, 6.0, genome->GetPhenoType(*this).colour)
    , genome_(genome)
    , brain_(genome->GetPhenoType(*this).brain)
    , senses_(genome->GetPhenoType(*this).senses)
{
    SetSpeed(0.5);
}

Swimmer::~Swimmer()
{
}

std::shared_ptr<Entity> Swimmer::GiveBirth(const std::shared_ptr<Genome>& other)
{
    return std::make_shared<Egg>(TakeEnergy(100_mj), GetTransform(), genome_, other ? other : genome_, Random::Poisson(50u));
}

void Swimmer::TickImpl(EntityContainerInterface& container)
{
    // TODO bearing should JUMP at Pi to -Pi (not Tau to 0), also makes maths and normalising easier
    // In the event that the brain gene is lost, or it evolves to be 0 wide
    if (brain_ && brain_->GetInputCount() > 0) {
        std::vector<double> sensoryOutput(brain_->GetInputCount(), 0.0);
        for (auto& sense : senses_) {
            sense->Tick(sensoryOutput, container, {});
        }

        brain_->ForwardPropogate(sensoryOutput);
        double newBearing = GetTransform().rotation;
        newBearing += (sensoryOutput[0]);
        if (newBearing < 0.0) {
            newBearing += EoBE::Tau;
        } else if (newBearing > EoBE::Tau) {
            newBearing -= EoBE::Tau;
        }
        SetBearing(newBearing);
    }

    UseEnergy(200_uj); // TODO remove this, entities will use energy based on what they are doing (well maybe a small base usage would deter couch potatoes...)

    std::shared_ptr<Genome> otherGenes;
    container.ForEachCollidingWith(Circle{ GetTransform().x, GetTransform().y, GetRadius() }, [&](Entity& other) -> void
    {
        if (&other != this) {
            if (FoodPellet* f = dynamic_cast<FoodPellet*>(&other)) {
                FeedOn(*f, f->GetEnergy());
                assert(!f->Alive());
            } else if (Swimmer* s = dynamic_cast<Swimmer*>(&other)) {
                otherGenes = s->genome_;
            }
        }
    });

    if (GetEnergy() > 300_mj) {
        container.AddEntity(GiveBirth(otherGenes));
    }
}

void Swimmer::DrawImpl(QPainter& paint)
{
    paint.save();
    paint.drawEllipse({ GetTransform().x, GetTransform().y }, GetRadius(), GetRadius());
    paint.setBrush(GetColour());
    paint.drawEllipse({ GetTransform().x, GetTransform().y }, GetRadius(), GetRadius());
    paint.restore();

    for (auto& sense : senses_) {
        paint.save();
        sense->Draw(paint);
        paint.restore();
    }
}

std::vector<std::shared_ptr<Gene> > Swimmer::CreateDefaultGenome()
{
    // TODO create genes for the following
    // std::make_shared<SenseEntitiesTouching>(*this, 0.0, 0.0, 1.0, std::vector<std::pair<double, Trait>>{ /*{1.0, Trait::Green},*/ }),
    // std::make_shared<SenseEntityRaycast>(*this, GetRadius() * 2, 0.0, std::vector<Trait>{}),

    unsigned brainWidth = 7;
    return {
        std::make_shared<GenePigment>(),
        std::make_shared<GenePigment>(),
        std::make_shared<GenePigment>(),
        std::make_shared<GenePigment>(),
        std::make_shared<GeneBrain>(3, brainWidth, 0.5),
        std::make_shared<GeneSenseEntitiesInArea>(std::vector<std::pair<double, Trait>>{ {1.0, Trait::Green}, }, 0, brainWidth, Transform{ 0, 24,  0.6}, 20, 1.0),
        std::make_shared<GeneSenseEntitiesInArea>(std::vector<std::pair<double, Trait>>{ {1.0, Trait::Green}, }, 0, brainWidth, Transform{ 0, 24, -0.6}, 20, 1.0),
        std::make_shared<GeneSenseRandom>(1, brainWidth),
        std::make_shared<GeneSenseMagneticField>(brainWidth),
    };
}
