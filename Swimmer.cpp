#include "Swimmer.h"
#include "Random.h"
#include "FoodPellet.h"
#include "Utils.h"
#include "Egg.h"

#include <QPainter>

#include <math.h>

Swimmer::Swimmer(EnergyPool&& energy, double x, double y)
    : Swimmer(std::move(energy), x, y, NeuralNetwork(3, 7))
{
}

Swimmer::Swimmer(EnergyPool&& energy, double x, double y, NeuralNetwork&& brain)
    : Entity(std::move(energy), x, y, 6.0, QColor::fromRgb(15, 15, 235))
    , brain_(std::move(brain))
    , taste_(*this, 0.0, 0.0, GetRadius(), SenseEntityPresence::MakeCustomFilter<FoodPellet>(0, { 1.0 }))
    , leftAntenna_(*this, -0.6,  GetRadius() * 3.5, GetRadius() * 5, SenseEntityDistance::MakeCustomFilter<FoodPellet>(0, { 1.0 }))
    , rightAntenna_(*this, 0.6,  GetRadius() * 3.5, GetRadius() * 5, SenseEntityDistance::MakeCustomFilter<FoodPellet>(0, { 1.0 }))
    , echoLocator_(*this, GetRadius() * 2, 0.0)
    , compass_(*this)
    , rand_(*this, 1)
{
    SetSpeed(0.5);
}

Swimmer::~Swimmer()
{
}

std::shared_ptr<Entity> Swimmer::GiveBirth()
{
    return std::make_shared<Egg>(TakeEnergy(100_mj), GetX(), GetY(), brain_.Mutated(), Random::Poisson(50u));
}

void Swimmer::TickImpl(EntityContainerInterface& container)
{
    // TODO bearing should JUMP at Pi to -Pi (not Tau to 0), also makes maths and normalising easier
    auto taste = taste_.Tick(container, {});
    auto leftSmell = leftAntenna_.Tick(container, {});
    auto rightSmell = rightAntenna_.Tick(container, {});
    auto echo = echoLocator_.Tick(container, {});
    auto compass = compass_.Tick(container, {});
    auto rand = rand_.Tick(container, {});

    auto& out = brain_.ForwardPropogate({ taste[0], leftSmell[0], rightSmell[0], echo[0], compass[0], compass[1], rand[0] });
    double newBearing = GetBearing();
    newBearing += (out[1]);
    if (newBearing < 0.0) {
        newBearing += EoBE::Tau;
    } else if (newBearing > EoBE::Tau) {
        newBearing -= EoBE::Tau;
    }
    SetBearing(newBearing);

    UseEnergy(333_uj); // TODO remove this, entities will use energy based on what they are doing (well maybe a small base usage would deter couch potatoes...)

    container.ForEachIn(Circle{ GetX(), GetY(), GetRadius() }, [&](Entity& other) -> void
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
    paint.restore();
    paint.save();
    paint.setPen(QColor(255, 255, 255));
    echoLocator_.Draw(paint);
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

