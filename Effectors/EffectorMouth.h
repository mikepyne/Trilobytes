#ifndef EFFECTORMOUTH_H
#define EFFECTORMOUTH_H

#include "Effector.h"
#include "Transform.h"
#include "Utils.h"

/**
 * This effector is draws energy from a colliding target. The range is from the
 * total energy of the target to zero.
 *
 * The aim is to create a feeding apparatus that is capable of precise ant total
 * consumption of a target, with a high energy cost for usage, but zero base
 * energy use.
 */
class EffectorMouth : public Effector {
public:
    EffectorMouth(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& inputConnections, Swimmer& owner, const Transform& transform);
    ~EffectorMouth() override {}

    virtual std::string_view GetName() const override { return "Mouth"; }
    virtual void Draw(QPainter& paint) const override;

private:
    double radius_;
    Transform transform_;

    virtual void PerformActions(const std::vector<double>& actionValues, const EntityContainerInterface& entities) override;

    Circle GetGeometry() const;
};

#endif // EFFECTORMOUTH_H
