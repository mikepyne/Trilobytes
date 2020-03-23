#include "Swimmer.h"
#include "Random.h"
#include "FoodPellet.h"
#include "Utils.h"
#include "Egg.h"

#include <QPainter>

#include <math.h>

Swimmer::Swimmer(EnergyPool&& energy, double x, double y)
    : Swimmer(std::move(energy), x, y, NeuralNetwork(3, 6))
{
}

Swimmer::Swimmer(EnergyPool&& energy, double x, double y, NeuralNetwork&& brain)
    : Entity(std::move(energy), x, y, 6.0)
    , brain_(std::move(brain))
    , taste_(*this, 0.0, 0.0, radius_, SenseEntityPresence::MakeCustomFilter<FoodPellet>(0, { 1.0 }))
    , leftAntenna_(*this, -15.0, -20.0, radius_ * 5, SenseEntityDistance::MakeCustomFilter<FoodPellet>(0, { 1.0 }))
    , rightAntenna_(*this, 15.0, -20.0, radius_ * 5, SenseEntityDistance::MakeCustomFilter<FoodPellet>(0, { 1.0 }))
    , compass_(*this)
    , rand_(*this, 1)
{
    speed_ = 0.5;
}

Swimmer::~Swimmer()
{
}

bool Swimmer::Tick(EntityContainerInterface& container)
{
    // TODO bearing should JUMP at Pi to -Pi (not Tau to 0), also makes maths and normalising easier
    auto taste = taste_.Tick(container, {});
    auto leftSmell = leftAntenna_.Tick(container, {});
    auto rightSmell = rightAntenna_.Tick(container, {});
    auto compass = compass_.Tick(container, {});
    auto rand = rand_.Tick(container, {});

    auto& out = brain_.ForwardPropogate({ taste[0], leftSmell[0], rightSmell[0], compass[0], compass[1], rand[0] });
    bearing_ += (out[1]);
    if (bearing_ < 0.0) {
        bearing_ += EoBE::Tau;
    } else if (bearing_ > EoBE::Tau) {
        bearing_ -= EoBE::Tau;
    }

    energy_.Decay(100000 / 333);

    container.ForEachIn(Circle{ GetX(), GetY(), radius_ }, [&](Entity& other) -> void
    {
        if (FoodPellet* f = dynamic_cast<FoodPellet*>(&other)) {
            FeedOn(*f, f->GetEnergy());
        }
    });

    if (energy_.Quantity() > 300000) {
        container.AddEntity(GiveBirth());
    }

    return Move();
}

void Swimmer::Draw(QPainter& paint)
{
    paint.save();
    paint.setBrush(QColor(15, 15, 235));
    paint.drawEllipse({GetX(), GetY()}, radius_, radius_);
    paint.setPen(QColor(255, 255, 255));
    paint.drawLine(QPointF(GetX(), GetY()), QPointF(GetX() + std::sin(bearing_) * 50.0, GetY() + -(std::cos(bearing_) * 50.0)));
    paint.restore();
    paint.save();
    compass_.Draw(paint);
    paint.restore();
    paint.save();
    leftAntenna_.Draw(paint);
    paint.restore();
    paint.save();
    rightAntenna_.Draw(paint);
    paint.restore();
}

std::shared_ptr<Entity> Swimmer::GiveBirth()
{
    return std::make_shared<Egg>(energy_.CreateChild(100000), GetX(), GetY(), brain_.Mutated(), Random::Poisson(50u));
}

