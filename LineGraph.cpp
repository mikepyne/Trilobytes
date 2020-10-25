#include "LineGraph.h"

#include <QPaintEvent>
#include <QPainter>

#include <cmath>
#include <limits>

LineGraph::LineGraph(QWidget* parent, QString xAxisLabel, QString yAxisLabel)
    : QWidget(parent)
    , xRange_()
    , yRange_()
    , xAxisLabel_(xAxisLabel)
    , yAxisLabel_(yAxisLabel)
{
    QPalette p = palette();
    p.setColor(QPalette::ColorRole::Window, QColor(Qt::white));
    setPalette(p);
    setAutoFillBackground(true);
}

void LineGraph::AddPlot(QRgb colour, QString name)
{
    plots_.push_back({ name, colour, {} });
}

void LineGraph::AddPoint(size_t index, qreal x, qreal y)
{
    xRange_.ExpandToContain(x);
    yRange_.ExpandToContain(y);
    plots_.at(index).points_.push_back({ x, y });
    update();
}

void LineGraph::Reset()
{
    xRange_.Reset();
    yRange_.Reset();
    for (auto& [ name, colour, points ] : plots_) {
        (void) name; // unused
        (void) colour; // unused
        points.clear();
    }
    update();
}

void LineGraph::paintEvent(QPaintEvent* event)
{
    QPainter paint(this);
    paint.setClipRegion(event->region());

    QPointF origin = PaintAxes(paint);
    PaintKey(paint);
    qreal xAxisLength = width() - origin.x();
    qreal yAxisLength = origin.y();

    for (auto& [ name, colour, points ] : plots_) {
        (void) name; // unused
        paint.setPen(colour);
        QPointF lastPoint = origin;
        for (const auto& [ dataX, dataY ] : points) {
            QPointF nextPoint(origin.x() + ( xAxisLength * ((dataX - xRange_.Min()) / xRange_.Range())), origin.y() - (yAxisLength * ((dataY - yRange_.Min()) / yRange_.Range())));
            if (lastPoint != origin) {
                paint.drawLine(lastPoint, nextPoint);
            }
            lastPoint = nextPoint;
        }
    }
}

QPointF LineGraph::PaintAxes(QPainter& p) const
{
    QFontMetricsF metrics(p.font());

    QString yMinLabel = QString("%1").arg(yRange_.Min(), 0, 'f', 2);
    QString yMaxLabel = QString("%1").arg(yRange_.Max(), 0, 'f', 2);
    QString xMinLabel = QString("%1").arg(xRange_.Min(), 0, 'f', 2);
    QString xMaxLabel = QString("%1").arg(xRange_.Max(), 0, 'f', 2);

    // The bottem left of the graph, aka (minX, minY)
    QPointF origin(std::max({ metrics.height(), metrics.width(yMinLabel), metrics.width(yMaxLabel) }), height() - (2 * metrics.height()));

    /*
     * Draw the X and Y min/max values
     */
    QRectF yMinRect(0, 0, metrics.width(yMinLabel), metrics.height());
    QRectF yMaxRect(0, 0, metrics.width(yMaxLabel), metrics.height());
    QRectF xMinRect(0, 0, metrics.width(xMinLabel), metrics.height());
    QRectF xMaxRect(0, 0, metrics.width(xMaxLabel), metrics.height());

    yMinRect.translate(origin.x() - yMinRect.width(), origin.y() - yMinRect.height());
    yMaxRect.translate(origin.x() - yMaxRect.width(), 0);
    xMinRect.translate(origin.x(), height() - (xMinRect.height() * 2.0));
    xMaxRect.translate(width() - xMaxRect.width(), height() - (xMaxRect.height() * 2.0));

    p.drawText(yMinRect, yMinLabel);
    p.drawText(yMaxRect, yMaxLabel);
    p.drawText(xMinRect, xMinLabel);
    p.drawText(xMaxRect, xMaxLabel);

    /*
     * Draw the Axis labels & axis lines
     */
    qreal xAxisLength = width() - origin.x();
    qreal yAxisLength = origin.y();

    p.setPen(Qt::black);
    p.drawLine(QLineF(origin, QPointF(origin.x(), 0)));
    p.drawLine(QLineF(origin, QPointF(width(), origin.y())));

    QRectF xLabelRect(0, 0, metrics.width(xAxisLabel_), metrics.height());
    QRectF yLabelRect(0, 0, metrics.width(yAxisLabel_), metrics.height());

    xLabelRect.translate(((xAxisLength - xLabelRect.width()) / 2.0) + origin.x(), height() - (xLabelRect.height() * 2));
    yLabelRect.translate((yLabelRect.height() / 2.0) - (yLabelRect.width() / 2.0), (yAxisLength / 2.0) - (yLabelRect.height() / 2.0));

    p.drawText(xLabelRect, xAxisLabel_);
    p.translate(yLabelRect.center());
    yLabelRect.translate(-yLabelRect.center());
    p.rotate(270.0);
    p.drawText(yLabelRect, yAxisLabel_);
    p.resetTransform();

    return origin;
}

void LineGraph::PaintKey(QPainter& p) const
{
    QFontMetricsF metrics(p.font());

    // note names for drawing a key
    qreal y = height() - metrics.height();
    qreal keyWidth = 0;
    for (auto& [ name, colour, points ] : plots_) {
        (void) colour; // unused
        (void) points; // unused
        keyWidth += metrics.width(name) + metrics.height() + 6;
    }

    qreal left = (width() - keyWidth) / 2.0;
    p.setPen(Qt::black);
    for (auto& [ name, colour, points ] : plots_) {
        (void) points; // unused
        QRectF colourRect(0, 0, metrics.height(), metrics.height());
        QRectF labelRect(0, 0, metrics.width(name), metrics.height());
        colourRect.translate(left, y);
        labelRect .translate(left + colourRect.width() + 3, y);

        p.fillRect(colourRect, QColor::fromRgb(colour));
        p.drawText(labelRect, name);

        left += labelRect.width() + colourRect.width() + 6;
    }
}
