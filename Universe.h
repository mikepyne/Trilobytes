#ifndef UNIVERSE_H
#define UNIVERSE_H

#include "QuadTree.h"
#include "Energy.h"
#include "LineGraph.h"
#include "FeedDispenser.h"

#include <QTimer>
#include <QPainter>

#include <iomanip>
#include <functional>
#include <math.h>

/*
 * Allows the Universe, which has no other notion of being observed, let an
 * observer know where to look.
 */
class UniverseObserver {
public:
    virtual void SuggestFocus(const Point& focus) = 0;
    virtual void SuggestUpdate() = 0;
};

class Universe {
public:
    /**
     * Intentionally empty, used to identify tasks for removal from
     * Universe::perTickTasks_
     */
    enum class TaskHandle : size_t {
    };

    Universe(UniverseObserver& focusInterface);

    void SetPaused(bool paused) { pauseSim_ = paused; }
    void SetLimitTickRate(bool limited);
    void SetTpsTarget(int tps);
    void SetSpawnFood(bool spawn) { spawnFood_ = spawn; }
    void SetAutoSelectFittest(bool autoSelect) { autoSelectFittest_ = autoSelect; }
    void SetTrackSelected(bool track) { trackSelectedEntity_ = track; }
    void SetEntityTargetPerQuad(uint64_t target, uint64_t leeway);
    void Respawn() { respawn_ = true; }
    void Reset() {  reset_ = true; }
    void RemoveAllSwimmers() { removeAllSwimmers_ = true; }
    void RemoveAllFood() { removeAllFood_ = true; }
    void SelectEntity(const Point& location);
    void SelectFittestSwimmer();
    void AddDefaultSwimmer(double x, double y);
    void AddRandomSwimmer(double x, double y);
    /**
     * @brief The Task system allows clients to have code run each tick, without
     * the Universe needing to know implementation specifics. For example,
     * graphs can use tasks to collect the data they wish to display.
     *
     * @param task The code to be run each tick.
     *
     * @return A Handle that defines the lifetime of the task. When no copies of
     * the handle exist, the task is removed from the list
     */
    [[nodiscard]] EoBE::Handle AddTask(std::function<void(uint64_t tick)>&& task);

    bool GetPaused() const { return pauseSim_; }
    bool GetSpawnFood() const { return spawnFood_; }
    bool GetAutoSelectFittest() const { return autoSelectFittest_; }
    bool GetTrackSelected() const { return trackSelectedEntity_; }
    std::shared_ptr<Entity> GetSelectedEntity() { return selectedEntity_; }
    EntityContainerInterface& GetEntityContainer() { return rootNode_.GetContainer(); }
    void Render(QPainter& painter) const;
    void ForEach(const std::function<void (const std::shared_ptr<Entity>& e)>& action) const;

    double GetLunarCycle() const;

private:
    UniverseObserver& observerInterface_;

    bool spawnFood_ = true;
    bool pauseSim_ = false;
    bool limitSim_ = true;
    unsigned targetTps_ = 1000 / 60;
    bool respawn_ = true;
    bool reset_ = false;
    bool removeAllSwimmers_ = false;
    bool removeAllFood_ = false;
    bool autoSelectFittest_ = false;
    bool trackSelectedEntity_ = false;
    std::shared_ptr<Entity> selectedEntity_;

    QuadTree rootNode_;
    std::vector<std::shared_ptr<FeedDispenser>> feedDispensers_;

    QTimer mainThread_;

    uint64_t tickIndex_;

    EoBE::AutoClearingContainer<std::function<void(uint64_t tick)>> perTickTasks_;

    void Thread();
    void Tick();
    void UpdateTps();
};

#endif // UNIVERSE_H
