#include "Universe.h"

#include "Swimmer.h"
#include "FeedDispenser.h"
#include "FoodPellet.h"
#include "Random.h"
#include "MainWindow.h"

// TODO RenderSettings struct to contain such as showQuads, showSenseRanges, showPole, showFoodSpawnArea

Universe::Universe(QWidget* parent)
    : QWidget(parent)
    , rootNode_({0, 0, 1000, 1000})
{
    setFocusPolicy(Qt::StrongFocus);

    feedDispensers_.push_back(std::make_shared<FeedDispenser>(rootNode_,  1000, 0, 950, 0.001));
    feedDispensers_.push_back(std::make_shared<FeedDispenser>(rootNode_, -1000, 0, 950, 0.001));

    for (auto& feeder : feedDispensers_) {
        Point startLocation = TransformSimToLocalCoords({ feeder->GetX(), feeder->GetY() });
        simX_ = startLocation.x;
        simY_ = startLocation.y;
        feeder->AddPelletsImmediately(feeder->GetMaxPellets() / 8);
    }

    /*
     * QT hack to get this running in the QT event loop (necessary for
     * drawing anything, not ideal for running the Sim quickly...)
     */
    mainThread_.setSingleShot(false);
    mainThread_.connect(&mainThread_, &QTimer::timeout, this, &Universe::Thread);
    mainThread_.start(0);

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
        selectedEntity_.reset();
        Point simLocation = TransformLocalToSimCoords({ static_cast<double>(event->x()), static_cast<double>(event->y()) });
        rootNode_.ForEachCollidingWith(simLocation, [&](const std::shared_ptr<Entity>& e)
        {
            if (Swimmer* swimmer = dynamic_cast<Swimmer*>(e.get()); swimmer != nullptr) {
                MainWindow* mainWindow = dynamic_cast<MainWindow*>(parentWidget()->parentWidget());
                assert(mainWindow);
                selectedEntity_ = e;
                mainWindow->SetSwimmerToInspect(*swimmer, rootNode_.GetContainer());
                update();
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
    case Qt::Key_A :
        autoSelectFittest_ = !autoSelectFittest_;
        trackSelectedEntity_ = autoSelectFittest_;
        break;
    case Qt::Key_T :
        trackSelectedEntity_ = !trackSelectedEntity_;
        break;
    case Qt::Key_K :
        SelectFittestSwimmer();
        trackSelectedEntity_ = true;
        break;
    }
}


void Universe::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setBackground(QColor(200, 225, 255));

    p.save();
    p.translate(width() / 2, height() / 2);
    p.scale(simScale_, simScale_);
    p.translate(simX_, simY_);

    if (spawnFood_) {
        for (auto& dispenser : feedDispensers_) {
            dispenser->Draw(p);
        }
    }
    rootNode_.Draw(p);
    p.restore();

    p.setPen(Qt::black);
    int line = 0;
    p.drawText(0, line += 15, "Entities:    " + QString::number(lastEntityCount_));
    p.drawText(0, line += 15, "Swimmers:    " + QString::number(lastSwimmerCount_));
    p.drawText(0, line += 15, "Paused (space): " + QVariant(pauseSim_).toString());
    p.drawText(0, line += 15, "Spawn Food (F): " + QVariant(spawnFood_).toString());
    p.drawText(0, line += 15, "Respawn (R)");
    p.drawText(0, line += 15, "Reset Graph (G)");
    p.drawText(0, line += 15, "Select Fittest (K)");
    p.drawText(0, line += 15, "Auto Select Fittest (A): " + QVariant(autoSelectFittest_).toString());
    p.drawText(0, line += 15, "Track Selected (T): " + QVariant(trackSelectedEntity_).toString());
    if (selectedEntity_) {
        auto f = dynamic_cast<Swimmer*>(selectedEntity_.get());
        p.drawText(0, line += 15, QString("   - %1 Ticks Old").arg(f->GetAge()));
        p.drawText(0, line += 15, QString("   - Laid %1 Eggs").arg(f->GetEggsLayedCount()));
        p.drawText(0, line += 15, QString("   - Descendants %1/%2").arg(f->GetLivingDescendantsCount()).arg(f->GetTotalDescendantsCount()));
        p.drawText(0, line += 15, QString("   - Energy %1mj").arg(f->GetEnergy() / 1_mj));
    }
}

void Universe::Thread()
{
    if (!pauseSim_) {
        rootNode_.Tick();

        if (autoSelectFittest_) {
            SelectFittestSwimmer();
        }

        if (trackSelectedEntity_ && selectedEntity_ && selectedEntity_->Alive()) {
            simX_ = -selectedEntity_->GetLocation().x;
            simY_ = -selectedEntity_->GetLocation().y;
            MainWindow* mainWindow = dynamic_cast<MainWindow*>(parentWidget()->parentWidget());
            assert(mainWindow);
            Swimmer& s = dynamic_cast<Swimmer&>(*selectedEntity_.get());
            mainWindow->SetSwimmerToInspect(s, rootNode_.GetContainer());
        }

        if (respawn_) {
            respawn_ = false;
            for (auto feeder : feedDispensers_) {
                for (unsigned i = 0; i < std::max(1u, 25 / feedDispensers_.size()); i++) {
                    double rotation = Random::Number(0.0, EoBE::Tau);
                    double distance = std::sqrt(Random::Number(0.0, 1.0)) * feeder->GetRadius();
                    double swimmerX = feeder->GetX() + distance * std::cos(rotation);
                    double swimmerY = feeder->GetY() + distance * std::sin(rotation);
                    rootNode_.AddEntity(std::make_shared<Swimmer>(300_mj, Transform{ swimmerX, swimmerY, 0 }));
                }
            }
        }

        if (spawnFood_) {
            for (auto& dispenser : feedDispensers_) {
                dispenser->Tick();
            }
        }

        static uint64_t tick = 0;
        if (++tick % 100 == 0) {
            Energy foodEnergy = 0;
            Energy swimmerEnergy = 0;
            lastEntityCount_ = 0;
            lastSwimmerCount_ = 0;
            rootNode_.ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                ++lastEntityCount_;
                if (const auto* f = dynamic_cast<const FoodPellet*>(e.get())) {
                    foodEnergy += f->GetEnergy();
                } else if (const auto* s = dynamic_cast<const Swimmer*>(e.get())) {
                    ++lastSwimmerCount_;
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

void Universe::SelectFittestSwimmer() {
    unsigned mostLivingChildren = 0;
    rootNode_.ForEach([&](const std::shared_ptr<Entity>& e)
    {
        if (const auto* s = dynamic_cast<const Swimmer*>(e.get())) {
            if (s->GetLivingDescendantsCount() > mostLivingChildren) {
                mostLivingChildren = s->GetLivingDescendantsCount();
                selectedEntity_ = e;
            }
        }
    });
}
