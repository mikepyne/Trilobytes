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

void NeuralNetworkInspector::SetSwimmer(std::shared_ptr<Swimmer> toInspect)
{
    if (inspectedSwimmer_ != toInspect) {
        inspectedSwimmer_ = toInspect;
        sensorGroups_.clear();
        effectorGroups_.clear();
        brainGroup_ = {};

        if (inspectedSwimmer_ != nullptr) {
            if (inspectedSwimmer_->InspectBrain()) {
                brainGroup_ = CreateGroup(*inspectedSwimmer_->InspectBrain(), "Brain");
            }

            for (auto& sense : inspectedSwimmer_->InspectSenses()) {
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
                            senseGroup.horizontalNodes = sense->InspectConnection().GetInputCount();
                            senseGroup.verticalNodes = 1;
                            senseGroup.nodes[{ 0, 0 }];
                        }
                        Node& node = senseGroup.nodes[{ neuronIndex, senseGroup.verticalNodes - 1 }];
                        if (brainGroup_.horizontalNodes > 0 && brainGroup_.verticalNodes > 0) {
                            node.connections.push_back({ brainGroup_.nodes[{ weightIndex, 0 }], weight });
                        }
                        node.value = 0;
                        ++weightIndex;
                    }
                    ++neuronIndex;
                }
            }

            for (auto& effector : inspectedSwimmer_->InspectEffectors()) {
                effectorGroups_.push_back(CreateGroup(effector->Inspect(), std::string(effector->GetName())));
                Group& effectorGroup = effectorGroups_.back();

                unsigned neuronIndex = 0;
                for (auto& neuron : effector->InspectConnection().Inspect()) {
                    unsigned weightIndex = 0;
                    for (auto& weight : neuron) {
                        /*
                 * Some effectors are simple passthroughs and do not have any
                 * neurons.
                 */
                        if (effectorGroup.nodes.empty()) {
                            effectorGroup.horizontalNodes = effector->InspectConnection().GetOutputCount();
                            effectorGroup.verticalNodes = 1;
                            effectorGroup.nodes[{ 0, 0 }];
                        }
                        if (brainGroup_.horizontalNodes > 0 && brainGroup_.verticalNodes > 0) {
                            Node& node = brainGroup_.nodes.at({ neuronIndex, brainGroup_.verticalNodes - 1 });
                            node.connections.push_back({ effectorGroup.nodes[{ weightIndex, 0 }], weight });
                        }
                        effectorGroup.nodes[{ weightIndex, 0 }].value = 0;
                        ++weightIndex;
                    }
                    ++neuronIndex;
                }
            }
        }
        LayoutGroupsInView();
        update();
    }
}

void NeuralNetworkInspector::UpdateConnectionStrengths(EntityContainerInterface& container)
{
    if (inspectedSwimmer_ && liveUpdate_) {
        // zeroise every input value everywhere
        ForEachGroup([](GroupType /*type*/, Group& group)
        {
            for (auto& [ location, node ] : group.nodes) {
                (void) location; // unused
                node.value = 0.0;
            }
        });

        EoBE::IterateBoth<std::shared_ptr<Sense>, Group>(inspectedSwimmer_->InspectSenses(), sensorGroups_, [&](const std::shared_ptr<Sense>& sense, NeuralNetworkInspector::Group& senseGroup)
        {
            std::vector<double> inputs(senseGroup.horizontalNodes);
            sense->PrimeInputs(inputs, container);
            for (unsigned x = 0; x < senseGroup.horizontalNodes; ++x) {
                senseGroup.nodes.at({ x, 0 }).value = inputs.at(x);
            }
        });
        ForwardPropogate();
        update();
    }
}

void NeuralNetworkInspector::SetUpdateLive(bool liveUpdate)
{
    liveUpdate_ = liveUpdate;
    update();
}

void NeuralNetworkInspector::ResetViewTransform()
{
    qreal sensorsWidth = sensorGroups_.empty() ? 0.0 : sensorGroups_.back().area.right() - sensorGroups_.front().area.left();
    qreal brainWidth = brainGroup_.area.width();
    qreal effectorsWidth = effectorGroups_.empty() ? 0.0 : effectorGroups_.back().area.right() - effectorGroups_.front().area.left();

    transformScale_ = width() / std::max({ sensorsWidth, brainWidth, effectorsWidth });
    transformX_ = 0.0;
    transformY_ = 0.0;
    update();
}

