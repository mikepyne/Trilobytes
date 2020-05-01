#include "Swimmer.h"
#include "Random.h"
#include "FoodPellet.h"
#include "Utils.h"
#include "Egg.h"

#include <QPainter>

#include <math.h>

Swimmer::Swimmer(EnergyPool&& energy, double x, double y)
    : Swimmer(std::move(energy), x, y, NeuralNetwork(3, 7, NeuralNetwork::InitialWeights::Random), std::make_shared<Genome>(CreateDefaultGenome()))
{
}

Swimmer::Swimmer(EnergyPool&& energy, double x, double y, NeuralNetwork&& brain, std::shared_ptr<Genome> genome)
    : Entity(std::move(energy), x, y, 6.0, QColor::fromRgb(15, 15, 235))
    , genome_(std::move(genome))
    , brain_(std::move(brain))
    , taste_(*this, brain_.GetInputCount(), 0.0, 0.0, 1.0, { {1.0, Trait::Green}, })
    , leftAntenna_(*this, brain_.GetInputCount(), GetRadius() * 5, GetRadius() * 3.5, -0.6, 1.0, { {1.0, Trait::Green}, })
    , rightAntenna_(*this, brain_.GetInputCount(), GetRadius() * 5, GetRadius() * 3.5, 0.6, 1.0, { {1.0, Trait::Green}, })
    , echoLocator_(*this, brain_.GetInputCount(), GetRadius() * 2, 0.0, {})
    , compass_(*this, brain_.GetInputCount())
    , rand_(*this, 1, brain_.GetInputCount())
{
    SetSpeed(0.5);
    ApplyGenome();
}

Swimmer::~Swimmer()
{
}

std::shared_ptr<Entity> Swimmer::GiveBirth()
{
    return std::make_shared<Egg>(TakeEnergy(100_mj), GetX(), GetY(), brain_.Mutated(), genome_, Random::Poisson(50u));
}

void Swimmer::TickImpl(EntityContainerInterface& container)
{
    // TODO bearing should JUMP at Pi to -Pi (not Tau to 0), also makes maths and normalising easier
    std::vector<double> sensoryOutput(brain_.GetInputCount(), 0.0);
    taste_.Tick(sensoryOutput, container, {});
    leftAntenna_.Tick(sensoryOutput, container, {});
    rightAntenna_.Tick(sensoryOutput, container, {});
    echoLocator_.Tick(sensoryOutput, container, {});
    compass_.Tick(sensoryOutput, container, {});
    rand_.Tick(sensoryOutput, container, {});

    brain_.ForwardPropogate(sensoryOutput);
    double newBearing = GetBearing();
    newBearing += (sensoryOutput[1]);
    if (newBearing < 0.0) {
        newBearing += EoBE::Tau;
    } else if (newBearing > EoBE::Tau) {
        newBearing -= EoBE::Tau;
    }
    SetBearing(newBearing);

    UseEnergy(200_uj); // TODO remove this, entities will use energy based on what they are doing (well maybe a small base usage would deter couch potatoes...)

    container.ForEachCollidingWith(Circle{ GetX(), GetY(), GetRadius() }, [&](Entity& other) -> void
    {
        if (FoodPellet* f = dynamic_cast<FoodPellet*>(&other)) {
            FeedOn(*f, f->GetEnergy());
        }
    });

    if (GetEnergy() > 300_mj) {
        container.AddEntity(GiveBirth());
    }
}

void Swimmer::DrawImpl(QPainter& paint)
{
    paint.save();
    paint.drawEllipse({GetX(), GetY()}, GetRadius(), GetRadius());
    paint.setBrush(tempPigments_);
    paint.drawEllipse({GetX(), GetY()}, GetRadius(), GetRadius());
    paint.restore();
    paint.save();
    paint.setPen(QColor(255, 255, 255));
    echoLocator_.Draw(paint);
    paint.restore();
    paint.save();
    compass_.Draw(paint);
    paint.restore();
    paint.save();
    paint.setPen(QColor(0, 0, 0));
    paint.setBrush(QColor(0, 0, 0, 0));
    leftAntenna_.Draw(paint);
    rightAntenna_.Draw(paint);
    paint.restore();
}

std::vector<std::shared_ptr<Gene> > Swimmer::CreateDefaultGenome()
{
    return {
        std::make_shared<GenePigment>(),
        std::make_shared<GenePigment>(),
        std::make_shared<GenePigment>(),
        std::make_shared<GenePigment>(),
    };
}

void Swimmer::ApplyGenome()
{
    double a, r, g, b;
    genome_->ForEach([&](const Gene& gene)
    {
        if (auto* pigment = dynamic_cast<const GenePigment*>(&gene)) {
            a += pigment->a_;
            r += pigment->r_;
            g += pigment->g_;
            b += pigment->b_;
        }
    });

    tempPigments_.setRgbF(std::clamp(r, 0.0, 1.0),
                          std::clamp(g, 0.0, 1.0),
                          std::clamp(b, 0.0, 1.0),
                          std::clamp(a, 0.0, 1.0));
}
