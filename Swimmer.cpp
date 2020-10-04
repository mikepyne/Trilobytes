#include "Swimmer.h"

#include "Random.h"
#include "FoodPellet.h"
#include "Utils.h"
#include "Egg.h"
#include "Genome/GeneBrain.h"
#include "Genome/GenePigment.h"
#include "Genome/GeneSenseRandom.h"
#include "Genome/GeneSenseMagneticField.h"
#include "Genome/GeneSenseEntityRaycast.h"
#include "Genome/GeneSenseEntitiesInArea.h"
#include "Genome/GeneSenseEntitiesTouching.h"

// TODO remove this once the Tail isn't hard-coded
#include "Effectors/EffectorTail.h"

#include <QPainter>

#include <math.h>

Swimmer::Swimmer(Energy energy, const Transform& transform)
    : Swimmer(energy, transform, std::make_shared<Genome>(CreateDefaultGenome()))
{
}

// TODO prevent multiple re-calculation of phenotype
Swimmer::Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome)
    : Entity(energy, transform, 6.0, genome->GetPhenoType(*this).colour)
    , genome_(genome)
    , brain_(genome->GetPhenoType(*this).brain)
    , senses_(genome->GetPhenoType(*this).senses)
//    , effectors_(genome.GetPhenoType(*this).effectors)
    , brainValues_(brain_->GetInputCount(), 0.0)
{
    effectors_.push_back(std::make_shared<EffectorTail>(std::make_shared<NeuralNetwork>(0, 3, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(NeuralNetworkConnector(brain_->GetOutputCount(), 3)), *this));
}

Swimmer::~Swimmer()
{
}

std::shared_ptr<Entity> Swimmer::GiveBirth(const std::shared_ptr<Genome>& other)
{
    return std::make_shared<Egg>(TakeEnergy(100_mj), GetTransform(), genome_, other ? other : genome_, Random::Poisson(50u));
}

void Swimmer::AdjustVelocity(double adjustment)
{
    UseEnergy(500_uj * std::pow(adjustment, 2.0));
    SetVelocity(GetVelocity() + adjustment);
}

void Swimmer::AdjustBearing(double adjustment)
{
    UseEnergy(10_uj * std::pow(adjustment, 2.0));
    SetBearing(GetTransform().rotation + adjustment);
}

void Swimmer::TickImpl(EntityContainerInterface& container)
{
    if (brain_ && brain_->GetInputCount() > 0) {
        std::fill(std::begin(brainValues_), std::end(brainValues_), 0.0);
        for (auto& sense : senses_) {
            sense->Tick(brainValues_, container);
        }

        brain_->ForwardPropogate(brainValues_);

        for (auto& effector : effectors_) {
            effector->Tick(brainValues_, container);
        }
    }

    // Baseline energy use TODO adjust based on genome (sense + effector count e.t.c.)
    UseEnergy(20_uj);

    std::shared_ptr<Genome> otherGenes;
    container.ForEachCollidingWith(Circle{ GetTransform().x, GetTransform().y, GetRadius() }, [&](const std::shared_ptr<Entity>& other) -> void
    {
        if (other.get() != this) {
            if (FoodPellet* f = dynamic_cast<FoodPellet*>(other.get())) {
                FeedOn(*f, f->GetEnergy());
                assert(!f->Alive());
            } else if (Swimmer* s = dynamic_cast<Swimmer*>(other.get())) {
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
    // EffectorTail

    unsigned brainWidth = 7;
    return {
        std::make_shared<GenePigment>(),
        std::make_shared<GenePigment>(),
        std::make_shared<GeneBrain>(3, brainWidth, 0.5),
        std::make_shared<GeneSenseRandom>(1, brainWidth),
        std::make_shared<GeneSenseMagneticField>(brainWidth),
        std::make_shared<GeneSenseEntitiesInArea>(std::vector<std::pair<double, Trait>>{ {1.0, Trait::Green}, }, 0, brainWidth, Transform{ 0, 24,  0.6}, 20, 1.0),
        std::make_shared<GeneSenseEntitiesInArea>(std::vector<std::pair<double, Trait>>{ {1.0, Trait::Green}, }, 0, brainWidth, Transform{ 0, 24, -0.6}, 20, 1.0),
        std::make_shared<GeneSenseEntityRaycast>(std::vector<std::pair<double, Trait>>{ {1.0, Trait::Size}, }, 0, brainWidth, Transform{ 0, -30, 0 }, 1.0),
        std::make_shared<GeneSenseEntitiesTouching>(std::vector<std::pair<double, Trait>>{ {1.0, Trait::Age}, }, 0, brainWidth, Transform{ 0, 0, 0 }, 1.0),
    };
}
