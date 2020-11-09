#ifndef GENESENSEMAGNETICFIELD_H
#define GENESENSEMAGNETICFIELD_H

#include "GeneSenseBase.h"
#include "Shape.h"
#include "Utils.h"

class GeneSenseMagneticField : public GeneSenseBase {
public:
    GeneSenseMagneticField(unsigned hiddenLayers, unsigned outputCount);
    GeneSenseMagneticField(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Point& target, const EoBE::RangeConverter& distanceNormaliser, const EoBE::Range<double>& frontBackNormaliser, const EoBE::Range<double>& leftRightNormaliser, double dominance, double mutationProbability);
    virtual ~GeneSenseMagneticField() override {}

    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;

private:
    Point target_;
    EoBE::RangeConverter distanceNormaliser_;
    EoBE::Range<double> frontBackNormaliser_;
    EoBE::Range<double> leftRightNormaliser_;
};

#endif // GENESENSEMAGNETICFIELD_H
