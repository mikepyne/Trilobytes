#ifndef EFFECTORTAIL_H
#define EFFECTORTAIL_H

#include "Effector.h"

class EffectorTail : public Effector {
public:
    EffectorTail(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, Swimmer& owner);
    virtual ~EffectorTail() override final;

    virtual std::string_view GetName() const override { return "Tail"; }
    virtual void Draw(QPainter& paint) const override;

private:
    virtual void PerformActions(const std::vector<double>& actionValues, const EntityContainerInterface& entities) override;
};

#endif // EFFECTORTAIL_H
