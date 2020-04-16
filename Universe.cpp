#include "Universe.h"

#include "Swimmer.h"
#include "FeedDispenser.h"
#include "FoodPellet.h"
#include "Random.h"
#include "MainWindow.h"

//RenderSettings struct to contain such as showQuads, showSenseRanges, showPole, showFoodSpawnArea
//If sticking with QT, consider spawning seperate thread for sim and leaving rendering on QT thread, ensure data races only over trivial things like where to draw things, so Render thread is read only, mutex over settings for obvious reasons

Universe::Universe(QWidget* parent)
    : QWidget(parent)
    , energy_(EnergyPool::GetGlobalEnergyPool())
    , rootNode_({0, 0, 1000, 1000})
{
    setFocusPolicy(Qt::StrongFocus);

    unsigned count = 50;
    double xRange = 1500;
    double yRange = 1500;
    uint64_t maxPellets = energy_.Quantity() / 25_mj;
    for (unsigned i = 0; i < count; i++) {
        double x = Random::Number(-xRange / 2, xRange / 2);
        double y = Random::Number(-yRange / 2, yRange / 2);
        feedDispensers_.emplace_back(energy_, rootNode_, x, y, Random::Number(50, 100), maxPellets / count, Random::Number(0, 5));
    }

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
    timer->start(1);

    rootNode_.SetEntityCapacity(25, 5);
}

void Universe::SetupGraphs(LineGraph& graph)
{
    connect(this, &Universe::OnFoodEnergySampled, [&](double energy) { graph.PushBack(QColor(0, 200, 0).rgb(), energy); });
    connect(this, &Universe::OnSwimmerEnergySampled, [&](double energy) { graph.PushBack(QColor(100, 100, 255).rgb(), energy); });
    connect(this, &Universe::OnGraphResetRequested, [&]() { graph.Reset(); });
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

void Universe::keyPressEvent(QKeyEvent* event)
{
    switch (event->key()) {
    case Qt::Key_Space :
        pauseSim_ = !pauseSim_;
        break;
    case Qt::Key_F :
        spawnFood_ = !spawnFood_;
        break;
    case Qt::Key_R :
        respawn_ = true;
        break;
    case Qt::Key_G :
        emit OnGraphResetRequested();
        break;
    }
}

void Universe::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setBackground(QColor(200, 225, 255));
    p.drawText(0, 15, "Entities:    " + QString::number(rootNode_.EntityCount()));
    p.drawText(0, 30, "SpareEnergy (mj): " + QString::number(energy_.Quantity() / 1_mj));
    p.drawText(0, 45, "Paused (space): " + QVariant(pauseSim_).toString());
    p.drawText(0, 60, "Spawn Food (F): " + QVariant(spawnFood_).toString());
    p.drawText(0, 75, "Respawn (R)");
    p.drawText(0, 90, "Reset Graph (G)");
    p.translate(simX_ + (width() / 2), simY_ + (height() / 2));
    p.scale(simScale_, simScale_);

    if (spawnFood_) {
        for (auto& dispenser : feedDispensers_) {
            dispenser.Draw(p);
        }
    }

    rootNode_.Draw(p);
}

void Universe::Thread()
{
    if (!pauseSim_) {
        rootNode_.Tick();
    }

    if (respawn_) {
        respawn_ = false;
        for (auto feeder : feedDispensers_) {
            double swimmerX = feeder.GetX() + Random::Number(-feeder.GetRadius(), feeder.GetRadius());
            double swimmerY = feeder.GetY() + Random::Number(-feeder.GetRadius(), feeder.GetRadius());
            rootNode_.AddEntity(std::make_shared<Swimmer>(energy_.CreateChild(300_mj), swimmerX, swimmerY));
        }
    }

    if (spawnFood_) {
        for (auto& dispenser : feedDispensers_) {
            dispenser.Tick();
        }
    }

    static uint64_t tick = 0;
    if (++tick % 100 == 0) {
        uint64_t foodEnergy = 0;
        uint64_t swimmerEnergy = 0;
        rootNode_.ForEach([&](const Entity& e) -> void
        {
            if (const auto* f = dynamic_cast<const FoodPellet*>(&e)) {
                foodEnergy += f->GetEnergy();
            } else if (const auto* s = dynamic_cast<const Swimmer*>(&e)) {
                swimmerEnergy += s->GetEnergy();
            }
        });
        emit OnFoodEnergySampled(foodEnergy);
        emit OnSwimmerEnergySampled(swimmerEnergy);
    }

    // QT paint call
    update();
}
