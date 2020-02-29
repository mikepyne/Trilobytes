#include "Universe.h"

//Sensors and effectors need to Inherit "Inheritable" with each value they hold inheriting trait (like for compass, the distance from the pole that they consider the max distance for calculating the normalised distance value for their neural network) then if a trait must be instantiated with an inheritable and each IHtble gets instantiated with a genome, we can automatically save/load and mutate our variable collection of traits and such, and easily couple each trait with a mutable bool, or a rate of mutation, or a fixed range (these things either construct with a parent and a bool for mutate, handled in base class, or created from scratch with ability to set the fixed values for a particular trait/inheritable) each could generate unique sciency name "ECRGB-4XT" for example and register with a global stats object that monitors number and popularity of traits in the population (graphs =])

//Eggs need to have an ideal sperm genome similarity and choose a sperm that has been deposited at it with weighted probability based on the similarity, they also need traits like time to wait and ability to self fertilise if they need to

//RenderSettings struct to contain such as showQuads, showSenseRanges, showPole, showFoodSpawnArea

//Settings for sim tick cap, tranalate for x y scale (with hot keys) plant distribution, maximum system energy (perhaps ease this one down instead of having instantaneous famine?) perhaps have apply and ease options next to slider all with tooltips

//If sticking with QT, consider spawning seperate thread for sim and leaving rendering on QT thread, ensure data races only over trivial things like where to draw things, so Render thread is read only, mutex over settings for obvious reasons

Universe::Universe(QWidget* parent)
    : QWidget(parent)
    , rootNode_({0, 0, 1000, 1000})
{
    /*
     * QT hack to get this running in the QT event loop (necessary for
     * drawing anything, not ideal for running the Sim quickly...)
     *
     * Leaking this pointer... not intended to last long
     */
    QTimer* timer = new QTimer();
    timer->setSingleShot(false);
    timer->connect(timer, &QTimer::timeout, [&](auto) -> void
    {
        this->Thread();
    });
    timer->start(20);

    for (auto n : std::vector<double>(30, 0.0)) {
        rootNode_.AddEntity(std::make_shared<Swimmer>(Random::Real(-500.0, 500.0) + n, Random::Real(-500.0, 500.0)));
        energy_ -= 1.0;
    }

    rootNode_.SetEntityCapacity(25, 5);
}

void Universe::wheelEvent(QWheelEvent* event)
{
    double d = 1.0 + (0.001 * double(event->angleDelta().y()));
    simScale_ *= d;
    simX_ *= d;
    simY_ *= d;
}

void Universe::mouseReleaseEvent(QMouseEvent*)
{
    dragging_ = false;
}

void Universe::mousePressEvent(QMouseEvent* event)
{
    dragging_ = true;
    dragX_ = event->x();
    dragY_ = event->y();
}

void Universe::mouseMoveEvent(QMouseEvent* event)
{
    if (dragging_) {
        simX_ += (event->x() - dragX_);
        simY_ += (event->y() - dragY_);
        dragX_ = event->x();
        dragY_ = event->y();
    }
}

void Universe::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setBackground(QColor(200, 225, 255));
    p.drawText(0, 15, QString::number(rootNode_.EntityCount()));
    p.translate(simX_ + (width() / 2), simY_ + (height() / 2));
    p.scale(simScale_, simScale_);

    rootNode_.Draw(p);
}

void Universe::Thread()
{
    rootNode_.Tick();

    if (energy_ > 1) {
        double foodX = Random::Real(-500.0, 500.0);
        double foodY = Random::Real(-500.0, 500.0);
        rootNode_.AddEntity(std::make_shared<FoodPellet>(foodX, foodY));
        --energy_;
    }

    // QT paint call
    update();
}
