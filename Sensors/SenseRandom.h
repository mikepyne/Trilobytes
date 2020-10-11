#ifndef SENSERANDOM_H
#define SENSERANDOM_H

#include "Sense.h"

class SenseRandom : public Sense {
public:
    struct FilteredRandom {
        double min_;
        double max_;
        double alpha_; // i.e. Low pass filter
    };

    SenseRandom(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, std::vector<FilteredRandom>&& filteredRandoms);
    virtual ~SenseRandom() override {}

    virtual std::string_view GetName() const override { return "Random"; }
    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities) const override;

private:
    const std::vector<FilteredRandom> filteredRandoms_;
    std::vector<double> last_;

    virtual void PrepareToPrime() override;
};

#endif // SENSERANDOM_H
