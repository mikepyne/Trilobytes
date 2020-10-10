#ifndef GENESENSETRAITSBASE_H
#define GENESENSETRAITSBASE_H

#include "Gene.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"
#include "Sensors/SenseTraitsBase.h"

#include <vector>
#include <memory>

class GeneSenseTraitsBase : public Gene {
public:
    GeneSenseTraitsBase(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform);
    GeneSenseTraitsBase(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance, double mutationProbability);
    virtual ~GeneSenseTraitsBase() override {}

protected:
    std::vector<SenseTraitsBase::TraitNormaliser> toDetect_;
    std::shared_ptr<NeuralNetwork> network_;
    std::shared_ptr<NeuralNetworkConnector> outputs_;
    Transform transform_;

    std::tuple<std::vector<SenseTraitsBase::TraitNormaliser>, std::shared_ptr<NeuralNetwork>, std::shared_ptr<NeuralNetworkConnector>, Transform, double, double> GetMutated(std::vector<std::function<void()>> additionalMutations) const;
};

#endif // GENESENSETRAITSBASE_H
