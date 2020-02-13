#include "Universe.h"

Universe::Universe(QWidget* parent)
    : QWidget(parent)
    , rootNode_({0, 0, 1000, 1000})
{
    /*
     * QT hack to get this running in the QT event loop (necessary for
     * drawing anything, not ideal for running the Sim quickly...)
     *
     * Leaking this pointer... not intended to last long
     */
    QTimer* timer = new QTimer();
    timer->setSingleShot(false);
    timer->connect(timer, &QTimer::timeout, [&](auto) -> void
    {
        this->Thread();
    });
    timer->start(20);

    for (auto n : std::vector<double>(30, 0.0)) {
        rootNode_.AddEntity(std::make_shared<Swimmer>(Random::Real(n, 1000.0), Random::Real(0.0, 1000.0)));
        energy_ -= 1.0;
    }
}

void Universe::wheelEvent(QWheelEvent* event)
{
    simScale_ += 0.001 * event->angleDelta().y();
    simScale_ = std::max(0.2, std::min(simScale_, 2.0));
}

void Universe::mouseReleaseEvent(QMouseEvent*)
{
    dragging_ = false;
}

void Universe::mousePressEvent(QMouseEvent* event)
{
    dragging_ = true;
    dragX_ = event->x();
    dragY_ = event->y();
}

void Universe::mouseMoveEvent(QMouseEvent* event)
{
    if (dragging_) {
        simX_ += (event->x() - dragX_);
        simY_ += (event->y() - dragY_);
        dragX_ = event->x();
        dragY_ = event->y();
    }
}

void Universe::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setBackground(QColor(200, 225, 255));
    p.drawText(0, 15, QString::number(rootNode_.EntityCount()));
    p.translate(simX_, simY_);
    p.scale(simScale_, simScale_);


    rootNode_.Draw(p);
}

void Universe::Thread()
{
    rootNode_.Tick();

    if (energy_ > 1) {
        double foodX = Random::Real(0.0, 1000.0);
        double foodY = Random::Real(0.0, 1000.0);
        rootNode_.AddEntity(std::make_shared<FoodPellet>(foodX, foodY));
        --energy_;
    }

    // QT paint call
    update();
}
