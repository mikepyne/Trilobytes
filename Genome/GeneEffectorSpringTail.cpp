#include "GeneEffectorSpringTail.h"

#include "Swimmer.h"
#include "Effectors/EffectorSpringTail.h"

GeneEffectorSpringTail::GeneEffectorSpringTail(unsigned hiddenLayers, unsigned inputCount, const Tril::RangeConverter& neuronExcitementToChargeRate, const Energy& storedEnergyCap, const double& triggerThreshold)
    : GeneEffectorBase(hiddenLayers, inputCount, 2)
    , neuronExcitementToChargeRate_(neuronExcitementToChargeRate)
    , storedEnergyCap_(storedEnergyCap)
    , triggerThreshold_(triggerThreshold)
{
    AddMutations();
}

GeneEffectorSpringTail::GeneEffectorSpringTail(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance, const Tril::RangeConverter& neuronExcitementToChargeRate, const Energy& storedEnergyCap, const double& triggerThreshold)
    : GeneEffectorBase(network, inputConnections, dominance)
    , neuronExcitementToChargeRate_(neuronExcitementToChargeRate)
    , storedEnergyCap_(storedEnergyCap)
    , triggerThreshold_(triggerThreshold)
{
    AddMutations();
}

GeneEffectorSpringTail::~GeneEffectorSpringTail()
{
}

void GeneEffectorSpringTail::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.effectors.push_back(std::make_shared<EffectorSpringTail>(GetNetwork(), GetInputConnections(), owner, neuronExcitementToChargeRate_, storedEnergyCap_, triggerThreshold_));
    target.baseMetabolism += GetMetabolicCost();
}

std::shared_ptr<Gene> GeneEffectorSpringTail::Copy() const
{
    return std::make_shared<GeneEffectorSpringTail>(GetNetwork(), GetInputConnections(), GetDominance(), neuronExcitementToChargeRate_, storedEnergyCap_, triggerThreshold_);
}

void GeneEffectorSpringTail::AddMutations()
{
    // Adjust charge rate activation range
    AddMutation(BASE_WEIGHT, [&]()
    {
        double newMin = Random::GaussianAdjustment<double>(neuronExcitementToChargeRate_.GetFrom().Min(), 0.1);
        double newMax = Random::GaussianAdjustment<double>(neuronExcitementToChargeRate_.GetFrom().Max(), 0.1);
        neuronExcitementToChargeRate_ = { Tril::Range(newMin, newMax), neuronExcitementToChargeRate_.GetTo() };
    });

    // Adjust charge rate output range
    AddMutation(BASE_WEIGHT, [&]()
    {
        Energy newMin = Random::GaussianAdjustment<Energy>(neuronExcitementToChargeRate_.GetTo().Min(), 0.3);
        Energy newMax = Random::GaussianAdjustment<Energy>(neuronExcitementToChargeRate_.GetTo().Max(), 0.3);
        neuronExcitementToChargeRate_ = { neuronExcitementToChargeRate_.GetFrom(), Tril::Range(newMin, newMax) };
    });

    // Adjust stored Energy Cap
    AddMutation(BASE_WEIGHT, [&]()
    {
        storedEnergyCap_ = Random::GaussianAdjustment(storedEnergyCap_, 0.3);
    });

    // Randomise trigger threshold
    AddMutation(BASE_WEIGHT, [&]()
    {
        triggerThreshold_ = Random::Number(-1.0, 1.0);
    });
}
