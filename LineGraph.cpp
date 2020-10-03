#include "LineGraph.h"

#include <QMouseEvent>

#include <cmath>
#include <limits>

LineGraph::LineGraph(QWidget* parent)
    : QWidget(parent)
{
}

void LineGraph::PushBack(QRgb colour, double datum)
{
    plots_[colour].push_back(datum);
    update();
}

void LineGraph::Reset()
{
    plots_.clear();
}

void LineGraph::wheelEvent(QWheelEvent* event)
{
    double d = 1.0 + (0.001 * double(event->angleDelta().y()));
    graphScale_ *= d;
    graphX_ *= d;
    graphY_ *= d;
    update();
}

void LineGraph::mouseReleaseEvent(QMouseEvent*)
{
    dragging_ = false;
}

void LineGraph::mousePressEvent(QMouseEvent* event)
{
    dragging_ = true;
    dragX_ = event->x();
    dragY_ = event->y();
}

void LineGraph::mouseMoveEvent(QMouseEvent* event)
{
    if (dragging_) {
        graphX_ += (event->x() - dragX_);
        graphY_ += (event->y() - dragY_);
        dragX_ = event->x();
        dragY_ = event->y();
        update();
    }
}

void LineGraph::paintEvent(QPaintEvent* /*event*/)
{
    QPainter paint(this);
    paint.translate(graphX_, graphY_);
    paint.scale(graphScale_, graphScale_);


    double max = std::numeric_limits<double>::lowest();
    size_t count = 0;
    for (auto& [colour, plot ] : plots_) {
        (void) colour;
        count = std::max(count, plot.size());
        for (const double& datum : plot) {
            max = std::max(max, datum);
        }
    }

    double xIncrement = static_cast<double>(width() - 1) / max;
    double yDistance = static_cast<double>(height()) / (count - 1);
    for (auto& [colour, plot] : plots_) {
        paint.setPen(colour);
        if (!plot.empty()) {
            double lastX = plot.front() * xIncrement;
            double lastY = -yDistance / 2.0;
            for (const double& datum : plot) {
                double nextX = xIncrement * datum;
                double nextY = lastY + yDistance;
                paint.drawLine(std::round(lastX), std::round(lastY), std::round(nextX), std::round(nextY));
                lastX = nextX;
                lastY = nextY;
            }
        }
    }
}
