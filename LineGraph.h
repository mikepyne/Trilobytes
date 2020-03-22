#ifndef BARGRAPH_H
#define BARGRAPH_H

#include <QWidget>
#include <QPaintEvent>
#include <QPainter>

class LineGraph : public QWidget {
    Q_OBJECT
public:
    LineGraph(QWidget* parent);

    void PushBack(QRgb colour, double datum);
    void Reset();

protected:
    virtual void paintEvent(QPaintEvent* event) override;

private:
    std::map<QRgb, std::vector<double>> plots_;
};

#endif // BARGRAPH_H
