#ifndef SCATTERGRAPH_H
#define SCATTERGRAPH_H

#include "Utility/MinMax.h"

#include <QWidget>

class ScatterGraph : public QWidget {
public:
    struct DataPoint {
        QRgb colour_;
        qreal x_;
        qreal y_;
    };

    ScatterGraph(QWidget* parent, QString title, QString xAxisLabel, QString yAxisLabel);

    qreal GetXAxisMin() const { return xRange_.Min(); }
    qreal GetXAxisMax() const { return xRange_.Max(); }
    qreal GetYAxisMin() const { return yRange_.Min(); }
    qreal GetYAxisMax() const { return yRange_.Max(); }
    qreal GetXAxisMinOverride() const { return xAxisMinOverride_; }
    qreal GetXAxisMaxOverride() const { return xAxisMaxOverride_; }
    qreal GetYAxisMinOverride() const { return yAxisMinOverride_; }
    qreal GetYAxisMaxOverride() const { return yAxisMaxOverride_; }

    void SetTitle(QString newTitle) { graphTitle_ = newTitle; }
    void SetPoints(std::vector<DataPoint>&& points);
    void SetGraticuleHidden(bool hidden);
    void Reset();
    void RecalculateAxisBounds();

public slots:
    void EnableXAxisMinOverride(bool override) { xAxisMinOverride_ = override; update(); }
    void EnableXAxisMaxOverride(bool override) { xAxisMaxOverride_ = override; update(); }
    void EnableYAxisMinOverride(bool override) { yAxisMinOverride_ = override; update(); }
    void EnableYAxisMaxOverride(bool override) { yAxisMaxOverride_ = override; update(); }
    void SetXAxisMinOverride(qreal value) { xAxisMinOverrideValue_ = value; update(); }
    void SetXAxisMaxOverride(qreal value) { xAxisMaxOverrideValue_ = value; update(); }
    void SetYAxisMinOverride(qreal value) { yAxisMinOverrideValue_ = value; update(); }
    void SetYAxisMaxOverride(qreal value) { yAxisMaxOverrideValue_ = value; update(); }

protected:
    virtual void mouseMoveEvent(QMouseEvent* event) override final;
    virtual void paintEvent(QPaintEvent* event) override final;

private:
    QString graphTitle_;
    Tril::MinMax<qreal> xRange_;
    Tril::MinMax<qreal> yRange_;
    QString xAxisLabel_;
    QString yAxisLabel_;
    std::vector<DataPoint> points_;
    bool xAxisMinOverride_ = false;
    bool xAxisMaxOverride_ = false;
    bool yAxisMinOverride_ = false;
    bool yAxisMaxOverride_ = false;
    qreal xAxisMinOverrideValue_ = 0.0;
    qreal xAxisMaxOverrideValue_ = 0.0;
    qreal yAxisMinOverrideValue_ = 0.0;
    qreal yAxisMaxOverrideValue_ = 0.0;

    bool graticuleHidden_ = true;
    QPointF graticuleLocation_;

    QPointF PaintAxes(QPainter& painter) const;
    void PaintTitle(QPainter& painter) const;
    void PaintGraticule(QPainter& painter, const QPointF& target, const QRectF& area) const;
};

#endif // SCATTERGRAPH_H
