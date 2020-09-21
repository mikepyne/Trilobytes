#include "NeuralNetworkInspector.h"

#include <QMouseEvent>
#include <QPainter>

NeuralNetworkInspector::NeuralNetworkInspector(QWidget* parent)
    : QWidget(parent)
{
}

void NeuralNetworkInspector::SetSwimmer(Swimmer& toInspect)
{
    nodes_.clear();
    toInspect.InspectBrain().ForEach([&](unsigned x, unsigned y, const NeuralNetwork::Node& node)
    {
        /*
         * NeuralNetwork::Nodes are simply an array of input weights, from the previous layer, but we want actual nodes for all layers, including the input layer
         */
        unsigned inputX = 0;
        unsigned inputY = y - 1;
        for (const auto& connectionWeight : node) {
            nodes_[{ inputX, inputY }].connections.push_back({ { x, y }, connectionWeight });
            nodes_[{ x, y }].x = x + (x * y); // Add nodes that connections go to
            inputX++;
        }
    });

    horizontalNodes_ = 0;
    verticalNodes_ = 0;
    for (const auto& node : nodes_) {
        const auto& [ x, y ] = node.first;{}
        horizontalNodes_ = std::max(horizontalNodes_, x + 1);
        verticalNodes_ = std::max(verticalNodes_, y + 1);
    }

    // add the senses

    // add the effectors

    update();
}

void NeuralNetworkInspector::wheelEvent(QWheelEvent* event)
{
    double d = 1.0 + (0.001 * double(event->angleDelta().y()));
    viewScale_ *= d;
    viewX_ *= d;
    viewY_ *= d;
    // QT paint call
    update();
}

void NeuralNetworkInspector::mouseReleaseEvent(QMouseEvent*)
{
    dragging_ = false;
}

void NeuralNetworkInspector::mousePressEvent(QMouseEvent* event)
{
    dragging_ = true;
    dragX_ = event->x();
    dragY_ = event->y();
}

void NeuralNetworkInspector::mouseMoveEvent(QMouseEvent* event)
{
    if (dragging_) {
        viewX_ += (event->x() - dragX_);
        viewY_ += (event->y() - dragY_);
        dragX_ = event->x();
        dragY_ = event->y();
        // QT paint call
        update();
    }
}

void NeuralNetworkInspector::paintEvent(QPaintEvent* /*event*/)
{
    QPainter paint(this);
    paint.translate(viewX_ + (width() / 2), viewY_ + (height() / 2));
    paint.scale(viewScale_, viewScale_);

    PaintNetwork(rect(), paint);
}

void NeuralNetworkInspector::PaintNetwork(QRect area, QPainter& p)
{
    // Draw the connections
    for (auto& nodePair : nodes_) {
        auto& [ xIndex, yIndex ] = nodePair.first;{}
        auto& node = nodePair.second;
        qreal nodeX = area.x() + ((area.width() / (horizontalNodes_ + 1)) * xIndex);
        qreal nodeY = area.y() + ((area.height() / (verticalNodes_ + 1)) * yIndex);

        p.setBrush(Qt::BrushStyle::NoBrush);
        for (auto& [ target, strength ] : node.connections) {
            QPen pen(QColor(strength > 0 ? 0 : 255, 0, 0));
            pen.setWidthF((std::abs(strength) / 1.5) * 10);
            p.setPen(pen);

            auto& [ targetIndexX, targetIndexY ] = target;{}
            qreal targetX = area.x() + ((area.width() / (horizontalNodes_ + 1)) * targetIndexX);
            qreal targetY = area.y() + ((area.height() / (verticalNodes_ + 1)) * targetIndexY);
            p.drawLine(QPointF{ nodeX, nodeY }, QPointF{ targetX, targetY });
        }
    }

    // Draw the Nodes over the top
    for (auto& nodePair : nodes_) {
        auto& [ xIndex, yIndex ] = nodePair.first;{}
        qreal nodeX = area.x() + ((area.width() / (horizontalNodes_ + 1)) * xIndex);
        qreal nodeY = area.y() + ((area.height() / (verticalNodes_ + 1)) * yIndex);

        // Draw the Node
        p.setPen(Qt::PenStyle::NoPen);
        p.setBrush(QColor(25, 25, 255));
        p.drawEllipse(QPointF(nodeX, nodeY), 15.0, 15.0);

        p.setPen(Qt::PenStyle::SolidLine);
        p.setBrush(QColor(Qt::black));
        p.drawText(QPointF(nodeX + 15, nodeY), QString("%1, %2, %3").arg(nodeX).arg(nodeY).arg(nodePair.second.x));
    }
}
