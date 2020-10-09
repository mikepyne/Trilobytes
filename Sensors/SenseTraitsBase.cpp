#include "SenseTraitsBase.h"

#include "Swimmer.h"

SenseTraitsBase::SenseTraitsBase(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, const Transform& transform, std::vector<TraitNormaliser>&& toDetect)
    : Sense(network, outputConnections, owner)
    , transform_(transform)
    , toDetect_(std::move(toDetect))
{
}

void SenseTraitsBase::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities) const
{
    // First adjust inputs for each entity detected
    FilterEntities(entities, [&](const Entity& entity, const double& scale)
    {
        EoBE::IterateBoth<TraitNormaliser, double>(toDetect_, inputs, [&](const TraitNormaliser& norm, double& input)
        {
            /*
             * FIXME Scale is usually based on distance, so when detecting
             * Distance, the distance gets squared, which gives undesired values
             * so the value needs to be adjusted to fall between the
             * normalisation range...
             */
            if (norm.trait == Trait::Distance) {
                input += scale * (norm.range.ValueRange() - GetTraitFrom(entity, norm.trait));
            } else {
                input += scale * GetTraitFrom(entity, norm.trait);
            }
        });
    });
    // Then normalise each input to a 0.0 -> 1.0 range
    EoBE::IterateBoth<TraitNormaliser, double>(toDetect_, inputs, [&](const TraitNormaliser& norm, double& input)
    {
        double value = std::clamp(input, norm.range.Min(), norm.range.Max());
        input = (value - norm.range.Min()) / norm.range.ValueRange();
    });
}

double SenseTraitsBase::GetTraitFrom(const Entity& target, Trait trait) const
{
    switch (trait) {
    case Trait::Red :
        return target.GetColour().redF();
    case Trait::Green :
        return target.GetColour().greenF();
    case Trait::Blue :
        return target.GetColour().blueF();
    case Trait::Energy :
        return target.GetEnergy();
    case Trait::Age :
        return target.GetAge();
    case Trait::Size :
        return target.GetRadius();
    case Trait::Distance :
        Transform senseLocation = transform_ + owner_.GetTransform();
        return std::sqrt(GetDistanceSquare({ senseLocation.x, senseLocation.y }, target.GetLocation()));
    }
    assert(false && "Entity::GetTrait, unimplemented trait.");
    return 0.0;
}
