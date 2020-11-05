#ifndef GENESENSETRAITSBASE_H
#define GENESENSETRAITSBASE_H

#include "GeneSenseBase.h"
#include "Sensors/SenseTraitsBase.h"

#include <vector>
#include <memory>

class GeneSenseTraitsBase : public GeneSenseBase {
public:
    GeneSenseTraitsBase(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform);
    GeneSenseTraitsBase(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance, double mutationProbability);
    virtual ~GeneSenseTraitsBase() override {}

protected:
    std::vector<SenseTraitsBase::TraitNormaliser> toDetect_;
    Transform transform_;
};

#endif // GENESENSETRAITSBASE_H
