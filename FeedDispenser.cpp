#include "FeedDispenser.h"

#include "Random.h"
#include "Energy.h"
#include "FoodPellet.h"
#include "QuadTree.h"

#include <QPainter>


FeedDispenser::FeedDispenser(QuadTree& entityContainer, double x, double y, double radius, double pelletDensity)
    : entityContainer_(entityContainer)
    , x_(x)
    , y_(y)
    , radius_(radius)
    , maxPellets_(std::pow(EoBE::Pi * radius_, 2.0) * pelletDensity)
    , ticksTillNext_(0)
    , currentPelletCount_(0)
{
    while(currentPelletCount_ < maxPellets_ / 4) {
        SpawnPellet();
    }
}

void FeedDispenser::Draw(QPainter& paint)
{
    paint.save();
    paint.setPen(QColor(0, 205, 90, 255));
    paint.setBrush(QColor(0, 205, 90, 190).lighter());
    paint.drawEllipse({x_, y_}, GetRadius(), GetRadius());
    paint.restore();
}

void FeedDispenser::Tick()
{
    while (ticksTillNext_ <= 0 && currentPelletCount_ < maxPellets_) {
        SpawnPellet();
    }
    ticksTillNext_ -= 1.0;
}

void FeedDispenser::PelletEaten()
{
    --currentPelletCount_;
    //    ticksTillNext_ -= currentPelletCount_ / maxPellets_;
}

void FeedDispenser::SpawnPellet()
{
    double rotation = Random::Number(0.0, EoBE::Tau);
    double distance = std::sqrt(Random::Number(0.0, 1.0)) * radius_;
    double foodX = x_ + distance * std::cos(rotation);
    double foodY = y_ + distance * std::sin(rotation);
    if (entityContainer_.SelectEntities(Point{ foodX, foodY }).empty()) {
        entityContainer_.AddEntity(std::make_shared<FoodPellet>(*this, 50_mj, Transform{ foodX, foodY, 0 }));
        ++currentPelletCount_;
//                ticksTillNext_ = 1'00.0 * std::pow(double(currentPelletCount_) / double(maxPellets_), 2.0);
//        ticksTillNext_ += std::max(0.1, 10.0 * (1.0 - std::pow(double(currentPelletCount_) / double(maxPellets_), 0.5)));
        auto x = double(currentPelletCount_) / double(maxPellets_);
        ticksTillNext_ += 10.0 * ((-0.8 * (std::pow(x, 2.0) * -std::pow(x - 2, 2.0))) + 0.1);
    }
}
