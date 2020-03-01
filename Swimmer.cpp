#include "Swimmer.h"
#include "FoodPellet.h"
#include "Utils.h"

#include <cmath>

Swimmer::Swimmer(double x, double y)
    : Swimmer(x, y, NeuralNetwork(3, 4))
{
}

Swimmer::Swimmer(double x, double y, NeuralNetwork&& brain)
    : Entity(x, y, 6.0)
    , brain_(std::move(brain))
    , taste_(*this, 0.0, 0.0, radius_, SenseEntityPresence::MakeCustomFilter<FoodPellet>(1.0))
    , compass_(*this)
    , rand_(*this, 1)
{
    speed_ = 0.5;
    energy_ = 1.0;
}

bool Swimmer::Tick(EntityContainerInterface& container)
{
    // TODO bearing should JUMP at Pi to -Pi (not Tau to 0), also makes maths and normalising easier
    auto taste = taste_.Tick(container, {});
    auto compass = compass_.Tick(container, {});
    auto rand = rand_.Tick(container, {});

    auto& out = brain_.ForwardPropogate({ taste[0], compass[0], compass[1], rand[0] });
    bearing_ += (out[1]);
    if (bearing_ < 0.0) {
        bearing_ += EoBE::Tau;
    } else if (bearing_ > EoBE::Tau) {
        bearing_ -= EoBE::Tau;
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
    compass_.Draw(paint);
}

std::shared_ptr<Swimmer> Swimmer::GiveBirth()
{
    return std::make_shared<Swimmer>(GetX(), GetY(), brain_.Mutated());
}

