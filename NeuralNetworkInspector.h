#ifndef NEURALNETWORKINSPECTOR_H
#define NEURALNETWORKINSPECTOR_H

#include "Shape.h"

#include <QWidget>

#include <memory>

class Swimmer;
class NeuralNetwork;
class EntityContainerInterface;

class NeuralNetworkInspector : public QWidget {
    Q_OBJECT
public:
    explicit NeuralNetworkInspector(QWidget *parent);

    void SetSwimmer(Swimmer& toInspect, EntityContainerInterface& container);

protected:
    virtual void mousePressEvent(QMouseEvent* event) override;
    virtual void mouseReleaseEvent(QMouseEvent* event) override;
    virtual void mouseMoveEvent(QMouseEvent* event) override;
    virtual void wheelEvent(QWheelEvent* event) override;
    virtual void paintEvent(QPaintEvent* event) override;
    virtual void resizeEvent(QResizeEvent* event) override;

private:
    struct Node {
        double x;
        double y;
        double value;
        // Nodes are arranged from 0 -> 1 on the x & y axis
        std::vector<std::pair<std::reference_wrapper<Node>, double>> connections;
    };

    struct Group {
        std::string name;
        QRect area;
        std::map<std::pair<unsigned, unsigned>, Node> nodes;
        unsigned horizontalNodes;
        unsigned verticalNodes;
    };

    std::vector<Group> sensorGroups_;
    Group brainGroup_;
    std::vector<Group> effectorGroups_;

    double viewX_ = 0.0;
    double viewY_ = 0.0;
    double viewScale_ = 1.0;
    double dragX_;
    double dragY_;
    bool dragging_ = false;

    Group CreateGroup(const NeuralNetwork& network, const std::string& name);

    void LayoutGroups();
    void LayoutGroup(Group& group, QRect area);
    void PaintGroup(const Group& group, QPainter& p) const;

    void ForwardPropogate();
};

#endif // NEURALNETWORKINSPECTOR_H
