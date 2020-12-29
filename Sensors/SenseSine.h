#ifndef SENSESINE_H
#define SENSESINE_H

#include "Sense.h"

class SenseSine : public Sense {
public:
    struct SineWave {
        double amplitude_;
        double frequency_;
        double x_ = 0.0;
    };

    SenseSine(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, std::vector<SineWave>&& sineWaves);

    virtual std::string_view GetName() const override { return "SineWaves"; }
    virtual void PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface& entities, const UniverseParameters& universeParameters) const override;

private:
    std::vector<SineWave> sineWaves_;

    virtual void PrepareToPrime() override;
};


#endif // SENSESINE_H
