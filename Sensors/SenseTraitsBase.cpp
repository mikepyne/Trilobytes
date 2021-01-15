#include "SenseTraitsBase.h"

#include "Swimmer.h"

std::string SenseTraitsBase::ToString(SenseTraitsBase::Trait trait)
{
    switch (trait) {
    case Trait::Red : return "Red";
    case Trait::Green : return "Green";
    case Trait::Blue : return "Blue";
    case Trait::Energy : return "Energy";
    case Trait::Age : return "Age";
    case Trait::Size : return "Size";
    case Trait::Distance : return "Distance";
    case Trait::Health : return "Health";
    }
    assert(false && "SenseTraitsBase::ToString(Trait), invalid trait.");
    return "";
}

SenseTraitsBase::SenseTraitsBase(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, const Transform& transform, std::vector<TraitNormaliser>&& toDetect)
    : Sense(network, outputConnections, owner)
    , transform_(transform)
    , toDetect_(std::move(toDetect))
{
}

void SenseTraitsBase::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseParameters& /*universeParameters*/) const
{
    // First adjust inputs for each entity detected
    FilterEntities(entities, [&](const Entity& entity)
    {
        EoBE::IterateBoth<TraitNormaliser, double>(toDetect_, inputs, [&](const TraitNormaliser& norm, double& input)
        {
            input += GetTraitFrom(entity, norm.trait);
        });
    });
    // Then normalise each input to a 0.0 -> 1.0 range
    EoBE::IterateBoth<TraitNormaliser, double>(toDetect_, inputs, [&](const TraitNormaliser& norm, double& input)
    {
        input = norm.range.ConvertAndClamp(input);
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
    case Trait::Distance : {
        Transform senseLocation = transform_ + owner_.GetTransform();
        return GetDistance({ senseLocation.x, senseLocation.y }, target.GetLocation());
    }
    case Trait::Health :
        if (const Swimmer* targetSwimmer = dynamic_cast<const Swimmer*>(&target)) {
            return targetSwimmer->GetHealth();
        } else {
            return 0.0;
        }
    }
    assert(false && "Entity::GetTrait, unimplemented trait.");
    return 0.0;
}
