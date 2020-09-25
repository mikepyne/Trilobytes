#ifndef SENSE_H
#define SENSE_H

class EntityContainerInterface;
class QPainter;

#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"

#include <string_view>

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
    Sense(Swimmer& owner, unsigned inputs, unsigned hiddenLayers);
    virtual ~Sense();

    virtual void Draw(QPainter& paint) const;
    virtual void Tick(std::vector<double>& outputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) final;

    virtual std::string_view GetName() const = 0;
    unsigned GetOutputCount() const { return network_.GetOutputCount(); }

    const NeuralNetwork& Inspect() const { return network_; }
    const NeuralNetworkConnector& InspectConnection() const { return senseToBrainConnector_; }

protected:
    Swimmer& owner_;

private:
    NeuralNetwork network_;
    NeuralNetworkConnector senseToBrainConnector_;
    std::vector<double> inputs_;

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) = 0;
};

#endif // SENSE_H
