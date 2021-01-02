#include "GeneSenseTraitsBase.h"

#include "Random.h"

#include <functional>

GeneSenseTraitsBase::GeneSenseTraitsBase(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, unsigned hiddenLayers, unsigned outputCount, const Transform& transform)
    : GeneSenseTraitsBase(std::move(toDetect), std::make_shared<NeuralNetwork>(hiddenLayers, toDetect.size(), NeuralNetwork::InitialWeights::PassThrough), std::make_shared<NeuralNetworkConnector>(toDetect.size(), outputCount), transform, Random::Number(0.0, 100.0))
{
}

GeneSenseTraitsBase::GeneSenseTraitsBase(std::vector<SenseTraitsBase::TraitNormaliser>&& toDetect, const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Transform& transform, double dominance)
    : GeneSenseBase(network, outputConnections, dominance)
    , toDetect_(std::move(toDetect))
    , transform_(transform)
{
    // Modify transform
    AddMutation(BASE_WEIGHT, [&]() {
        transform_ = {
            Random::GaussianAdjustment<double>(transform_.x, 0.1),
            Random::GaussianAdjustment<double>(transform_.y, 0.1),
            Random::GaussianAdjustment<double>(transform_.rotation, 0.1),
        };
    });

    // Adjust trait strength
    AddMutation(BASE_WEIGHT, [&]()
    {
        if (!toDetect_.empty()) {
            auto& item = Random::Item(toDetect_);
            double newMin = Random::GaussianAdjustment<double>(item.range.Min(), 0.1);
            double newMax = Random::GaussianAdjustment<double>(item.range.Max(), 0.1);
            item.range = { newMin, newMax };
        }
    });

    // Switch trait
    AddMutation(BASE_WEIGHT, [&]()
    {
        if (!toDetect_.empty()) {
            auto& item = Random::Item(toDetect_);
            item.trait = Random::Item(SenseTraitsBase::ALL_TRAITS);
        }
    });

    // Add trait
    AddColumnInsertedMutation(0.15 * BASE_WEIGHT, [&](unsigned index)
    {
        auto iter = toDetect_.begin();
        std::advance(iter, index);
        toDetect_.insert(iter, SenseTraitsBase::DefaultNormalisation(Random::Item(SenseTraitsBase::ALL_TRAITS)));
    });

    // Remove trait
    AddColumnRemovedMutation(0.15 * BASE_WEIGHT, [&](unsigned index)
    {
        auto iter = toDetect_.begin();
        std::advance(iter, index);
        toDetect_.erase(iter);
    });
}
