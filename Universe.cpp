#include "Universe.h"

#include "Swimmer.h"
#include "FeedDispenser.h"
#include "FoodPellet.h"
#include "Random.h"
#include "MainWindow.h"

// TODO RenderSettings struct to contain such as showQuads, showSenseRanges, showPole, showFoodSpawnArea

Universe::Universe(UniverseFocusInterface& focusInterface)
    : focusInterface_(focusInterface)
    , rootNode_({0, 0, 1000, 1000})
{
    Reset();

    focusInterface_.SetFocus({0,0});
    /*
     * QT hack to get this running in the QT event loop (necessary for
     * drawing anything, not ideal for running the Sim quickly...)
     */
    mainThread_.setSingleShot(false);
    mainThread_.connect(&mainThread_, &QTimer::timeout, [&]()
    {
        Thread();
    });
    mainThread_.start(0);

}

void Universe::SelectEntity(const Point& location)
{
    selectedEntity_.reset();
    rootNode_.ForEachCollidingWith(location, [&](const std::shared_ptr<Entity>& e)
    {
        if (Swimmer* swimmer = dynamic_cast<Swimmer*>(e.get()); swimmer != nullptr) {
            selectedEntity_ = e;
        }
    });
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

void Universe::Render(QPainter& p) const
{
    if (spawnFood_) {
        for (auto& dispenser : feedDispensers_) {
            dispenser->Draw(p);
        }
    }
    rootNode_.Draw(p);

    // TODO remove all this once it is in a GUI
    p.setPen(Qt::black);
    p.setBackground(QColor(200, 255, 255));
    p.setBackgroundMode(Qt::BGMode::OpaqueMode);
    int line = 0;
    p.drawText(0, line += 15, "Paused (space): " + QVariant(pauseSim_).toString());
    p.drawText(0, line += 15, "Spawn Food (F): " + QVariant(spawnFood_).toString());
    p.drawText(0, line += 15, "Reset (Q)");
    p.drawText(0, line += 15, "Respawn (R)");
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
    if (reset_) {
        reset_ = false;

        feedDispensers_.clear();
        tickIndex_ = 0;
        rootNode_.Clear();
        selectedEntity_.reset();
        respawn_ = true;

        feedDispensers_.push_back(std::make_shared<FeedDispenser>(rootNode_,  1000, 0, 950, 0.001));
        feedDispensers_.push_back(std::make_shared<FeedDispenser>(rootNode_, -1000, 0, 950, 0.001));

        for (auto& feeder : feedDispensers_) {
            focusInterface_.SetFocus({ feeder->GetX(), feeder->GetY() });
            feeder->AddPelletsImmediately(feeder->GetMaxPellets() / 8);
        }

        rootNode_.SetEntityCapacity(25, 5);
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

    if (!pauseSim_) {

        rootNode_.Tick();

        if (autoSelectFittest_) {
            SelectFittestSwimmer();
        }

        if (trackSelectedEntity_ && selectedEntity_ && selectedEntity_->Alive()) {
            focusInterface_.SetFocus( { selectedEntity_->GetLocation().x, selectedEntity_->GetLocation().y });
        }

        if (spawnFood_) {
            for (auto& dispenser : feedDispensers_) {
                dispenser->Tick();
            }
        }

        // static uint64_t tick = 0;
        // if (++tick % 100 == 0) {
        //     Energy foodEnergy = 0;
        //     Energy swimmerEnergy = 0;
        //     lastEntityCount_ = 0;
        //     lastSwimmerCount_ = 0;
        //     rootNode_.ForEach([&](const std::shared_ptr<Entity>& e) -> void
        //     {
        //         ++lastEntityCount_;
        //         if (const auto* f = dynamic_cast<const FoodPellet*>(e.get())) {
        //             foodEnergy += f->GetEnergy();
        //         } else if (const auto* s = dynamic_cast<const Swimmer*>(e.get())) {
        //             ++lastSwimmerCount_;
        //             swimmerEnergy += s->GetEnergy();
        //         }
        //     });
        //     emit OnFoodEnergySampled(foodEnergy);
        //     emit OnSwimmerEnergySampled(swimmerEnergy);
        // }

        for (auto& [handle, task] : perTickTasks_) {
            task(tickIndex_);
        }

        ++tickIndex_;
    }
}


