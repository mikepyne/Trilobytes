#ifndef FEEDDISPENSER_H
#define FEEDDISPENSER_H

class QuadTree;
class QPainter;

#include <memory>

class FeedDispenser : public std::enable_shared_from_this<FeedDispenser> {
public:
    FeedDispenser(QuadTree& entityContainer, double x, double y, double radius, double pelletDensity);

    void Draw(QPainter& paint);
    void Tick();
    void PelletEaten();
    void AddPelletsImmediately(unsigned pelletCount);

    double GetX() const { return x_; }
    double GetY() const { return y_; }
    double GetRadius() const { return radius_; }
    unsigned GetMaxPellets() const { return maxPellets_; }

private:
    QuadTree& entityContainer_;

    double x_;
    double y_;
    double radius_;

    unsigned maxPellets_;
    double ticksTillNext_;

    unsigned currentPelletCount_;

    void SpawnPellet();
};

#endif // FEEDDISPENSER_H
