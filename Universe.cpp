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
    , rootNode_({0, 0, 1000, 1000})
{
    setFocusPolicy(Qt::StrongFocus);

    feedDispensers_.emplace_back(rootNode_,  1000, 0, 950, 0.001);
    feedDispensers_.emplace_back(rootNode_, -1000, 0, 950, 0.001);

    Point startLocation = TransformSimToLocalCoords({ feedDispensers_.front().GetX(), feedDispensers_.front().GetY() });
    simX_ = startLocation.x;
    simY_ = startLocation.y;

    /*
     * QT hack to get this running in the QT event loop (necessary for
     * drawing anything, not ideal for running the Sim quickly...)
     */
    QTimer* timer = new QTimer(this);
    timer->setSingleShot(false);
    timer->connect(timer, &QTimer::timeout, [&](auto) -> void
    {
        this->Thread();
    });
    timer->start(0);

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
}

void Universe::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::MiddleButton) {
        dragging_ = false;
    }
}

void Universe::mousePressEvent(QMouseEvent* event)
{
    if (event->button() == Qt::LeftButton || event->button() == Qt::MiddleButton) {
        dragging_ = true;
        dragX_ = event->x();
        dragY_ = event->y();
    } else if (event->button() == Qt::RightButton) {
        Point simLocation = TransformLocalToSimCoords({ static_cast<double>(event->x()), static_cast<double>(event->y()) });
        rootNode_.ForEachCollidingWith(simLocation, [&](const std::shared_ptr<Entity>& e)
        {
            if (Swimmer* swimmer = dynamic_cast<Swimmer*>(e.get()); swimmer != nullptr) {
                MainWindow* mainWindow = dynamic_cast<MainWindow*>(parentWidget()->parentWidget());
                assert(mainWindow);
                mainWindow->SetSwimmerToInspect(*swimmer, rootNode_.GetContainer());
            }
        });
    }
}

void Universe::mouseMoveEvent(QMouseEvent* event)
{
    if (dragging_) {
        simX_ += ((event->x() - dragX_) / simScale_);
        simY_ += ((event->y() - dragY_) / simScale_);
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
    case Qt::Key_K :
        followFittest_ = !followFittest_;
        if (!followFittest_) {
            following_ = {};
        }
        break;
    }
}

void Universe::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setBackground(QColor(200, 225, 255));
    p.drawText(0, 15, "Entities:    " + QString::number(rootNode_.EntityCount()));
    p.drawText(0, 45, "Paused (space): " + QVariant(pauseSim_).toString());
    p.drawText(0, 60, "Spawn Food (F): " + QVariant(spawnFood_).toString());
    p.drawText(0, 75, "Respawn (R)");
    p.drawText(0, 90, "Reset Graph (G)");
    p.drawText(0, 105, "Find Fittest (K): " + QVariant(followFittest_).toString());
    if (followFittest_ && following_) {
        auto f = dynamic_cast<Swimmer*>(following_.get());
        p.drawText(0, 120, QString("   - Age [%1]").arg(f->GetAge()));
        p.drawText(0, 135, QString("   - Eggs Layed [%1]").arg(f->GetEggLayedCount()));
        p.drawText(0, 150, QString("   - Energy [%1]mj").arg(f->GetEnergy()));
    }

    p.translate(width() / 2, height() / 2);
    p.scale(simScale_, simScale_);
    p.translate(simX_, simY_);

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

        if (followFittest_) {
            unsigned mostEggsLayed = 0;
            rootNode_.ForEach([&](const std::shared_ptr<Entity>& e)
            {
                if (const auto* s = dynamic_cast<const Swimmer*>(e.get())) {
                    if (s->GetEggLayedCount() > mostEggsLayed) {
                        mostEggsLayed = s->GetEggLayedCount();
                        following_ = e;
                    }
                }
            });
        }

        if (following_ && following_->Alive()) {
            simX_ = -following_->GetLocation().x;
            simY_ = -following_->GetLocation().y;
            MainWindow* mainWindow = dynamic_cast<MainWindow*>(parentWidget()->parentWidget());
            assert(mainWindow);
            Swimmer& s = dynamic_cast<Swimmer&>(*following_.get());
            mainWindow->SetSwimmerToInspect(s, rootNode_.GetContainer());
        }

        if (respawn_) {
            respawn_ = false;
            for (auto feeder : feedDispensers_) {
                for (unsigned i = 0; i < std::max(1u, 25 / feedDispensers_.size()); i++) {
                    double rotation = Random::Number(0.0, EoBE::Tau);
                    double distance = std::sqrt(Random::Number(0.0, 1.0)) * feeder.GetRadius();
                    double swimmerX = feeder.GetX() + distance * std::cos(rotation);
                    double swimmerY = feeder.GetY() + distance * std::sin(rotation);
                    rootNode_.AddEntity(std::make_shared<Swimmer>(300_mj, Transform{ swimmerX, swimmerY, 0 }));
                }
            }
        }

        if (spawnFood_) {
            for (auto& dispenser : feedDispensers_) {
                dispenser.Tick();
            }
        }

        static uint64_t tick = 0;
        if (++tick % 100 == 0) {
            Energy foodEnergy = 0;
            Energy swimmerEnergy = 0;
            rootNode_.ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (const auto* f = dynamic_cast<const FoodPellet*>(e.get())) {
                    foodEnergy += f->GetEnergy();
                } else if (const auto* s = dynamic_cast<const Swimmer*>(e.get())) {
                    swimmerEnergy += s->GetEnergy();
                }
            });
            emit OnFoodEnergySampled(foodEnergy);
            emit OnSwimmerEnergySampled(swimmerEnergy);
        }
    }

    // QT paint call
    update();
}

Point Universe::TransformLocalToSimCoords(const Point& local)
{
    double x = local.x;
    double y = local.y;
    // Sim is centred on screen
    x -= (width() / 2);
    y -= (height() / 2);
    // Sim is scaled
    x /= simScale_;
    y /= simScale_;
    // Sim is transformed
    x -= simX_;
    y -= simY_;
    return { x, y };
}

Point Universe::TransformSimToLocalCoords(const Point& sim)
{
    double x = sim.x;
    double y = sim.y;
    // Sim is transformed
    x += simX_;
    y += simY_;
    // Sim is scaled
    x *= simScale_;
    y *= simScale_;
    // Sim is centred on screen
    x += (width() / 2);
    y += (height() / 2);
    return { x, y };
}
