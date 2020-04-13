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
    , currentPelletCount_(0)
    , ticksTillNext_(0)
{
}

void FeedDispenser::Draw(QPainter& paint)
{
    paint.save();
    paint.setPen(QColor(0, 0, 0, 0));
    auto gradient = QRadialGradient(x_, y_, spawnStdDeviation_ * 3);
    gradient.setColorAt(0.0, QColor(0, 255, 100, 100));
    gradient.setColorAt(1.0, QColor(0, 255, 100, 10));
    paint.setBrush(gradient);
    paint.drawEllipse({x_, y_}, spawnStdDeviation_ * 3, spawnStdDeviation_ * 3);
    paint.restore();
}

void FeedDispenser::Tick()
{
    if (ticksTillNext_ == 0) {
        if (currentPelletCount_ < maxPelletCount_) {
            double foodX = Random::Gaussian(x_, spawnStdDeviation_);
            double foodY = Random::Gaussian(y_, spawnStdDeviation_);
            entityContainer_.AddEntity(std::make_shared<FoodPellet>(*this, energyPool_.CreateChild(25_mj), foodX, foodY));

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
}
