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
class UniverseFocusInterface {
public:
    virtual void SetFocus(const Point& focus) = 0;
};

class Universe {
public:
    /**
     * Intentionally empty, used to identify tasks for removal from
     * Universe::perTickTasks_
     */
    enum class TaskHandle : size_t {
    };

    Universe(UniverseFocusInterface& focusInterface);

    void SetPaused(bool paused) { pauseSim_ = paused; }
    void SetSpawnFood(bool spawn) { spawnFood_ = spawn; }
    void SetAutoSelectFittest(bool autoSelect) { autoSelectFittest_ = autoSelect; }
    void SetTrackSelected(bool track) { trackSelectedEntity_ = track; }
    void Respawn() { respawn_ = true; }
    void Reset() {  reset_ = true; }
    void SelectEntity(const Point& location);
    void SelectFittestSwimmer();

    bool GetPaused() const { return pauseSim_; }
    bool GetSpawnFood() const { return spawnFood_; }
    bool GetAutoSelectFittest() const { return autoSelectFittest_; }
    bool GetTrackSelected() const { return trackSelectedEntity_; }
    void Render(QPainter& painter) const;

    /**
     * @brief The Task system allows clients to have code run each tick, without
     * the Univers neding to know implementation specifics. For example, graphs
     * can use tasks to collect the data they wish to display.
     *
     * @param task The code to be run each tick.
     *
     * @return A handle that can be used to remove the task in the future.
     */
    TaskHandle AddTask(std::function<void(uint64_t tick)>&& task)
    {
        // Find lowest value task handle not in use
        TaskHandle newHandle = static_cast<TaskHandle>(0);
        for(auto& [ handle, task ] : perTickTasks_) {
            (void) task; // unused
            if (newHandle == handle) {
                newHandle = static_cast<TaskHandle>(static_cast<size_t>(newHandle) + 1);
            }
        }
        perTickTasks_.insert({ newHandle, std::move(task) });
        return newHandle;
    }

private:
    UniverseFocusInterface& focusInterface_;

    bool spawnFood_ = true;
    bool pauseSim_ = false;
    bool respawn_ = true;
    bool reset_ = false;
    bool autoSelectFittest_ = false;
    bool trackSelectedEntity_ = false;
    std::shared_ptr<Entity> selectedEntity_;

    QuadTree rootNode_;
    std::vector<std::shared_ptr<FeedDispenser>> feedDispensers_;

    QTimer mainThread_;

    uint64_t tickIndex_;
    std::map<TaskHandle, std::function<void(uint64_t tick)>> perTickTasks_;

    void Thread();
};

#endif // UNIVERSE_H
