#include "SenseMagneticField.h"

#include "Swimmer.h"
#include "Utils.h"
#include "Shape.h"

#include <QPainter>

SenseMagneticField::SenseMagneticField(const std::shared_ptr<NeuralNetwork>& network, const std::shared_ptr<NeuralNetworkConnector>& outputConnections, const Swimmer& owner, const Point& target, const EoBE::RangeConverter& distanceNormaliser, const EoBE::Range<double>& frontBackNormaliser, const EoBE::Range<double>& leftRightNormaliser)
    : Sense(network, outputConnections, owner)
    , target_(target)
    , distanceNormaliser_(distanceNormaliser)
    , frontBackNormaliser_({0, EoBE::Pi}, frontBackNormaliser)
    , leftRightNormaliser_({0, EoBE::Pi}, leftRightNormaliser)
{
}

void SenseMagneticField::PrimeInputs(std::vector<double>& inputs, const EntityContainerInterface&) const
{
    double distance = GetDistance(Point{ owner_.GetTransform().x, owner_.GetTransform().y }, target_);
    double bearing = GetBearing(Point{ owner_.GetTransform().x, owner_.GetTransform().y }, target_);

    // Distance to the target location
    inputs.at(0) = distanceNormaliser_.ConvertAndClamp(distance);
    // 1 Facing target, -1 back to target (left/right both 0)
    inputs.at(1) = BearingToFrontBack(bearing);
    // 1 Left facing target, -1 right facing target (front/back both 0)
    inputs.at(2) = BearingToLeftRight(bearing);
}

void SenseMagneticField::Draw(QPainter& paint) const
{
    Point begin{ owner_.GetTransform().x, owner_.GetTransform().y };
    double distance = GetDistance(begin, target_);
    double bearing = GetBearing(begin, target_);

    // Adjust distance so it can be drawn reasonably
    distance = 30 * (distance / distanceNormaliser_.GetFrom().Max());

    Point end = ApplyOffset(begin, bearing, distance);

    paint.setPen(Qt::blue);
    paint.drawLine(QLineF(begin.x, begin.y, end.x, end.y));
}

double SenseMagneticField::BearingToFrontBack(double bearing) const
{
    // Get bearing from direction facing to target
    bearing -= owner_.GetTransform().rotation;

    // Ensure we are still in the range if 0 to Tau
    if (bearing < 0.0) {
        bearing += EoBE::Tau;
    }

    // Convert range from 0-Tau to 0 to Pi
    bearing = std::abs(bearing - EoBE::Pi);

    return frontBackNormaliser_.Convert(bearing);
}

double SenseMagneticField::BearingToLeftRight(double bearing) const
{
    // Get bearing from right side to target
    bearing -= owner_.GetTransform().rotation + (EoBE::Tau / 4.0);

    // Ensure we are still in the range if 0 to Tau
    while (bearing < 0.0) {
        bearing += EoBE::Tau;
    }

    // Convert range from 0-Tau to 0 to Pi
    bearing = std::abs(bearing - EoBE::Pi);

    return leftRightNormaliser_.Convert(bearing);
}
