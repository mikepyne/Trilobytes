#ifndef GENESENSETRAITSBASE_H
#define GENESENSETRAITSBASE_H

#include "GeneSenseBase.h"
#include "SenseTraitsBase.h"

#include <vector>
#include <memory>

class GeneSenseTraitsBase : public GeneSenseBase {
public:
    static std::shared_ptr<Gene> Generate(unsigned brainWidth);
    static std::shared_ptr<Gene> Deserialise(const nlohmann::json& serialised);

    GeneSenseTraitsBase(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform);
    GeneSenseTraitsBase(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance);
    virtual ~GeneSenseTraitsBase() override {}

protected:
    static const inline std::string KEY_TRAIT_NORMALISERS;
    static const inline std::string KEY_TRANSFORM;

    std::vector<SenseTraitsBase::TraitNormaliser> toDetect_;
    Transform transform_;

    nlohmann::json GetSerialisedTraitNormalisers() const;
};

#endif // GENESENSETRAITSBASE_H
