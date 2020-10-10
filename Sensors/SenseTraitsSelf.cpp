#include "SenseTraitsSelf.h"

#include "Swimmer.h"

SenseTraitsSelf::SenseTraitsSelf(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect)
    : SenseTraitsBase(network, outputConnections, owner, {}, std::move(toDetect))
{
}

SenseTraitsSelf::~SenseTraitsSelf()
{
}

void SenseTraitsSelf::FilterEntities(const EntityContainerInterface&, const std::function<void (const Entity&, const double&)>& forEachEntity) const
{
    forEachEntity(owner_, 1.0);
}
