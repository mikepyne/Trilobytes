#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "FoodPellet.h"
#include "Swimmer.h"
#include "MeatChunk.h"
#include "GraphContainerWidget.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    universe_ = std::make_shared<Universe>(*(ui->universe));
    ui->universe->SetUniverse(universe_);
    ui->graphCreationTab->setLayout(new QVBoxLayout());

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
            if (GraphContainerWidget* graphContainer = dynamic_cast<GraphContainerWidget*>(ui->graphs->widget(tabIndex))) {
                graphContainer->ResetGraph();
            }
        }
    });
    connect(ui->removeAllSwimmersButton, &QPushButton::pressed, [&]()
    {
        universe_->RemoveAllSwimmers();
    });
    connect(ui->removeAllFoodButton, &QPushButton::pressed, [&]()
    {
        universe_->RemoveAllFood();
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

    connect(ui->meanGeneMutationSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double mean) { universe_->SetMeanGeneMutationCount(mean); });
    connect(ui->geneMutationStdDevSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double stdDev) { universe_->SetGeneMutationStdDev(stdDev); });
    connect(ui->meanChromosomeMutationSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double mean) { universe_->SetMeanChromosomeMutationCount(mean); });
    connect(ui->chromosomeMutationStdDevSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double stdDev) { universe_->SetChromosomeMutationStdDev(stdDev); });

    /// Food Controlls
    connect(ui->spawnFoodToggle, &QPushButton::toggled, [&](bool state) { universe_->SetSpawnFood(state); });

    /// Selected Swimmer Controlls
    connect(ui->selectFittestButton, &QPushButton::pressed, [&]() { universe_->SelectFittestSwimmer(); });
    connect(ui->followSelectedToggle, &QPushButton::toggled, [&](bool state) { universe_->SetTrackSelected(state); });

    /// Graph Controlls
    connect(ui->graphs, &QTabWidget::tabCloseRequested, [&](int index)
    {
        if (index != 0) {
            ui->graphs->removeTab(index);
        }
    });

    AddGraph("Lunar Cycle", { {0x010101, "Moon Phase"} }, "Time (tick)", "Full (%)", [=](uint64_t tick, LineGraph& graph)
    {
        graph.AddPoint(0, tick, 50 * (universe_->GetLunarCycle() + 1));
    });
    AddGraph("Total Energy", { {0x00F100, "Food Pellet"}, {0xFF0000, "Meat Chunk"}, {0x3333FF, "Swimmer"} }, "Time (tick)", "Combined Energy (mj)", [=](uint64_t tick, LineGraph& graph)
    {
        if (tick % 100 == 0) {
            Energy foodEnergy = 0;
            Energy chunkEnergy = 0;
            Energy swimmerEnergy = 0;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (dynamic_cast<const FoodPellet*>(e.get())) {
                    foodEnergy += e->GetEnergy();
                } else if (dynamic_cast<const MeatChunk*>(e.get())) {
                    chunkEnergy += e->GetEnergy();
                } else if (dynamic_cast<const Swimmer*>(e.get())) {
                    swimmerEnergy += e->GetEnergy();
                }
            });
            graph.AddPoint(0, tick, foodEnergy / 1_mj);
            graph.AddPoint(1, tick, chunkEnergy / 1_mj);
            graph.AddPoint(2, tick, swimmerEnergy / 1_mj);
        }
    });
    AddGraph("Average Age", { {0x00FC00, "Food"}, {0x3333FF, "Swimmer"} }, "Time (tick)", "Age (tick)", [=](uint64_t tick, LineGraph& graph)
    {
        if (tick % 100 == 0) {
            EoBE::RollingStatistics foodStats;
            EoBE::RollingStatistics swimmerStats;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (dynamic_cast<const FoodPellet*>(e.get())) {
                    foodStats.AddValue(e->GetAge());
                } else if (dynamic_cast<const Swimmer*>(e.get())) {
                    swimmerStats.AddValue(e->GetAge());
                }
            });
            graph.AddPoint(0, tick, foodStats.Mean());
            graph.AddPoint(1, tick, swimmerStats.Mean());
        }
    });
    AddGraph("Max Age", { {0x00FC00, "Food"}, {0x3333FF, "Swimmer"} }, "Time (tick)", "Age (tick)", [=](uint64_t tick, LineGraph& graph)
    {
        if (tick % 100 == 0) {
            EoBE::RollingStatistics foodStats;
            EoBE::RollingStatistics swimmerStats;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (dynamic_cast<const FoodPellet*>(e.get())) {
                    foodStats.AddValue(e->GetAge());
                } else if (dynamic_cast<const Swimmer*>(e.get())) {
                    swimmerStats.AddValue(e->GetAge());
                }
            });
            graph.AddPoint(0, tick, foodStats.Max());
            graph.AddPoint(1, tick, swimmerStats.Max());
        }
    });
    AddGraph("Health (%)", { { 0xFFDFDF, "Min (%)" }, { 0xFF0000, "Mean (%)" }, { 0xFFDFDF, "Max (%)" }, { 0x00CF00, "StdDev lowerBound" }, { 0xCF3000, "StdDev upper bound" } }, "Time (tick)", "Swimmer Health (%)", [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
    {
        if (tick % 100 == 0) {
            EoBE::RollingStatistics stats;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (const Swimmer* swimmer = dynamic_cast<const Swimmer*>(e.get()); swimmer != nullptr) {
                    stats.AddValue(swimmer->GetHealth());
                }
            });
            if (stats.Count() > 0) {
                graph.AddPoint(0, tick, stats.Min());
                graph.AddPoint(1, tick, stats.Mean());
                graph.AddPoint(2, tick, stats.Max());
                // Don't let our lower bound go below min
                graph.AddPoint(3, tick, std::max(stats.Min(), stats.Mean() - stats.StandardDeviation()));
                // Don't let our upper  bound go above max
                graph.AddPoint(4, tick, std::min(stats.Max(), stats.Mean() + stats.StandardDeviation()));
            }
        }
    });
    AddGraph("Generation", { { 0xDFDFDF, "Min" }, { 0x0000FF, "Mean" }, { 0xDFDFDF, "Max" }, { 0x00CF00, "StdDev lowerBound" }, { 0xCF3000, "StdDev upper bound" } }, "Time (tick)", "Swimmer Generation", [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
    {
        if (tick % 100 == 0) {
            EoBE::RollingStatistics stats;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (const Swimmer* swimmer = dynamic_cast<const Swimmer*>(e.get()); swimmer != nullptr) {
                    stats.AddValue(swimmer->GetGeneration());
                }
            });
            if (stats.Count() > 0) {
                graph.AddPoint(0, tick, stats.Min());
                graph.AddPoint(1, tick, stats.Mean());
                graph.AddPoint(2, tick, stats.Max());
                // Don't let our lower bound go below min
                graph.AddPoint(3, tick, std::max(stats.Min(), stats.Mean() - stats.StandardDeviation()));
                // Don't let our upper  bound go above max
                graph.AddPoint(4, tick, std::min(stats.Max(), stats.Mean() + stats.StandardDeviation()));
            }
        }
    });
    AddGraph("Mutations", { { 0xF0F000, "Mean Gene Mutations" }, { 0xF000FF, "Mean Chromosome Mutations" } }, "Time (tick)", "Mutations per Genome", [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
    {
        if (tick % 100 == 0) {
            EoBE::RollingStatistics geneStats;
            EoBE::RollingStatistics chromosomeStats;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (const Swimmer* swimmer = dynamic_cast<const Swimmer*>(e.get()); swimmer != nullptr) {
                    geneStats.AddValue(swimmer->GetGeneMutationCount());
                    chromosomeStats.AddValue(swimmer->GetChromosomeMutationCount());
                }
            });
            if (geneStats.Count() > 0) {
                graph.AddPoint(0, tick, geneStats.Mean());
            }
            if (chromosomeStats.Count() > 0) {
                graph.AddPoint(1, tick, chromosomeStats.Mean());
            }
        }
    });
    AddGraph("Base Metabolism", { { 0xDFDFDF, "Min (uj)" }, { 0x0000FF, "Mean (uj)" }, { 0xDFDFDF, "Max (uj)" }, { 0x00CF00, "StdDev lowerBound" }, { 0xCF3000, "StdDev upper bound" } }, "Time (tick)", "Energy (uj)",
    [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
    {
        if (tick % 100 == 0) {
            EoBE::RollingStatistics stats;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (const Swimmer* swimmer = dynamic_cast<const Swimmer*>(e.get()); swimmer != nullptr) {
                    stats.AddValue(swimmer->GetBaseMetabolism());
                }
            });
            if (stats.Count() > 0) {
                graph.AddPoint(0, tick, stats.Min() / 1_uj);
                graph.AddPoint(1, tick, stats.Mean() / 1_uj);
                graph.AddPoint(2, tick, stats.Max() / 1_uj);
                // Don't let our lower bound go below min
                graph.AddPoint(3, tick, std::max(stats.Min(), stats.Mean() - stats.StandardDeviation()) / 1_uj);
                // Don't let our upper bound go above max
                graph.AddPoint(4, tick, std::min(stats.Max(), stats.Mean() + stats.StandardDeviation()) / 1_uj);
            }
        }
    });

    AddGraph("Performance", { { 0xFC02DF, "Ticks per Second (Mean)" } }, "Time (tick)", "TPS", [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
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
    static auto hackTaskHandle = universe_->AddTask([&](uint64_t /*tick*/)
    {
        ui->inspector->SetSwimmer(std::dynamic_pointer_cast<Swimmer>(universe_->GetSelectedEntity()));
        ui->inspector->UpdateConnectionStrengths(universe_->GetEntityContainer(), universe_->GetUniverseParameters());
    });
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::AddGraph(QString graphTitle, std::vector<std::pair<QRgb, QString> >&& plots, QString xAxisTitle, QString yAxisTitle, std::function<void (uint64_t tick, LineGraph& graph)>&& task)
{
    QPushButton* button = new QPushButton(QString("Create \"%1\" graph").arg(graphTitle));
    ui->graphCreationTab->layout()->addWidget(button);

    connect(button, &QPushButton::pressed, [=]()
    {
        LineGraph* lineGraph = new LineGraph(nullptr, xAxisTitle, yAxisTitle);
        for (const auto& [ colour, name ] : plots) {
            lineGraph->AddPlot(colour, name);
        }
        auto handle = universe_->AddTask([=, task = std::move(task)](uint64_t tick) { task(tick, *lineGraph); });
        ui->graphs->addTab(new GraphContainerWidget(nullptr, std::move(handle), lineGraph), graphTitle);
    });
    emit button->pressed();
}
