#include "NeuralNetworkInspector.h"

#include "Swimmer.h"
#include "EntityContainerInterface.h"
#include "NeuralNetwork.h"

#include <QMouseEvent>
#include <QPainter>

NeuralNetworkInspector::NeuralNetworkInspector(QWidget* parent)
    : QWidget(parent)
{
}

void NeuralNetworkInspector::SetSwimmer(Swimmer& toInspect, EntityContainerInterface& container)
{
    sensorGroups_.clear();
    effectorGroups_.clear();
    brainGroup_ = {};

    // TODO low priority, make groups movable and resizable and hideable

    brainGroup_ = CreateGroup(toInspect.InspectBrain(), "Brain");

    for (auto& sense : toInspect.InspectSenses()) {
        sensorGroups_.push_back(CreateGroup(sense->Inspect(), std::string(sense->GetName())));
        Group& senseGroup = sensorGroups_.back();

        unsigned neuronIndex = 0;
        for (auto& neuron : sense->InspectConnection().Inspect()) {
            unsigned weightIndex = 0;
            for (auto& weight : neuron) {
                /*
                 * Some senses are simple passthroughs and do not have any
                 * neurons.
                 */
                if (senseGroup.nodes.empty()) {
                    senseGroup.horizontalNodes = sense->InspectConnection().Inspect().size();
                    senseGroup.verticalNodes = 1;
                    senseGroup.nodes[{ 0, 0 }];
                }
                Node& node = senseGroup.nodes[{ neuronIndex, senseGroup.verticalNodes - 1 }];
                node.connections.push_back({ brainGroup_.nodes[{ weightIndex, 0 }], weight });
                node.value = 0;
                ++weightIndex;
            }
            ++neuronIndex;
        }

        std::vector<double> inputs(senseGroup.horizontalNodes);
        sense->PrimeInputs(inputs, container, {});
        for (unsigned x = 0; x < senseGroup.horizontalNodes; ++x) {
            senseGroup.nodes[{ x, 0 }].value = inputs.at(x);
        }
    }

    // TODO add the effectors, and connect to brain

    ForwardPropogate();
    LayoutGroups();
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

    for (const auto& group : sensorGroups_) {
        PaintGroup(group, paint);
    }

    PaintGroup(brainGroup_, paint);

}

void NeuralNetworkInspector::resizeEvent(QResizeEvent* /*event*/)
{
    LayoutGroups();
}

NeuralNetworkInspector::Group NeuralNetworkInspector::CreateGroup(const NeuralNetwork& network, const std::string& name)
{
    Group group = { name, rect(), {}, 0, 0};

    network.ForEach([&](unsigned x, unsigned y, const NeuralNetwork::Node& node)
    {
        /*
         * NeuralNetwork::Nodes are simply an array of input weights, from the previous layer, but we want actual nodes for all layers, including the input layer
         */
        unsigned inputX = 0;
        unsigned inputY = y - 1;
        for (const auto& connectionWeight : node) {
            group.nodes[{ inputX, inputY }].connections.push_back({ group.nodes[{ x, y }], connectionWeight });
            group.nodes[{ x, y }]; // Ensure even nodes without connections are added
            inputX++;
        }
    });

    group.horizontalNodes = 0;
    group.verticalNodes = 0;
    for (const auto& pair : group.nodes) {
        auto& [ x, y ] = pair.first; {}
        group.horizontalNodes = std::max(group.horizontalNodes, x + 1);
        group.verticalNodes = std::max(group.verticalNodes, y + 1);
    }

    return group;
}

void NeuralNetworkInspector::LayoutGroups()
{
    if (width() != 0 && height() != 0) {
        QRect screen = rect();
        LayoutGroup(brainGroup_, screen.adjusted(5, (height() / 2) + 5, -5, -5));

        unsigned index = 0;
        for (auto& group : sensorGroups_) {
            qreal senseWidth = static_cast<qreal>(width() / sensorGroups_.size());
            LayoutGroup(group, screen.adjusted((index * senseWidth) + 5, 5, -(screen.width() - (senseWidth * (index + 1))) - 5, (-height() / 2) - 5));
            ++index;
        }
    }
}

void NeuralNetworkInspector::LayoutGroup(NeuralNetworkInspector::Group& group, QRect area)
{
    group.area = area;
    for (auto& [ nodeCoordinates, node ] : group.nodes) {
        auto& [column, row] = nodeCoordinates;{}
        node.x = group.area.x() + ((group.area.width() / (group.horizontalNodes + 1)) * (column + 1));
        node.y = group.area.y() + ((group.area.height() / (group.verticalNodes + 1)) * (row + 1));
    }
}

void NeuralNetworkInspector::PaintGroup(const Group& group, QPainter& p) const
{
    p.setPen(Qt::black);
    p.setBrush(Qt::NoBrush);
    p.drawRect(group.area);p.setClipRect(rect());

    p.drawText(group.area.topLeft() + QPoint(2, 10), QString(group.name.c_str()));

    // Draw the connections
    for (auto& [ nodeCoordinates, node ] : group.nodes) {
        (void) nodeCoordinates; // unused
        p.setBrush(Qt::BrushStyle::NoBrush);
        for (auto& [ target, strength ] : node.connections) {
            unsigned opacity = std::clamp(std::abs(node.value) * 200, 0.0, 255.0);
            QPen pen(QColor(strength > 0 ? 0 : 255, 0, 0, opacity));
            pen.setWidthF((std::abs(strength) / 1.5) * 10);
            p.setPen(pen);

            p.drawLine(QPointF{ node.x, node.y }, QPointF{ target.get().x, target.get().y });
        }
    }

    // Draw the Nodes over the top
    for (auto& [ nodeCoordinates, node ] : group.nodes) {
        (void) nodeCoordinates; // unused
        // Draw the Node
        p.setPen(Qt::PenStyle::NoPen);
        p.setBrush(QColor(25, 25, 255));
        double scale = node.value / 2;
        p.drawEllipse(QPointF(node.x, node.y), 15.0 * scale, 15.0 * scale);
    }
}

void NeuralNetworkInspector::ForwardPropogate()
{
    auto propogateGroup = [](Group& group)
    {
        for (unsigned y = 0; y < group.verticalNodes; ++y) {
            for (unsigned x = 0; x < group.horizontalNodes; ++x) {
                Node& node = group.nodes.at({ x, y });
                if (y != 0) {
                    // Don't normalise inputs
                    node.value = std::tanh(node.value);
                }
                for (auto& [ connectedNode, strength ] : node.connections) {
                    connectedNode.get().value += node.value * strength;
                }
            }
        }
    };

    for (auto& group : sensorGroups_) {
        propogateGroup(group);
    }
    propogateGroup(brainGroup_);
}
