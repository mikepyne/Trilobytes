#include "Swimmer.h"
#include "FoodPellet.h"

#include <cmath>

Swimmer::Swimmer(double x, double y)
    : Swimmer(x, y, NeuralNetwork(3, 3))
{
}

Swimmer::Swimmer(double x, double y, NeuralNetwork&& brain)
    : Entity(x, y, 6.0)
    , brain_(std::move(brain))
{
    speed_ = 0.5;
    energy_ = 1.0;
}

bool Swimmer::Tick(EntityContainerInterface& container)
{
    auto& out = brain_.ForwardPropogate({Random::Real(-2.0, 2.0), energy_ - 2.0, (bearing_ / 3.14) - 1});
    bearing_ += (out[1]);
    if (bearing_ < 0.0) {
        bearing_ += 6.28;
    } else if (bearing_ > 6.28) {
        bearing_ -= 6.28;
    }

    double energyLoss = 1.0 / 1000;
    if (energy_ > energyLoss) {
        energy_ -= energyLoss;
    } else {
        energy_ = 0;
    }

    container.ForEachIn(Circle{ GetX(), GetY(), radius_ }, [&](Entity& other) -> void
    {
        if (FoodPellet* f = dynamic_cast<FoodPellet*>(&other)) {
            energy_ += f->Eat();
        }
    });

    if (energy_ >= 3.0) {
        energy_ -= 1;
        container.AddEntity(GiveBirth());
    }

    return Move();
}

void Swimmer::Draw(QPainter& paint)
{
    paint.setBrush(QColor(15, 15, 235));
    paint.drawEllipse({GetX(), GetY()}, radius_, radius_);
}

std::shared_ptr<Swimmer> Swimmer::GiveBirth()
{
    return std::make_shared<Swimmer>(GetX(), GetY(), brain_.Mutated());
}

