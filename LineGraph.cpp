#include "LineGraph.h"

#include <cmath>
#include <limits>

LineGraph::LineGraph(QWidget* parent)
    : QWidget(parent)
{
    setFixedWidth(100);
}

void LineGraph::PushBack(QRgb colour, double datum)
{
    plots_[colour].push_back(datum);
    update();
}

void LineGraph::Reset()
{
    for (auto& [colour, plot] : plots_) {
        (void) colour;
        plot.clear();
    }
}

void LineGraph::paintEvent(QPaintEvent* /*event*/)
{
    QPainter paint(this);

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
