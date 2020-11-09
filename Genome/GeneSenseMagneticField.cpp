#include "GeneSenseMagneticField.h"

#include "Phenotype.h"
#include "Sensors/SenseMagneticField.h"

GeneSenseMagneticField::GeneSenseMagneticField(unsigned hiddenLayers, unsigned outputCount)
    : GeneSenseMagneticField(std::make_shared<NeuralNetwork>(hiddenLayers, 3, NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(3, outputCount), Point{0, 0}, EoBE::RangeConverter({0, 1000}, {0, 0.3}), {-0.1, 0.1}, {-0.1, 0.1}, Random::Number(0.0, 100.0), Random::Number(0.0, 1.0))
{
}

GeneSenseMagneticField::GeneSenseMagneticField(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Point& target, const EoBE::RangeConverter& distanceNormaliser, const EoBE::Range<double>& frontBackNormaliser, const EoBE::Range<double>& leftRightNormaliser, double dominance, double mutationProbability)
    : GeneSenseBase(network, outputConnections, dominance, mutationProbability)
    , target_(target)
    , distanceNormaliser_(distanceNormaliser)
    , frontBackNormaliser_(frontBackNormaliser)
    , leftRightNormaliser_(leftRightNormaliser)
{
    // Move target
    AddMutation(BASE_WEIGHT, [&]()
    {
        target_ = { Random::Gaussian(target.x, 5.0), Random::Gaussian(target.y, 5.0) };
    });

    // Move target a lot
    AddMutation(0.05 * BASE_WEIGHT, [&]()
    {
        target_ = { Random::Gaussian(target.x, 100.0), Random::Gaussian(target.y, 100.0) };
    });

    // Adjust range within which the sence can discern differences in distance
    AddMutation(BASE_WEIGHT, [&]()
    {
        EoBE::Range<double> from = distanceNormaliser_.GetFrom();
        from = { Random::Gaussian(from.First(), 5.0), Random::Gaussian(from.Last(), 5.0) };
        distanceNormaliser_ = { from, distanceNormaliser_.GetTo() };
    });

    // Adjust neuron activation based on range
    AddMutation(BASE_WEIGHT, [&]()
    {
        EoBE::Range<double> to = distanceNormaliser_.GetTo();
        to = { std::clamp(Random::Gaussian(to.First(), 0.05), -1.0, 1.0), std::clamp(Random::Gaussian(to.Last(), 0.05), -1.0, 1.0) };
        distanceNormaliser_ = { distanceNormaliser_.GetFrom(), to };
    });

    // Big adjustment range within which the sence can discern differences in distance
    AddMutation(0.05 * BASE_WEIGHT, [&]()
    {
        EoBE::Range<double> from = distanceNormaliser_.GetFrom();
        from = { Random::Gaussian(from.First(), 100.0), Random::Gaussian(from.Last(), 100.0) };
        distanceNormaliser_ = { from, distanceNormaliser_.GetTo() };
    });

    // Adjust neuron activation based on front/back orientation
    AddMutation(BASE_WEIGHT, [&]()
    {
        EoBE::Range<double> to = frontBackNormaliser_;
        to = { std::clamp(Random::Gaussian(to.First(), 0.05), -1.0, 1.0), std::clamp(Random::Gaussian(to.Last(), 0.05), -1.0, 1.0) };
        frontBackNormaliser_ = to;
    });

    // Adjust neuron activation based on left/right orientation
    AddMutation(BASE_WEIGHT, [&]()
    {
        EoBE::Range<double> to = leftRightNormaliser_;
        to = { std::clamp(Random::Gaussian(to.First(), 0.05), -1.0, 1.0), std::clamp(Random::Gaussian(to.Last(), 0.05), -1.0, 1.0) };
        leftRightNormaliser_ = to;
    });
}

void GeneSenseMagneticField::ExpressGene(Swimmer& owner, Phenotype& target) const
{
    target.senses.push_back(std::make_shared<SenseMagneticField>(GetNetwork(), GetOutputConnections(), owner, target_, distanceNormaliser_, frontBackNormaliser_, leftRightNormaliser_));
}

std::shared_ptr<Gene> GeneSenseMagneticField::Copy() const
{
    return std::make_shared<GeneSenseMagneticField>(GetNetwork(), GetOutputConnections(), target_, distanceNormaliser_, frontBackNormaliser_, leftRightNormaliser_, GetDominance(), GetMutationProbability());
}
