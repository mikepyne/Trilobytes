#include "FeedDispenser.h"

#include "Random.h"
#include "EnergyPool.h"
#include "FoodPellet.h"
#include "QuadTree.h"

#include <QPainter>

FeedDispenser::FeedDispenser(EnergyPool& energyPool, QuadTree& entityContainer, double x, double y, double spawnStdDeviation, unsigned maxPelletCount, unsigned averageTicksBetweenPellets)
    : energyPool_(energyPool)
    , entityContainer_(entityContainer)
    , x_(x)
    , y_(y)
    , spawnStdDeviation_(spawnStdDeviation)
    , maxPelletCount_(maxPelletCount)
    , averageTicksBetweenPellets_(averageTicksBetweenPellets)
    , xVelocity_(0)
    , yVelocity_(0)
    , stepsRemaining_(0)
    , currentPelletCount_(0)
    , ticksTillNext_(0)
{
}

void FeedDispenser::Draw(QPainter& paint)
{
    paint.save();
    paint.setPen(QColor(0, 205, 90, 255));
    auto gradient = QRadialGradient(x_, y_, GetRadius());
    gradient.setColorAt(0.0, QColor(0, 255, 100, 100));
    gradient.setColorAt(1.0, QColor(0, 255, 100, 10));
    paint.setBrush(gradient);
    paint.drawEllipse({x_, y_}, GetRadius(), GetRadius());
    paint.restore();
}

void FeedDispenser::Tick()
{
    if (ticksTillNext_ == 0) {
        if (currentPelletCount_ < maxPelletCount_) {
            double foodX = Random::Gaussian(x_, spawnStdDeviation_);
            double foodY = Random::Gaussian(y_, spawnStdDeviation_);
            entityContainer_.AddEntity(std::make_shared<FoodPellet>(*this, energyPool_.CreateChild(25_mj), Transform{ foodX, foodY, 0 }));

            ++currentPelletCount_;
            ticksTillNext_ = averageTicksBetweenPellets_;
        }
    } else {
        --ticksTillNext_;
    }
}

void FeedDispenser::PelletEaten()
{
    --currentPelletCount_;
    if (stepsRemaining_ == 0) {
        double xDestination = Random::Number<double>(-1500 / 2, 1500 / 2);
        double yDestination = Random::Number<double>(-1500 / 2, 1500 / 2);
        double xDistance = xDestination - x_;
        double yDistance = yDestination - y_;
        double distance = std::sqrt((xDistance * xDistance) + (yDistance * yDistance));
        double speed = 0.01;
        stepsRemaining_ = static_cast<unsigned>(std::round(distance / speed));
        xVelocity_ = xDistance / stepsRemaining_;
        yVelocity_ = yDistance / stepsRemaining_;
    } else {
        x_ += xVelocity_;
        y_ += yVelocity_;
        --stepsRemaining_;
    }
}
