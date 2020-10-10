#include "Swimmer.h"

#include "Random.h"
#include "FoodPellet.h"
#include "Utils.h"
#include "Egg.h"
#include "Genome/GeneBrain.h"
#include "Genome/GenePigment.h"
#include "Genome/GeneSenseRandom.h"
#include "Genome/GeneSenseMagneticField.h"
#include "Genome/GeneEffectorTail.h"
#include "Genome/GeneSenseTraitsRaycast.h"
#include "Genome/GeneSenseTraitsInArea.h"
#include "Genome/GeneSenseTraitsTouching.h"
#include "Genome/GeneSenseTraitsSelf.h"

#include <QPainter>

#include <math.h>

Swimmer::Swimmer(Energy energy, const Transform& transform)
    : Swimmer(energy, transform, std::make_shared<Genome>(CreateDefaultGenome()))
{
}

Swimmer::Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome)
    : Swimmer(energy, transform, genome, genome->GetPhenoType(*this))
{
}

Swimmer::~Swimmer()
{
}

std::shared_ptr<Entity> Swimmer::GiveBirth(const std::shared_ptr<Genome>& other)
{
    eggsLayed_++;
    return std::make_shared<Egg>(TakeEnergy(100_mj), GetTransform(), genome_, other ? other : genome_, Random::Poisson(50u));
}

void Swimmer::AdjustVelocity(double adjustment)
{
    UseEnergy(350_uj * std::pow(adjustment, 2.0));
    SetVelocity(GetVelocity() + adjustment);
}

void Swimmer::AdjustBearing(double adjustment)
{
    UseEnergy(100_uj * std::pow(adjustment, 2.0));
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

Swimmer::Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome, const Phenotype& phenotype)
    : Entity(energy, transform, 6.0, phenotype.colour)
    , genome_(genome)
    , brain_(phenotype.brain)
    , senses_(phenotype.senses)
    , effectors_(phenotype.effectors)
    , brainValues_(brain_->GetInputCount(), 0.0)
    , eggsLayed_(0)
    {
    }

std::vector<std::shared_ptr<Gene> > Swimmer::CreateDefaultGenome()
{
    return {
        std::make_shared<GenePigment>(),
        std::make_shared<GenePigment>(),
        std::make_shared<GeneBrain>(3, NeuralNetwork::BRAIN_WIDTH, 0.5),
        std::make_shared<GeneSenseRandom>(1, NeuralNetwork::BRAIN_WIDTH),
        std::make_shared<GeneSenseMagneticField>(NeuralNetwork::BRAIN_WIDTH),
        std::make_shared<GeneSenseTraitsInArea>(std::vector{
                                                    SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Distance),
                                                    SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Green), },
                                                0, NeuralNetwork::BRAIN_WIDTH, Transform{ 0, 24,  0.6}, 20),
        std::make_shared<GeneSenseTraitsInArea>(std::vector{
                                                    SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Distance),
                                                    SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Green), },
                                                0, NeuralNetwork::BRAIN_WIDTH, Transform{ 0, 24, -0.6}, 20),
        std::make_shared<GeneSenseTraitsRaycast>(std::vector{
                                                     SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Distance),
                                                     SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Size), },
                                                 0, NeuralNetwork::BRAIN_WIDTH, Transform{ 0, -30, 0 }),
        std::make_shared<GeneSenseTraitsTouching>(std::vector{
                                                      SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Age), },
                                                  0, NeuralNetwork::BRAIN_WIDTH, Transform{ 0, 0, 0 }),
        std::make_shared<GeneSenseTraitsSelf>(std::vector{
                                                  SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Age),
                                                  SenseTraitsBase::DefaultNormalisation(SenseTraitsBase::Trait::Energy), },
                                              0, NeuralNetwork::BRAIN_WIDTH),
        std::make_shared<GeneEffectorTail>(0, NeuralNetwork::BRAIN_WIDTH),
    };
}
