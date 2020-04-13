#ifndef FEEDDISPENSER_H
#define FEEDDISPENSER_H

class EnergyPool;
class QuadTree;
class QPainter;

class FeedDispenser {
public:
    FeedDispenser(EnergyPool& energyPool, QuadTree& entityContainer, double x, double y, double averageSpawnRadius, unsigned maxPelletCount, unsigned averageTicksBetweenPellets_);

    void Draw(QPainter& paint);
    void Tick();
    void PelletEaten();

private:
    EnergyPool& energyPool_;
    QuadTree& entityContainer_;
    double x_;
    double y_;
    double spawnStdDeviation_;
    unsigned maxPelletCount_;
    unsigned averageTicksBetweenPellets_;

    unsigned currentPelletCount_;
    unsigned ticksTillNext_;
};

#endif // FEEDDISPENSER_H
