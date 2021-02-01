#include "LineGraph.h"

#include <QPaintEvent>
#include <QPainter>

#include <cmath>
#include <limits>

LineGraph::LineGraph(QWidget* parent, QString xAxisLabel, QString yAxisLabel, size_t plotDataPointCount)
    : QWidget(parent)
    , xRange_()
    , yRange_()
    , xAxisLabel_(xAxisLabel)
    , yAxisLabel_(yAxisLabel)
    , plotDataPointCount_(plotDataPointCount)
{
    QPalette p = palette();
    p.setColor(QPalette::ColorRole::Window, QColor(Qt::white));
    setPalette(p);
    setAutoFillBackground(true);
}

void LineGraph::ForEachPlot(const std::function<void(const Plot& plot, size_t plotIndex)>& action) const
{
    size_t index = 0;
    for (const auto& plot : plots_) {
        action(plot, index++);
    }
}

void LineGraph::AddPlot(QRgb colour, QString name)
{
    plots_.push_back({ name, colour, false, decltype(Plot::points_)(plotDataPointCount_) });
    update();
}

void LineGraph::AddPoint(size_t index, qreal x, qreal y)
{
    if (plots_.size() > index) {
        plots_.at(index).points_.PushBack({ x, y });
        if (!plots_.at(index).hidden_) {
            xRange_.ExpandToContain(x);
            yRange_.ExpandToContain(y);
            if (plots_.at(index).points_.Full()) {
                RecalculateAxisBounds();
            }
        }
        update();
    }
}

void LineGraph::SetPlotHidden(size_t plotIndex, bool hidden)
{
    plots_.at(plotIndex).hidden_ = hidden;
    RecalculateAxisBounds();
    update();
}

void LineGraph::SetGraticuleHidden(bool hidden)
{
    graticuleHidden_ = hidden;
    setMouseTracking(!hidden);
    setCursor(hidden ? Qt::CursorShape::ArrowCursor : Qt::CursorShape::CrossCursor);
    update();
}

void LineGraph::Reset()
{
    xRange_.Reset();
    yRange_.Reset();
    for (auto& [ name, colour, hidden, points ] : plots_) {
        (void) name; // unused
        (void) colour; // unused
        (void) hidden; // unused
        points.Clear();
    }
    update();
}

void LineGraph::RecalculateAxisBounds()
{
    xRange_.Reset();
    yRange_.Reset();
    for (auto& [ name, colour, hidden, points ] : plots_) {
        (void) name; // unused
        (void) colour; // unused
        if (!hidden) {
            points.ForEach([&](const auto& pair)
            {
                const auto& [ dataX, dataY ] = pair;{}
                xRange_.ExpandToContain(dataX);
                yRange_.ExpandToContain(dataY);
            });
        }
    }
}

void LineGraph::SetPlotDataPointCount(size_t count)
{
    plotDataPointCount_ = count;
    for (Plot& plot : plots_) {
        plot.points_.Resize(count);
    }
}

void LineGraph::mouseMoveEvent(QMouseEvent* event)
{
    graticuleLocation_ = event->pos();
    update();
}

void LineGraph::paintEvent(QPaintEvent* event)
{
    QPainter paint(this);
    paint.setClipRegion(event->region());

    const Tril::MinMax<qreal> xRange = { xAxisMinOverride_ ? xAxisMinOverrideValue_ : xRange_.Min(), xAxisMaxOverride_ ? xAxisMaxOverrideValue_ : xRange_.Max() };
    const Tril::MinMax<qreal> yRange = { yAxisMinOverride_ ? yAxisMinOverrideValue_ : yRange_.Min(), yAxisMaxOverride_ ? yAxisMaxOverrideValue_ : yRange_.Max() };

    const QPointF origin = PaintAxes(paint);
    PaintKey(paint);

    const qreal xAxisLength = width() - origin.x();
    const qreal yAxisLength = origin.y();

    paint.setClipRegion(event->region().intersected(QRect(QPoint(origin.x(), 0.0), QPoint(width(), yAxisLength))));

    for (auto& [ name, colour, hidden, points ] : plots_) {
        (void) name; // unused
        if (!hidden) {
            paint.setPen(colour);
            QPointF lastPoint = origin;
            points.ForEach([&](const auto& pair)
            {
                const auto& [ dataX, dataY ] = pair;{}
                QPointF nextPoint(origin.x() + ( xAxisLength * ((dataX - xRange.Min()) / xRange.Range())), origin.y() - (yAxisLength * ((dataY - yRange.Min()) / yRange.Range())));
                if (lastPoint != origin) {
                    paint.drawLine(lastPoint, nextPoint);
                }
                lastPoint = nextPoint;
            });
        }
    }

    if (!graticuleHidden_) {
        PaintGraticule(paint, graticuleLocation_, QRectF(origin.x(), 0, xAxisLength, yAxisLength));
    }
}

