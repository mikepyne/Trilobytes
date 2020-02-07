#include "Universe.h"

Universe::Universe(QWidget* parent)
    : QWidget(parent)
{
    for (auto i = 0; i < 50; i++) {
        swimmers_.push_back({Random::Real(0.0, 1000.0), Random::Real(0.0, 1000.0)});
    }

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
    p.translate(simX_, simY_);
    p.scale(simScale_, simScale_);

    p.drawRect(0, 0, 1000, 1000);

    for (auto& e : food_) {
        e.Draw(p);
    }
    for (auto& e : swimmers_) {
        e.Draw(p);
    }
    update();
}

void Universe::Thread()
{
    if (energy_ > 1) {
        double foodX;
        double foodY;
        //if (food_.size() < 100 || Random(0.0, 100.0) < 0.5) {
        foodX = Random::Real(0.0, 1000.0);
        foodY = Random::Real(0.0, 1000.0);
        //} else {
        //   auto& p = food_.at(Random(0u, food_.size() - 1u));
        //    foodX = p.x_ + Random(-17.0, 17.0);
        //    foodY = p.y_ + Random(-17.0, 17.0);
        //}

        food_.push_back({foodX, foodY});
        --energy_;
    }

    for (auto& e : swimmers_) {
        e.Tick();
        std::remove_if(std::begin(food_), std::end(food_), [&](FoodPellet& f)
        {
            if (e.DistanceSq(f) < std::pow(e.radius_, 2)) {
                --energy_;
                return true;
            }
            return false;
        });
    }

    update();
}
