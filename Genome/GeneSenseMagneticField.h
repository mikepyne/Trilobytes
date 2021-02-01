#ifndef GENESENSEMAGNETICFIELD_H
#define GENESENSEMAGNETICFIELD_H

#include "GeneSenseBase.h"
#include "Shape.h"
#include "Utils.h"

class GeneSenseMagneticField : public GeneSenseBase {
public:
    GeneSenseMagneticField(unsigned hiddenLayers, unsigned outputCount, const Point& point);
    GeneSenseMagneticField(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Point& target, const Tril::RangeConverter& distanceNormaliser, const Tril::Range<double>& frontBackNormaliser, const Tril::Range<double>& leftRightNormaliser, double dominance);
    virtual ~GeneSenseMagneticField() override {}

    virtual void ExpressGene(Swimmer& owner, Phenotype& target) const override;

protected:
    virtual std::shared_ptr<Gene> Copy() const override;

private:
    Point target_;
    Tril::RangeConverter distanceNormaliser_;
    Tril::Range<double> frontBackNormaliser_;
    Tril::Range<double> leftRightNormaliser_;
};

#endif // GENESENSEMAGNETICFIELD_H
