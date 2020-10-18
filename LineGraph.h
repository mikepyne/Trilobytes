#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class LineGraph : public QWidget {
    Q_OBJECT
public:
    LineGraph(QWidget* parent = nullptr);

    void PushBack(QRgb colour, double datum);
    void Reset();

protected:
    virtual void wheelEvent(QWheelEvent* event) override final;
    virtual void mouseReleaseEvent(QMouseEvent* event) override final;
    virtual void mousePressEvent(QMouseEvent* event) override final;
    virtual void mouseMoveEvent(QMouseEvent* event) override final;

    virtual void paintEvent(QPaintEvent* event) override final;

private:
    double graphX_ = 0.0;
    double graphY_ = 0.0;
    double graphScale_ = 1.0;
    double dragX_;
    double dragY_;
    bool dragging_ = false;

    std::map<QRgb, std::vector<double>> plots_;
};

#endif // BARGRAPH_H
