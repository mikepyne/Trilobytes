#include "GeneEffectorSpike.h"

#include "Swimmer.h"
#include "Effectors/EffectorSpike.h"



GeneEffectorSpike::GeneEffectorSpike(unsigned inputCount, double bearing, double length)
    : GeneEffectorBase(0, inputCount, 0)
    , bearing_(bearing)
    , length_(length)
{
    AddMutations();
}

GeneEffectorSpike::GeneEffectorSpike(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, double dominance, double bearing, double length)
    : GeneEffectorBase(network, inputConnections, dominance)
    , bearing_(bearing)
    , length_(length)
{
    AddMutations();
}

GeneEffectorSpike::~GeneEffectorSpike()
{
}

void GeneEffectorSpike::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.effectors.push_back(std::make_shared<EffectorSpike>(GetNetwork(), GetInputConnections(), owner, bearing_, length_));
    target.baseMetabolism += GetMetabolicCost();
    // TODO Spikes neeed to cost some energy to grow initially, perhaps use some energy here based on spike length?
}

std::shared_ptr<Gene> GeneEffectorSpike::Copy() const
{
    return std::make_shared<GeneEffectorSpike>(GetNetwork(), GetInputConnections(), GetDominance(), bearing_, length_);
}

void GeneEffectorSpike::AddMutations()
{
    // Adjust bearing
    AddMutation(BASE_WEIGHT, [&]()
    {
        bearing_ = std::abs(std::fmod(Random::Gaussian(bearing_, EoBE::Tau / 25), EoBE::Tau));
    });
    // Adjust length
    AddMutation(BASE_WEIGHT, [&]()
    {
        length_ = Random::Gaussian(length_, 0.5);
    });
}
