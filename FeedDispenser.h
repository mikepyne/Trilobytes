#ifndef FEEDDISPENSER_H
#define FEEDDISPENSER_H

class QuadTree;
class QPainter;

class FeedDispenser {
public:
    FeedDispenser(QuadTree& entityContainer, double x, double y, double radius, double pelletDensity);

    void Draw(QPainter& paint);
    void Tick();
    void PelletEaten();

    double GetX() const { return x_; }
    double GetY() const { return y_; }
    double GetRadius() const { return radius_; }

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
