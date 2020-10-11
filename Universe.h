#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "QuadTree.h"
#include "Energy.h"
#include "LineGraph.h"
#include "FeedDispenser.h"

#include <QScrollArea>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>

#include <iomanip>
#include <math.h>

class Universe : public QWidget {
    Q_OBJECT
public:
    Universe(QWidget *parent);

    void SetupGraphs(LineGraph& graph);

signals:
    void OnFoodEnergySampled(Energy energy);
    void OnSwimmerEnergySampled(Energy energy);
    void OnGraphResetRequested();

protected:
    virtual void wheelEvent(QWheelEvent* event) override final;
    virtual void mouseReleaseEvent(QMouseEvent* event) override final;
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
    bool autoSelectFittest_ = false;
    bool trackSelectedEntity_ = false;
    std::shared_ptr<Entity> selectedEntity_;
    unsigned lastEntityCount_ = 0;
    unsigned lastSwimmerCount_ = 0;

    QuadTree rootNode_;
    std::vector<std::shared_ptr<FeedDispenser>> feedDispensers_;

    QTimer mainThread_;

    void Thread();

    Point TransformLocalToSimCoords(const Point& local);
    Point TransformSimToLocalCoords(const Point& sim);
    void SelectFittestSwimmer();
};

#endif // UNIVERSE_H
