#include "Swimmer.h"

#include "Random.h"
#include "FoodPellet.h"
#include "MeatChunk.h"
#include "Utils.h"
#include "Egg.h"
#include "Genome/GeneFactory.h"

#include <QPainter>

#include <math.h>

Swimmer::Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome)
    : Swimmer(energy, transform, genome, {})
{
}

Swimmer::Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome, std::shared_ptr<Swimmer>&& parent)
    : Swimmer(energy, transform, genome, genome->GetPhenoType(*this), std::move(parent))
{
}

Swimmer::~Swimmer()
{
    if (closestLivingAncestor_) {
        closestLivingAncestor_->DescendantDied(generation_);
    }
}

std::shared_ptr<Entity> Swimmer::GiveBirth(const std::shared_ptr<Genome>& other)
{
    ++eggsLayed_;
    return std::make_shared<Egg>(shared_from_this(), TakeEnergy(100_mj), GetTransform(), genome_, other ? other : genome_, Random::Poisson(50u));
}

unsigned Swimmer::GetTotalDescendantsCount() const
{
    return std::accumulate(std::cbegin(totalDescentantCounts_), std::cend(totalDescentantCounts_), 0u, [](unsigned total, const auto& pair)
    {
        return total + pair.second;
    });
}

unsigned Swimmer::GetLivingDescendantsCount() const
{
    return std::accumulate(std::cbegin(extantDescentantCounts_), std::cend(extantDescentantCounts_), 0u, [](unsigned total, const auto& pair)
    {
        return total + pair.second;
    });
}

void Swimmer::AdjustVelocity(double adjustment)
{
    SetVelocity(GetVelocity() + adjustment);
}

void Swimmer::AdjustBearing(double adjustment)
{
    SetBearing(GetTransform().rotation + adjustment);
}

void Swimmer::TickImpl(EntityContainerInterface& container)
{
    if (closestLivingAncestor_ && !closestLivingAncestor_->Alive()) {
        closestLivingAncestor_ = FindClosestLivingAncestor();
    }

    if (health_ <= 0.0) {
        // explode into some chunks of meat
        const Energy swimmerEnergy = GetEnergy();
        const double swimmerSpeed = GetVelocity();
        const Transform swimmerTransform = GetTransform();

        const int chunks = swimmerEnergy / 40_mj;
        // make sure we explode evenly outwards, and not all in one direction
        const double rotationOffset = Random::Bearing();
        const double rotationStep = EoBE::Tau / chunks;
        for (int i = 0; i < chunks; i++) {
            Vec2 swimmerMovement = GetMovementVector(swimmerTransform.rotation, swimmerSpeed);
            Vec2 relativeMovement = GetMovementVector(rotationOffset + (i * rotationStep) + Random::Gaussian(0.0, EoBE::Tau / 10), + Random::Gaussian(10.0, 2.5));
            Vec2 chunkMovement = { swimmerMovement.x + relativeMovement.x, swimmerMovement.y + relativeMovement.y };
            auto [ rotation, speed ] = DeconstructMovementVector(chunkMovement);{}
            Transform chunkTransform = { swimmerTransform.x + (chunkMovement.x / 10.0), swimmerTransform.y + (chunkMovement.y / 10.0), rotation };
            container.AddEntity(std::make_shared<MeatChunk>(40_mj, chunkTransform, speed));
        }
        Terminate();
    } else {
        Energy energyUsed = 0_j;
        if (brain_ && brain_->GetInputCount() > 0) {
            std::fill(std::begin(brainValues_), std::end(brainValues_), 0.0);
            for (auto& sense : senses_) {
                sense->Tick(brainValues_, container);
            }

            brain_->ForwardPropogate(brainValues_);

            for (auto& effector : effectors_) {
                energyUsed += effector->Tick(brainValues_, container);
            }
        }

        // TODO put a bunch of these parameters into genes
        if (health_ < 100.0) {
            // Heal up to 100.0 a max of 1.5 per tick
            double toHeal = std::min(0.05, 100.0 - health_);
            health_ += toHeal;
            energyUsed += toHeal * 10_uj;
        }

        UseEnergy(baseMetabolism_ + energyUsed);

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
}

void Swimmer::DrawImpl(QPainter& paint)
{
    paint.save();
    paint.setBrush(GetColour());
    paint.drawEllipse({ GetTransform().x, GetTransform().y }, GetRadius(), GetRadius());
    paint.restore();

    for (auto& sense : senses_) {
        paint.save();
        sense->Draw(paint);
        paint.restore();
    }
    for (auto& effector : effectors_) {
        paint.save();
        effector->Draw(paint);
        paint.restore();
    }
}

Swimmer::Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome, const Phenotype& phenotype, std::shared_ptr<Swimmer>&& mother)
    : Entity(energy, transform, 6.0, phenotype.colour)
    , closestLivingAncestor_(std::move(mother))
    , generation_(closestLivingAncestor_ ? closestLivingAncestor_->generation_ + 1 : 0)
    , baseMetabolism_(phenotype.baseMetabolism)
    , health_(100.0)
    , genome_(genome)
    , brain_(phenotype.brain)
    , senses_(phenotype.senses)
    , effectors_(phenotype.effectors)
    , brainValues_(brain_->GetInputCount(), 0.0)
    , eggsLayed_(0)
{
    if (closestLivingAncestor_) {
        closestLivingAncestor_->DescendantBorn(generation_);
    }
}

std::shared_ptr<Swimmer> Swimmer::FindClosestLivingAncestor() const
{
    std::shared_ptr<Swimmer> ancestor = closestLivingAncestor_;
    while (ancestor && !ancestor->Alive()) {
        ancestor = ancestor->closestLivingAncestor_;
    }
    return ancestor;
}

void Swimmer::DescendantBorn(unsigned generation)
{
    // record generations of descendants relative to this's generation
    ++totalDescentantCounts_[generation - generation_];
    ++extantDescentantCounts_[generation - generation_];
    if (closestLivingAncestor_) {
        closestLivingAncestor_->DescendantBorn(generation);
    }
}

void Swimmer::DescendantDied(unsigned generation)
{
    // record generations of descendants relative to this's generation
    --extantDescentantCounts_[generation - generation_];
    if (closestLivingAncestor_) {
        closestLivingAncestor_->DescendantDied(generation);
    }
}
