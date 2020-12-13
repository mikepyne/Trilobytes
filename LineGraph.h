#ifndef BARGRAPH_H
#define BARGRAPH_H

#include "Utils.h"

#include <QWidget>
#include <QString>

class LineGraph : public QWidget {
    Q_OBJECT
public:
    struct Plot {
        QString name_;
        QRgb colour_;
        bool hidden_;
        EoBE::CircularBuffer<std::pair<qreal, qreal>> points_;
    };

    LineGraph(QWidget* parent, QString xAxisLabel, QString yAxisLabel, size_t plotDataPointCount = 20'000);

    qreal GetXAxisMin() const { return xRange_.Min(); }
    qreal GetXAxisMax() const { return xRange_.Max(); }
    qreal GetYAxisMin() const { return yRange_.Min(); }
    qreal GetYAxisMax() const { return yRange_.Max(); }
    qreal GetXAxisMinOverride() const { return xAxisMinOverride_; }
    qreal GetXAxisMaxOverride() const { return xAxisMaxOverride_; }
    qreal GetYAxisMinOverride() const { return yAxisMinOverride_; }
    qreal GetYAxisMaxOverride() const { return yAxisMaxOverride_; }
    size_t GetPlotDataPointCount() const { return plotDataPointCount_; }
    void ForEachPlot(const std::function<void(const Plot& plot, size_t plotIndex)>& action) const;

    void AddPlot(QRgb colour, QString name);
    void AddPoint(size_t index, qreal x, qreal y);
    void SetPlotHidden(size_t plotIndex, bool hidden);
    void Reset();

public slots:
    void EnableXAxisMinOverride(bool override) { xAxisMinOverride_ = override; update(); }
    void EnableXAxisMaxOverride(bool override) { xAxisMaxOverride_ = override; update(); }
    void EnableYAxisMinOverride(bool override) { yAxisMinOverride_ = override; update(); }
    void EnableYAxisMaxOverride(bool override) { yAxisMaxOverride_ = override; update(); }
    void SetXAxisMinOverride(qreal value) { xAxisMinOverrideValue_ = value; update(); }
    void SetXAxisMaxOverride(qreal value) { xAxisMaxOverrideValue_ = value; update(); }
    void SetYAxisMinOverride(qreal value) { yAxisMinOverrideValue_ = value; update(); }
    void SetYAxisMaxOverride(qreal value) { yAxisMaxOverrideValue_ = value; update(); }
    void SetPlotDataPointCount(size_t count);

protected:
    virtual void paintEvent(QPaintEvent* event) override final;

private:
    EoBE::MinMax<qreal> xRange_;
    EoBE::MinMax<qreal> yRange_;
    QString xAxisLabel_;
    QString yAxisLabel_;
    std::vector<Plot> plots_;
    size_t plotDataPointCount_;
    bool xAxisMinOverride_ = false;
    bool xAxisMaxOverride_ = false;
    bool yAxisMinOverride_ = false;
    bool yAxisMaxOverride_ = false;
    qreal xAxisMinOverrideValue_ = 0.0;
    qreal xAxisMaxOverrideValue_ = 0.0;
    qreal yAxisMinOverrideValue_ = 0.0;
    qreal yAxisMaxOverrideValue_ = 0.0;

    QPointF PaintAxes(QPainter& painter) const;
    void PaintKey(QPainter& painter) const;
};



#endif // BARGRAPH_H
