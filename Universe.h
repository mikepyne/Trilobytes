#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <QWidget>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>

#include "Random.h"

#include <iostream>
#include <iomanip>
#include <math.h>

class Universe : public QWidget
{
    Q_OBJECT
public:
    Universe(QWidget *parent)
        : QWidget(parent)
    {
        setGeometry(0, 0, 1000, 1000);

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
    void paintEvent(QPaintEvent* event)
    {
        QPainter p(this);

        for (auto& e : food_) {
            e.Draw(p);
        }
        for (auto& e : swimmers_) {
            e.Draw(p);
        }
        update();
    }

private:
    class FoodPellet {
    public:
        double x, y;

        void Draw(QPainter& paint) { paint.setBrush(QColor(15, 235, 15)); paint.drawEllipse(QPointF{x, y,}, 2.5f, 2.5f); }
    };

    class Swimmer {
    public:
        void Tick(){}
        void Draw(QPainter& paint){}
    };

    double energy_ = 1000.0;

    std::vector<FoodPellet> food_;
    std::vector<Swimmer> swimmers_;

    void Thread()
    {
        if (energy_ > 1) {
            double foodX;
            double foodY;
            if (food_.size() < 100 || Random(0.0, 100.0) < 0.5) {
                foodX = Random<double>(0.0, width());
                foodY = Random<double>(0.0, height());
            } else {
                auto& p = food_.at(Random(0u, food_.size() - 1u));
                foodX = p.x + Random(-17.0, 17.0);
                foodY = p.y + Random(-17.0, 17.0);
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
