#ifndef NEURALNETWORKINSPECTOR_H
#define NEURALNETWORKINSPECTOR_H

#include "Swimmer.h"

#include <QWidget>

#include <memory>

class NeuralNetworkInspector : public QWidget {
    Q_OBJECT
public:
    explicit NeuralNetworkInspector(QWidget *parent);

    void SetSwimmer(Swimmer& toInspect);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;

    virtual void paintEvent(QPaintEvent* event) override;

private:
    struct Node {
        unsigned x;
        // Nodes are arranged from 0 -> 1 on the x & y axis
        std::vector<std::pair<std::pair<unsigned, unsigned>, double>> connections;
    };

    struct Group {
        std::vector<std::vector<Node>> nodes;
    };

    std::map<std::pair<unsigned, unsigned>, Node> nodes_;

    unsigned horizontalNodes_ = 0;
    unsigned verticalNodes_ = 0;

    double viewX_ = 0.0;
    double viewY_ = 0.0;
    double viewScale_ = 1.0;
    double dragX_;
    double dragY_;
    bool dragging_ = false;

    void PaintNetwork(QRect area, QPainter& p);

};

#endif // NEURALNETWORKINSPECTOR_H
