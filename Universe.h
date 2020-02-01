#ifndef UNIVERSE_H
#define UNIVERSE_H

#include <QWidget>
#include <QTimer>
#include <QPaintEvent>
#include <QPainter>

#include <math.h>

class Universe : public QWidget
{
    Q_OBJECT
public:
    Universe(QWidget *parent)
        : QWidget(parent)
    {
        setGeometry(0, 0, 1000, 1000);

        QTimer* timer = new QTimer();
        timer->setSingleShot(false);
        timer->connect(timer, &QTimer::timeout, [&](auto) -> void
        {
            this->Thread();
        });
        timer->start(20);

        food_.push_back({500, 500});
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
    class Entity {
    public:
        float x, y;
        void Tick() { x = std::sin(x) * 500; y = std::cos(y) * 500; }
        void Draw(QPainter& paint) { paint.fillRect(x, y, 50, 50, QColor(255, 25, 2)); }
    };

    std::vector<Entity> food_;
    std::vector<Entity> swimmers_;

    void Thread()
    {
        for (auto& e : food_) {
            e.Tick();
        }
        for (auto& e : swimmers_) {
            e.Tick();
        }

        update();
    }
};

#endif // UNIVERSE_H
