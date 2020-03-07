#ifndef SENSE_H
#define SENSE_H

class Entity;
class EntityContainerInterface;
class QPainter;

#include "NeuralNetwork.h"

#include <string_view>

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
    Sense(Entity& owner, unsigned inputs, unsigned hiddenLayers);
    virtual ~Sense();

    virtual void Draw(QPainter& paint) const;
    virtual const std::vector<double>& Tick(const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) final;

    virtual std::string_view GetName() const = 0;
    unsigned GetOutputCount() const { return network_.GetOutputCount(); }

protected:
    Entity& owner_;

private:
    NeuralNetwork network_;
    std::vector<double> inputs_;

    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseInfoStructRef& environment) = 0;
};

#endif // SENSE_H
