#ifndef BARGRAPH_H
#define BARGRAPH_H

#include "Utils.h"

#include <QWidget>
#include <QString>

class LineGraph : public QWidget {
    Q_OBJECT
public:
    LineGraph(QWidget* parent, QString xAxisLabel, QString yAxisLabel);

    void AddPlot(QRgb colour, QString name);
    void AddPoint(size_t index, qreal x, qreal y);
    void Reset();

protected:
    virtual void paintEvent(QPaintEvent* event) override final;

private:
    struct Plot {
        QString name_;
        QRgb colour_;
        std::vector<std::pair<qreal, qreal>> points_;
    };
    EoBE::Range<qreal> xRange_;
    EoBE::Range<qreal> yRange_;
    QString xAxisLabel_;
    QString yAxisLabel_;
    std::vector<Plot> plots_;

    QPointF PaintAxes(QPainter& painter) const;
    void PaintKey(QPainter& painter) const;
};

#endif // BARGRAPH_H
