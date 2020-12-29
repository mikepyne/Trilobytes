#ifndef SENSEMAGNETICFIELD_H
#define SENSEMAGNETICFIELD_H

#include "Sense.h"
#include "NeuralNetwork.h"
#include "NeuralNetworkConnector.h"
#include "EntityContainerInterface.h"

#include <memory>

/**
 * This sense gves a swimmer a way to gauge its distance from a specific point,
 * and also a sense of whether it is facing towards/away from it, and if it is
 * to the left or the right.
 *
 * The Neuron activation for each of these variables is customisable in the form
 * of the range converter member variables.
 */
class SenseMagneticField final : public Sense {
public:
    SenseMagneticField(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, const Point& target, const EoBE::RangeConverter& distanceNormaliser, const EoBE::Range<double>& frontBackNormaliser, const EoBE::Range<double>& leftRightNormaliser);
    virtual ~SenseMagneticField() override {}

    virtual std::string_view GetName() const override { return "MagneticField"; }
    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& /*entities*/, const UniverseParameters& universeParameters) const override;

    virtual void Draw(QPainter& paint) const override;

private:
    Point target_;
    EoBE::RangeConverter distanceNormaliser_;
    EoBE::RangeConverter frontBackNormaliser_;
    EoBE::RangeConverter leftRightNormaliser_;

    double BearingToFrontBack(double bearing) const;
    double BearingToLeftRight(double bearing) const;
};

#endif // SENSEMAGNETICFIELD_H
