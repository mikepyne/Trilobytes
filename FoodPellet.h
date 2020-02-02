#ifndef FOODPELLET_H
#define FOODPELLET_H

#include <QPainter>

class FoodPellet {
public:
    double x_;
    double y_;

    void Draw(QPainter& paint)
    {
        paint.setBrush(QColor(15, 235, 15));
        paint.drawEllipse(QPointF{x_, y_,}, 2.5, 2.5);
    }
};

#endif // FOODPELLET_H
