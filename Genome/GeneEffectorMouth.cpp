#include "GeneEffectorMouth.h"

#include "Swimmer.h"
#include "Phenotype.h"
#include "Effectors/EffectorMouth.h"

GeneEffectorMouth::GeneEffectorMouth(unsigned hiddenLayers, unsigned inputCount, Transform transform)
    : GeneEffectorBase(hiddenLayers, inputCount, 1)
    , transform_(transform)
{
}

GeneEffectorMouth::GeneEffectorMouth(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, Transform transform, double dominance, double mutationProbability)
    : GeneEffectorBase(network, inputConnections, dominance, mutationProbability)
    , transform_(transform)
{
}

std::shared_ptr<Gene> GeneEffectorMouth::Mutate() const
{
    Transform transform = transform_;
    auto [ network, inputs, dominance, mutationProbability ] = GetMutated({[&]()
        {
            transform = {
                Random::GaussianAdjustment<double>(transform.x, 0.1),
                Random::GaussianAdjustment<double>(transform.y, 0.1),
                Random::GaussianAdjustment<double>(transform.rotation, 0.1),
            };
        }});
    return std::make_shared<GeneEffectorMouth>(network, inputs, transform, dominance, mutationProbability);
}

void GeneEffectorMouth::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.effectors.push_back(std::make_shared<EffectorMouth>(network_, inputs_, owner, transform_));
}
