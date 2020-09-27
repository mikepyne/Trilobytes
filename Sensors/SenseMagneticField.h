#ifndef SENSEMAGNETICFIELD_H
#define SENSEMAGNETICFIELD_H

#include "Sense.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"
#include "EntityContainerInterface.h"

#include <memory>

class SenseMagneticField : public Sense {
public:
    SenseMagneticField(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner);

    virtual std::string_view GetName() const override { return "MagneticField"; }
    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& /*entities*/, const UniverseInfoStructRef& /*environment*/) const override;

    virtual void Draw(QPainter& paint) const override;
};

#endif // SENSEMAGNETICFIELD_H
