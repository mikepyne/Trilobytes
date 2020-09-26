#ifndef SENSE_H
#define SENSE_H

class EntityContainerInterface;
class QPainter;

#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"

#include <string_view>
#include <memory>

class Swimmer;

/**
 * All senses contain a small Neural network which is propogated each tick.
 */
class Sense {
protected:
    struct UniverseInfoStructRef {
        // Placeholder till a real one exists
    };
public:
    /**
     * Instantiates with a random neural network
     */
    Sense(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner);
    virtual ~Sense();

    virtual void Draw(QPainter& paint) const;
    virtual void Tick(std::vector<double>& outputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) final;

    virtual std::string_view GetName() const = 0;
    unsigned GetOutputCount() const { return network_->GetOutputCount(); }

    const NeuralNetwork& Inspect() const { return *network_; }
    const NeuralNetworkConnector& InspectConnection() const { return *outputConnections_; }

protected:
    const Swimmer& owner_;

private:
    std::shared_ptr<NeuralNetwork> network_;
    std::shared_ptr<NeuralNetworkConnector> outputConnections_;
    std::vector<double> inputs_;

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) = 0;
};

#endif // SENSE_H