void NeuralNetworkInspector::wheelEvent(QWheelEvent* event)
{
    double d = 1.0 + (0.001 * double(event->angleDelta().y()));
    transformScale_ *= d;
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
        transformX_ += ((event->x() - dragX_) / transformScale_);
        transformY_ += ((event->y() - dragY_) / transformScale_);
        dragX_ = event->x();
        dragY_ = event->y();
        // QT paint call
        update();
    }
}

void NeuralNetworkInspector::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setClipRegion(event->region());
    p.translate(width() / 2, height() / 2);
    p.scale(transformScale_, transformScale_);
    p.translate(transformX_, transformY_);

    // TODO use the type of group for something
    ForEachGroup([&](GroupType /*type*/, const Group& group)
    {
        // Draw the connections
        for (auto& [ nodeCoordinates, node ] : group.nodes) {
            (void) nodeCoordinates; // unused
            p.setBrush(Qt::BrushStyle::NoBrush);
            for (auto& [ target, strength ] : node.connections) {
                qreal penWidth = (std::abs(strength) / 1.5) * 8;
                if (penWidth * transformScale_ >= 0.75) {
                    unsigned opacity = liveUpdate_ ? std::clamp(std::abs(node.value) * 200, 0.0, 255.0) : 255.0;
                    QPen pen(QColor(strength > 0 ? 0 : 255, 0, 0, opacity));
                    pen.setWidthF(penWidth);
                    p.setPen(pen);

                    p.drawLine(QPointF{ node.x, node.y }, QPointF{ target.get().x, target.get().y });
                }
            }
        }

        // Draw the Nodes over the top
        for (auto& [ nodeCoordinates, node ] : group.nodes) {
            (void) nodeCoordinates; // unused
            // Draw the Node
            QColor nodeCol(liveUpdate_ ? (node.value > 0 ? Qt::blue : Qt::red) : Qt::magenta);
            qreal transparency = liveUpdate_ ? std::abs(node.value) : 1.0;
            p.setPen(QPen(nodeCol, 3.0));
            nodeCol.setAlphaF(std::min(1.0, transparency));
            p.setBrush(nodeCol);
            // scale /should/ be 1.0, if it isn't something needs normalising!
            double scale = std::max(1.0, transparency);
            p.drawEllipse(QPointF(node.x, node.y), (nodeWidth_ / 2) * scale, (nodeHeight_ / 2) * scale);
        }

        // Draw last so can be seen over over stimulated neurons
        p.setPen(Qt::black);
        p.setBrush(Qt::NoBrush);
        p.drawRect(group.area);
        p.setCompositionMode(QPainter::CompositionMode_Exclusion);
        p.setPen(Qt::white);
        p.drawText(group.area.topLeft() + QPoint(2, 10), QString(group.name.c_str()));
        p.setCompositionMode(QPainter::CompositionMode_SourceOver);
    });
}

