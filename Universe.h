#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "FoodPellet.h"
#include "Swimmer.h"
#include "QuadTree.h"
#include "EnergyPool.h"
#include "Random.h"

#include <QScrollArea>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>

#include <iomanip>
#include <iomanip>
#include <math.h>

class Universe : public QWidget {
    Q_OBJECT
public:
    Universe(QWidget *parent);

protected:
    virtual void wheelEvent(QWheelEvent* event) override final;
    virtual void mouseReleaseEvent(QMouseEvent* /*event*/) override final;
    virtual void mousePressEvent(QMouseEvent* event) override final;
    virtual void mouseMoveEvent(QMouseEvent* event) override final;

    virtual void keyPressEvent(QKeyEvent* event) override;

    virtual void paintEvent(QPaintEvent* event) override final;

private:
    double simX_ = 0.0;
    double simY_ = 0.0;
    double simScale_ = 1.0;
    double dragX_;
    double dragY_;
    bool dragging_ = false;

    bool spawnFood_ = true;
    bool pauseSim_ = false;
    bool respawn_ = true;

    EnergyPool& energy_;

    QuadTree rootNode_;

    void Thread();
};

#endif // UNIVERSE_H