QPointF LineGraph::PaintAxes(QPainter& p) const
{
    QFontMetricsF metrics(p.font());

    Tril::MinMax<qreal> xRange = { xAxisMinOverride_ ? xAxisMinOverrideValue_ : xRange_.Min(), xAxisMaxOverride_ ? xAxisMaxOverrideValue_ : xRange_.Max() };
    Tril::MinMax<qreal> yRange = { yAxisMinOverride_ ? yAxisMinOverrideValue_ : yRange_.Min(), yAxisMaxOverride_ ? yAxisMaxOverrideValue_ : yRange_.Max() };

    QString yMinLabel = QString("%1").arg(yRange.Min(), 0, 'f', 2);
    QString yMaxLabel = QString("%1").arg(yRange.Max(), 0, 'f', 2);
    QString xMinLabel = QString("%1").arg(xRange.Min(), 0, 'f', 2);
    QString xMaxLabel = QString("%1").arg(xRange.Max(), 0, 'f', 2);

    // The bottem left of the graph, aka (minX, minY)
    QPointF origin(std::max(metrics.width(yMinLabel) + metrics.height(), metrics.width(yMaxLabel) + metrics.height()), height() - (2 * metrics.height()));

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
    for (auto& [ name, colour, hidden, points ] : plots_) {
        (void) colour; // unused
        (void) points; // unused
        if (!hidden) {
            keyWidth += metrics.width(name) + metrics.height() + 6;
        }
    }

    qreal left = (width() - keyWidth) / 2.0;
    p.setPen(Qt::black);
    for (auto& [ name, colour, hidden, points ] : plots_) {
        (void) points; // unused
        if (!hidden) {
            QRectF colourRect(0, 0, metrics.height(), metrics.height());
            QRectF labelRect(0, 0, metrics.width(name), metrics.height());
            colourRect.translate(left, y);
            labelRect .translate(left + colourRect.width() + 3, y);

            p.fillRect(colourRect, QColor::fromRgb(colour));
            p.drawText(labelRect, name);

            left += labelRect.width() + colourRect.width() + 6;
        }
    }
}

void LineGraph::PaintGraticule(QPainter& painter, const QPointF& target, const QRectF& area) const
{
    if (area.contains(target)) {
        // Draw lines inverse so they show up over plots
        painter.save();
        painter.setClipRect(area);
        painter.setCompositionMode(QPainter::CompositionMode_Exclusion);
        painter.setPen(Qt::white);
        painter.drawLines({ QLineF(QPointF(0, target.y()), QPointF(width(), target.y())), QLineF(QPointF(target.x(), 0), QPointF(target.x(), height())) });
        painter.restore();

        // Calculate axis values for point
        Tril::RangeConverter xConverter(Tril::Range(area.left(), area.right()), xRange_);
        Tril::RangeConverter yConverter(Tril::Range(area.bottom(), area.top()), yRange_);

        qreal xValueAtCrosshair = xConverter.Convert(target.x());
        qreal yValueAtCrosshair = yConverter.Convert(target.y());

        // Display axis values
        QString coordsText = QString("(%1, %2)").arg(xValueAtCrosshair).arg(yValueAtCrosshair);

        QFontMetricsF metrics = painter.fontMetrics();
        // Weird bug where half the text doesn't render without expanding the rect!
        QRectF coordsRect = metrics.boundingRect(coordsText).adjusted(0, 0, 1, 0);

        // Move the text away from the graticule slightly
        qreal adjustment = 5.0;

        // Make sure we don't draw them off screen
        bool left = target.x() - (coordsRect.width() + adjustment) > area.left();
        bool bottom = target.y() - (coordsRect.height() + adjustment) < area.top();

        coordsRect.translate(target + QPointF(left ? -(coordsRect.width() + adjustment) : adjustment, bottom ? coordsRect.height() + adjustment : -adjustment));

        painter.setPen(Qt::black);
        painter.fillRect(coordsRect.adjusted(-3, -3, 3, 3), Qt::white);
        painter.drawText(coordsRect, coordsText);
    }
}

