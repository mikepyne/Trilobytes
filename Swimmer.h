#ifndef SWIMMER_H
#define SWIMMER_H

#include "Entity.h"
#include "NeuralNetwork.h"
#include "Genome/Genome.h"
#include "Sensors/Sense.h"
#include "Effectors/Effector.h"

#include <memory>

class Swimmer : public Entity, public std::enable_shared_from_this<Swimmer> {
public:
    Swimmer(Energy energy, const Transform& transform);
    Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome, std::shared_ptr<Swimmer>&& parent);
    virtual ~Swimmer() override;

    virtual std::string_view GetName() const override { return "Swimmer"; }

    std::shared_ptr<Entity> GiveBirth(const std::shared_ptr<Genome>& other);

    const std::shared_ptr<NeuralNetwork>& InspectBrain() { return brain_; };
    const std::vector<std::shared_ptr<Sense>>& InspectSenses() { return senses_; };
    const std::vector<std::shared_ptr<Effector>>& InspectEffectors() { return effectors_; };

    unsigned GetEggsLayedCount() const { return eggsLayed_; }
    unsigned GetTotalDescendantsCount() const { return totalDescendantCount_; }
    unsigned GetLivingDescendantsCount() const { return extantDescendantCount_; }

    void AdjustVelocity(double adjustment);
    void AdjustBearing(double adjustment);

protected:
    std::shared_ptr<Swimmer> closestLivingAncestor_;

    virtual void TickImpl(EntityContainerInterface& container) override final;
    virtual void DrawImpl(QPainter& paint) override final;

private:
    std::shared_ptr<Genome> genome_;
    std::shared_ptr<NeuralNetwork> brain_;
    std::vector<std::shared_ptr<Sense>> senses_;
    std::vector<std::shared_ptr<Effector>> effectors_;
    std::vector<double> brainValues_;

    unsigned eggsLayed_;
    unsigned totalDescendantCount_;
    unsigned extantDescendantCount_;

    Swimmer(Energy energy, const Transform& transform, std::shared_ptr<Genome> genome, const Phenotype& phenotype, std::shared_ptr<Swimmer>&& mother);

    std::shared_ptr<Swimmer> FindClosestLivingAncestor() const;

    void DescendantBorn();
    void DescendantDied();

    static std::vector<std::shared_ptr<Gene>> CreateDefaultGenome();

};

#endif // SWIMMER_H