NeuralNetworkInspector::Group NeuralNetworkInspector::CreateGroup(const NeuralNetwork& network, const std::string& name)
{
    Group group = { name, rect(), {}, 0, 0};

    network.ForEach([&](unsigned x, unsigned y, const NeuralNetwork::Node& node)
    {
        /*
         * NeuralNetwork::Nodes are simply an array of input weights, from the
         * previous layer, but we want actual nodes for all layers, including
         * the input layer.
         */
        unsigned inputX = 0;
        unsigned inputY = y - 1;
        for (const auto& connectionWeight : node) {
            group.nodes[{ inputX, inputY }].connections.push_back({ group.nodes[{ x, y }], connectionWeight });
            group.nodes.insert({ { x, y }, { 0, 0, 0, {}} }); // Ensure even nodes without connections are added
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

void NeuralNetworkInspector::LayoutGroupsInView()
{
    /*
     * The aim here is to give each group an appropriate size based on its
     * content, then to arrange them so that senses are at the top,  the brain
     * in the middle and effectors at the bottom.
     */

    qreal totalSenseWidth = (sensorGroups_.size() * groupHSpacing_) - groupHSpacing_;
    qreal totalEffectorWidth = (effectorGroups_.size() * groupHSpacing_) - groupHSpacing_;

    qreal maxSenseHeight = 0.0;
    qreal maxEffectorHeight = 0.0;

    QFontMetrics metrics = fontMetrics();

    ForEachGroup([&](GroupType type, Group& group)
    {
        qreal titleWidth = metrics.width(QString(group.name.c_str())) + 10;
        qreal nodesWidth = (2 * groupPadding_) + (group.horizontalNodes * (nodeWidth_ + nodeHSpacing_)) - nodeHSpacing_;

        qreal titleHeight = metrics.height();
        qreal nodesHeight = (2 * groupPadding_) + (group.verticalNodes * (nodeHeight_ + nodeVSpacing_) - nodeVSpacing_);

        group.area.setSize(QSizeF(std::max(titleWidth, nodesWidth), titleHeight + nodesHeight));

        if (type == GroupType::Sense) {
            totalSenseWidth += group.area.width();
            maxSenseHeight = std::max(maxSenseHeight, group.area.height());
        } else if (type == GroupType::Effector) {
            totalEffectorWidth += group.area.width();
            maxEffectorHeight = std::max(maxEffectorHeight, group.area.height());
        }
    });

    /*
     * Centre everything around {0, 0}, senses at the top, effectors at the bottom
     */

    QRectF sensesRect(-totalSenseWidth / 2, -(maxSenseHeight + (groupVSpacing_ + brainGroup_.area.height() / 2)), totalSenseWidth, maxSenseHeight);
    qreal sensorsWidthSoFar = 0.0;
    for (auto& sense : sensorGroups_) {
        sense.area.moveTopLeft(sensesRect.topLeft());
        sense.area.translate(sensorsWidthSoFar, (maxSenseHeight - sense.area.height()) / 2.0);
        sensorsWidthSoFar += sense.area.width() + groupHSpacing_;
    }

    brainGroup_.area.moveTopLeft(QPointF(-brainGroup_.area.width() / 2.0, -(brainGroup_.area.height() / 2)));

    QRectF effectorsRect(-totalEffectorWidth / 2, (brainGroup_.area.height() / 2) + groupVSpacing_, totalEffectorWidth, maxEffectorHeight);
    qreal effectorsWidthSoFar = 0.0;
    for (auto& effector : effectorGroups_) {
        effector.area.moveTopLeft(effectorsRect.topLeft());
        effector.area.translate(effectorsWidthSoFar, (maxEffectorHeight - effector.area.height()) / 2.0);
        effectorsWidthSoFar += effector.area.width() + groupHSpacing_;
    }

    ForEachGroup([&](GroupType /*type*/, Group& group)
    {
        LayoutNodesInGroup(group);
    });

    ResetViewTransform();
}

void NeuralNetworkInspector::LayoutNodesInGroup(NeuralNetworkInspector::Group& group)
{
    for (auto& [ nodeCoordinates, node ] : group.nodes) {
        auto& [column, row] = nodeCoordinates; {}
        qreal totalNodesWidth  = (nodeWidth_ + nodeHSpacing_) * (group.horizontalNodes - 1);
        qreal totalNodesHeight = (nodeHeight_ + nodeVSpacing_) * (group.verticalNodes - 1);
        qreal xOffset = (group.area.width() - totalNodesWidth) / 2;
        qreal yOffset = (((group.area.height() - fontMetrics().height()) - totalNodesHeight) / 2) + fontMetrics().height();
        node.x = group.area.x() + xOffset + ((nodeWidth_ + nodeHSpacing_) * column);
        node.y = group.area.y() + yOffset + ((nodeHeight_ + nodeVSpacing_) * row);
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

void NeuralNetworkInspector::ForEachGroup(std::function<void (GroupType type, Group& group)>&& action)
{
    for (auto& group : sensorGroups_) {
        action(GroupType::Sense, group);
    }

    action(GroupType::Brain, brainGroup_);

    for (auto& group : effectorGroups_) {
        action(GroupType::Effector, group);
    }}

void NeuralNetworkInspector::ForEachGroup(std::function<void (GroupType type, const Group&)>&& action) const
{
    for (const auto& group : sensorGroups_) {
        action(GroupType::Sense, group);
    }

    action(GroupType::Brain, brainGroup_);

    for (const auto& group : effectorGroups_) {
        action(GroupType::Effector, group);
    }
}
