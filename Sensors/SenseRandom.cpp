#include "SenseRandom.h"

#include "Random.h"

SenseRandom::SenseRandom(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, std::vector<FilteredRandom>&& filteredRandoms)
    : Sense(network, outputConnections, owner)
    , filteredRandoms_(std::move(filteredRandoms))
    , last_(filteredRandoms_.size(), 0.0)
{
}

void SenseRandom::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& /*entities*/) const
{
    EoBE::IterateBoth<double, double>(inputs, last_, [](double& input, const double& last)
    {
        input = last;
    });
}

void SenseRandom::PrepareToPrime()
{
    EoBE::IterateBoth<double, FilteredRandom>(last_, filteredRandoms_, [](double& previous, const FilteredRandom& filter) {
        previous = std::clamp(previous + (filter.beta_ * (Random::Number(filter.min_, filter.max_) - previous)), -1.0, 1.0);
    });
}
