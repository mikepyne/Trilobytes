#ifndef SENSEENTITYRAYCAST_H
#define SENSEENTITYRAYCAST_H

#include "SenseTraitsBase.h"

class SenseEntityRaycast final : public SenseTraitsBase {
public:
    SenseEntityRaycast(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, std::vector<TraitNormaliser>&& toDetect, double maxDistance, double angle);
    virtual ~SenseEntityRaycast() override {}

    virtual std::string_view GetName() const override { return "SenseTraitsRaycast"; }
    virtual void Draw(QPainter& paint) const override;

private:
    double rayCastDistance_;
    Line GetLine() const;

    virtual void FilterEntities(const EntityContainerInterface& entities, const std::function<void (const Entity&, const double&)>& forEachEntity) const override;
};

#endif // SENSEENTITYRAYCAST_H
