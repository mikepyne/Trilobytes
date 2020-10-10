#ifndef SENSETRAITSSELF_H
#define SENSETRAITSSELF_H

#include "SenseTraitsBase.h"

class SenseTraitsSelf : public SenseTraitsBase {
public:
    SenseTraitsSelf(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, std::vector<TraitNormaliser>&& toDetect);
    virtual ~SenseTraitsSelf() override;

    virtual std::string_view GetName() const override { return "TraitsSelf"; }

private:
    virtual void FilterEntities(const EntityContainerInterface& /*entities*/, const std::function<void (const Entity&, const double&)>& forEachEntity) const override;
};

#endif // SENSETRAITSSELF_H
