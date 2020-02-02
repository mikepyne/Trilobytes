#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <QScrollArea>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>

#include "FoodPellet.h"
#include "Swimmer.h"
#include "Random.h"

#include <iostream>
#include <iomanip>
#include <iomanip>
#include <math.h>

class Universe : public QWidget {
    Q_OBJECT
public:
    Universe(QWidget *parent)
        : QWidget(parent)
    {
        //setGeometry(0, 0, 1000, 1000);
        //parent->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

        for (auto i = 0; i < 50; i++) {
            swimmers_.push_back({Random<double>(0, 1000.0), Random<double>(0, 1000.0)});
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
        timer->start(1);
    }

protected:
    void wheelEvent(QWheelEvent* event)
    {
        simScale_ += 0.001 * event->angleDelta().y();
        simScale_ = std::max(0.2, std::min(simScale_, 2.0));
    }

    virtual void mouseReleaseEvent(QMouseEvent* /*event*/) override final
    {
        dragging_ = false;
    }

    virtual void mousePressEvent(QMouseEvent* event) override final
    {
        dragging_ = true;
        dragX_ = event->x();
        dragY_ = event->y();
    }

    virtual void mouseMoveEvent(QMouseEvent* event) override final
    {
        if (dragging_) {
            simX_ += (event->x() - dragX_);
            simY_ += (event->y() - dragY_);
            dragX_ = event->x();
            dragY_ = event->y();
        }
    }

    virtual void paintEvent(QPaintEvent* /*event*/) override final
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

private:
    double simX_ = 0.0;
    double simY_ = 0.0;
    double simScale_ = 1.0;
    double dragX_;
    double dragY_;
    bool dragging_ = false;

    double energy_ = 1000.0;

    std::vector<FoodPellet> food_;
    std::vector<Swimmer> swimmers_;

    void Thread()
    {
        if (energy_ > 1) {
            double foodX;
            double foodY;
            if (food_.size() < 100 || Random(0.0, 100.0) < 0.5) {
                foodX = Random<double>(0.0, 1000.0);
                foodY = Random<double>(0.0, 1000.0);
            } else {
                auto& p = food_.at(Random(0u, food_.size() - 1u));
                foodX = p.x_ + Random(-17.0, 17.0);
                foodY = p.y_ + Random(-17.0, 17.0);
            }

            food_.push_back({foodX, foodY});
            --energy_;
        }

        // HACK simulate grazing
        while (Random(0u, food_.size()) > 500) {
            food_.erase(food_.begin() + Random(0u, food_.size() / 4u));
            ++energy_;
        }

        for (auto& e : swimmers_) {
            e.Tick();
        }

        update();
    }
};

#endif // UNIVERSE_H
