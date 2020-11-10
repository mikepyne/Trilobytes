#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "FoodPellet.h"
#include "Swimmer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    universe_ = std::make_shared<Universe>(*(ui->universe));
    ui->universe->SetUniverse(universe_);

    // Make sure graphs don't start too squashed
    ui->verticalSplitter->setSizes({ 700, 150 });

    /// NeuralNetowrk Inspector controlls
    connect(ui->liveUpdateSelector, &QCheckBox::toggled, [&](bool checked) { ui->inspector->SetUpdateLive(checked); });
    connect(ui->resetInspectorView, &QPushButton::pressed, ui->inspector, &NeuralNetworkInspector::ResetViewTransform);

    /// Universe TPS & FPS controlls
    connect(ui->pauseButton, &QPushButton::toggled, [&](bool state) { universe_->SetPaused(state); });
    connect(ui->limitButton, &QPushButton::toggled, [&](bool state) { universe_->SetLimitTickRate(state); });
    connect(ui->tpsSelector, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int value) { universe_->SetTpsTarget(value); });
    connect(ui->fpsSelector, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->universe, &UniverseWidget::SetFpsTarget);

    /// Global controlls
    connect(ui->resetAllButton, &QPushButton::pressed, [&]()
    {
        universe_->Reset();
        ui->inspector->SetSwimmer(nullptr);
        ui->inspector->ResetViewTransform();
        for (int tabIndex = 0; tabIndex < ui->graphs->count(); ++tabIndex) {
            if (LineGraph* graph = dynamic_cast<LineGraph*>(ui->graphs->widget(tabIndex))) {
                graph->Reset();
            }
        }
    });
    connect(ui->addDefaultSwimmerButton, &QPushButton::pressed, [&]()
    {
        auto point = ApplyOffset({0, 0}, Random::Bearing(), Random::Number(0.0, 1000.0));
        universe_->AddDefaultSwimmer(point.x, point.y);
    });
    connect(ui->addRandomSwimmerButton, &QPushButton::pressed, [&]()
    {
        auto point = ApplyOffset({0, 0}, Random::Bearing(), Random::Number(0.0, 1000.0));
        universe_->AddRandomSwimmer(point.x, point.y);
    });
    connect(ui->quadCapacitySpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int capacity)
    {
        universe_->SetEntityTargetPerQuad(capacity, ui->quadLeewaySpinner->value());
    });
    connect(ui->quadLeewaySpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int leeway)
    {
        universe_->SetEntityTargetPerQuad(ui->quadCapacitySpinner->value(), leeway);
    });

    /// Food Controlls
    connect(ui->spawnFoodToggle, &QPushButton::toggled, [&](bool state) { universe_->SetSpawnFood(state); });

    /// Selected Swimmer Controlls
    connect(ui->selectFittestButton, &QPushButton::pressed, [&]() { universe_->SelectFittestSwimmer(); });
    connect(ui->followSelectedToggle, &QPushButton::toggled, [&](bool state) { universe_->SetTrackSelected(state); });

    /// Graph Controlls
    connect(ui->graphs, &QTabWidget::tabCloseRequested, [&](int index)
    {
        // FIXME index of tabs changes whenever one closes, SO THIS DOES NOT WORK!!!!!!!
        if (index > 0) {
            // TODO perhaps the handle idea could be a little more RAII (destruction of handle automatically removes task from Universe)
            // TODO need to give the graphs the RAII handle for their update function so it is removed when a graph is closed
            // TODO need to work out how to update a graphs range when opened midway through a run
            // TODO graphs currently collect points forever, need to have two more options, fixed width moving window, and fixed width with resolution auto reduced when filled
            ui->graphs->removeTab(index);
            universe_->RemoveTask(graphTaskHandles_.at(index));
            graphTaskHandles_.erase(index);
        }
    });

    AddGraph("Energy", { {0x00F100, "Food"}, {0x3333FF, "Swimmer"} }, "Time (tick)", "Combined Energy (mj)", [=](uint64_t tick, LineGraph& graph)
    {
        if (tick % 100 == 0) {
            Energy foodEnergy = 0;
            Energy swimmerEnergy = 0;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (dynamic_cast<const FoodPellet*>(e.get())) {
                    foodEnergy += e->GetEnergy();
                } else if (dynamic_cast<const Swimmer*>(e.get())) {
                    swimmerEnergy += e->GetEnergy();
                }
            });
            graph.AddPoint(0, tick, foodEnergy / 1_mj);
            graph.AddPoint(1, tick, swimmerEnergy / 1_mj);
        }
    });
    AddGraph("Mean Age", { {0x00FC00, "Food"}, {0x3333FF, "Swimmer"} }, "Time (tick)", "Average Age (tick)", [=](uint64_t tick, LineGraph& graph)
    {
        if (tick % 100 == 0) {
            uint64_t count = 0;
            double totalFoodAge = 0;
            double totalSwimmerAge = 0;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (dynamic_cast<const FoodPellet*>(e.get())) {
                    ++count;
                    totalFoodAge += e->GetAge();
                } else if (dynamic_cast<const Swimmer*>(e.get())) {
                    ++count;
                    totalSwimmerAge += e->GetAge();
                }
            });
            graph.AddPoint(0, tick, totalFoodAge / count);
            graph.AddPoint(1, tick, totalSwimmerAge / count);
        }
    });
    AddGraph("Oldest", { {0x00FC00, "Food"}, {0x3333FF, "Swimmer"} }, "Time (tick)", "Oldest Individual (tick)", [=](uint64_t tick, LineGraph& graph)
    {
        if (tick % 100 == 0) {
            uint64_t oldestFoodAge = 0;
            uint64_t oldestSwimmerAge = 0;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (dynamic_cast<const FoodPellet*>(e.get())) {
                    oldestFoodAge = std::max(e->GetAge(), oldestFoodAge);
                } else if (dynamic_cast<const Swimmer*>(e.get())) {
                    oldestSwimmerAge = std::max(e->GetAge(), oldestSwimmerAge);
                }
            });
            graph.AddPoint(0, tick, oldestFoodAge);
            graph.AddPoint(1, tick, oldestSwimmerAge);
        }
    });
    AddGraph("Generation", { { 0xDFDFDF, "Min Generation" }, { 0x0000FF, "Mean Generation" }, { 0xDFDFDF, "Max Generation" }, { 0x00CF00, "StdDev lowerBound" }, { 0xCF3000, "StdDev upper bound" } }, "Time (tick)", "Swimmer Generation", [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
    {
        if (tick % 100 == 0) {
            EoBE::RollingStatistics stats;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (const Swimmer* swimmer = dynamic_cast<const Swimmer*>(e.get()); swimmer != nullptr) {
                    stats.AddValue(swimmer->GetGeneration());
                }
            });
            graph.AddPoint(0, tick, stats.Min());
            graph.AddPoint(1, tick, stats.Mean());
            graph.AddPoint(2, tick, stats.Max());
            // Don't let our lower bound go below min
            graph.AddPoint(3, tick, std::max(stats.Min(), stats.Mean() - stats.StandardDeviation()));
            // Don't let our upper  bound go above max
            graph.AddPoint(4, tick, std::min(stats.Max(), stats.Mean() + stats.StandardDeviation()));
        }
    });
    AddGraph("Performance", { { 0xFC02DF, "Ticks per Second" } }, "Time (tick)", "Ticks per Second (mean)", [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
    {
        // TODO check every tick and examine interesting periodic perofrmance behaviour
        if (tick % 100 == 0) {
            auto now = std::chrono::steady_clock::now();
            double seconds = std::chrono::duration_cast<std::chrono::duration<double>>(now - previous).count();
            double tps = seconds > 0.0 ? 1.0 / (seconds / 100.0) : 0.0;
            graph.AddPoint(0, tick, tps);
            previous = now;
        }
    });

    // FIXME temp hack to get inspector pane working again (priority is to get a distributable version)
    universe_->AddTask([&](uint64_t /*tick*/)
    {
        ui->inspector->SetSwimmer(std::dynamic_pointer_cast<Swimmer>(universe_->GetSelectedEntity()));
        ui->inspector->UpdateConnectionStrengths(universe_->GetEntityContainer());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddGraph(QString graphTitle, std::vector<std::pair<QRgb, QString> >&& plots, QString xAxisTitle, QString yAxisTitle, std::function<void (uint64_t tick, LineGraph& graph)>&& task)
{
    LineGraph* lineGraph = new LineGraph(nullptr, xAxisTitle, yAxisTitle);
    for (const auto& [ colour, name ] : plots) {
        lineGraph->AddPlot(colour, name);
    }
    Universe::TaskHandle handle = universe_->AddTask([=, task = std::move(task)](uint64_t tick) { task(tick, *lineGraph); });
    int index = ui->graphs->addTab(lineGraph, graphTitle);
    graphTaskHandles_.insert({ index, handle });
}
