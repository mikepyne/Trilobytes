#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "FoodPellet.h"
#include "Egg.h"
#include "Swimmer.h"
#include "MeatChunk.h"
#include "LineGraphContainerWidget.h"
#include "UniverseWidget.h"
#include "GeneFactory.h"

#include <nlohmann/json.hpp>

#include "QFileDialog"

#include <fstream>
//#include <filesystem>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    setWindowTitle("Trilobytes - Evolution Simulator");

    universe_ = std::make_shared<Universe>(Rect{ -500, -500, 500, 500 });
    ui->universe->SetUniverse(universe_);

    ui->newGraphButtonsContainer->setLayout(new QVBoxLayout());

    connect(ui->universe, &UniverseWidget::EntitySelected, [&](const std::shared_ptr<Entity>& selected)
    {
        ui->inspector->SetSwimmer(std::dynamic_pointer_cast<Swimmer>(selected));
        ui->inspector->UpdateConnectionStrengths(universe_->GetEntityContainer(), universe_->GetParameters());
    });
    connect(ui->universe, &UniverseWidget::Ticked, [&]()
    {
        ui->inspector->UpdateConnectionStrengths(universe_->GetEntityContainer(), universe_->GetParameters());
    });

    // Make sure graphs don't start too squashed
    ui->verticalSplitter->setSizes({ 700, 150 });

    /// File Menu Options
    connect(ui->actionSave_Selected_Swimmer, &QAction::triggered, [&]()
    {
        if (auto inspected = ui->inspector->GetSwimmer(); inspected != nullptr) {
            if (auto genome = inspected->InspectGenome(); genome != nullptr) {
                std::string saveFileName = QFileDialog::getSaveFileName(this, "Save Genome", "./SavedGenomes/", "Genome (*.genome)").toStdString();
                nlohmann::json serialised = Genome::Serialise(genome);
                // std::filesystem::create_directories(saveFileName);
                std::ofstream fileWriter(saveFileName);
                fileWriter << serialised.dump(2);
            }
        }
    });

    /// NeuralNetowrk Inspector controlls
    connect(ui->liveUpdateSelector, &QCheckBox::toggled, [&](bool checked) { ui->inspector->SetUpdateLive(checked); });
    connect(ui->resetInspectorView, &QPushButton::pressed, ui->inspector, &NeuralNetworkInspector::ResetViewTransform);

    /// Universe TPS & FPS controlls
    connect(ui->pauseButton, &QPushButton::toggled, ui->universe, &UniverseWidget::SetTicksPaused);
    connect(ui->limitButton, &QPushButton::toggled, ui->universe, &UniverseWidget::SetLimitTickRate);
    connect(ui->tpsSelector, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->universe, &UniverseWidget::SetTpsTarget);
    connect(ui->fpsSelector, static_cast<void(QSpinBox::*)(int)>(&QSpinBox::valueChanged), ui->universe, &UniverseWidget::SetFpsTarget);

    /// Global controlls
    connect(ui->resetAllButton, &QPushButton::pressed, [&]()
    {
        universe_ = std::make_shared<Universe>(Rect{ -500, -500, 500, 500 });
        ui->universe->SetUniverse(universe_);
        ui->inspector->SetSwimmer(nullptr);
        ui->inspector->ResetViewTransform();
        ResetGraphs();
    });
    connect(ui->removeAllSwimmersButton, &QPushButton::pressed, [&]()
    {
        universe_->ClearAllEntitiesOfType<Swimmer, Egg>();
    });
    connect(ui->removeAllFoodButton, &QPushButton::pressed, [&]()
    {
        universe_->ClearAllEntitiesOfType<FoodPellet, MeatChunk>();
    });
    connect(ui->addDefaultSwimmerButton, &QPushButton::pressed, [&]()
    {
        auto point = ApplyOffset({0, 0}, Random::Bearing(), Random::Number(0.0, 1000.0));
        universe_->AddEntity(std::make_shared<Swimmer>(300_mj, Transform{ point.x, point.y, Random::Bearing() }, GeneFactory::Get().GenerateDefaultGenome(NeuralNetwork::BRAIN_WIDTH)));
    });
    connect(ui->addRandomSwimmerButton, &QPushButton::pressed, [&]()
    {
        auto point = ApplyOffset({0, 0}, Random::Bearing(), Random::Number(0.0, 1000.0));
        universe_->AddEntity(std::make_shared<Swimmer>(300_mj, Transform{ point.x, point.y, Random::Bearing() }, GeneFactory::Get().GenerateRandomGenome(NeuralNetwork::BRAIN_WIDTH)));
    });
    connect(ui->quadCapacitySpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int capacity)
    {
        universe_->SetEntityTargetPerQuad(capacity, ui->quadLeewaySpinner->value());
    });
    connect(ui->quadLeewaySpinner, static_cast<void (QSpinBox::*)(int)>(&QSpinBox::valueChanged), [&](int leeway)
    {
        universe_->SetEntityTargetPerQuad(ui->quadCapacitySpinner->value(), leeway);
    });

    connect(ui->meanGeneMutationSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double mean) { universe_->GetParameters().meanGeneMutationCount_ = mean; });
    connect(ui->geneMutationStdDevSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double stdDev) { universe_->GetParameters().geneMutationCountStdDev_ = stdDev; });
    connect(ui->meanChromosomeMutationSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double mean) { universe_->GetParameters().meanStructuralMutationCount_ = mean; });
    connect(ui->chromosomeMutationStdDevSpinBox, static_cast<void(QDoubleSpinBox::*)(double)>(&QDoubleSpinBox::valueChanged), [&](double stdDev) { universe_->GetParameters().structuralMutationCountStdDev_ = stdDev; });

    /// Food Controlls
    connect(ui->spawnFoodToggle, &QPushButton::toggled, [&](bool state) { universe_->GetParameters().foodSpawnRateModifier = state ? 1.0 : 0.0; });

    /// Selected Swimmer Controlls
    connect(ui->selectFittestButton, &QPushButton::pressed, ui->universe, &UniverseWidget::SelectFittestSwimmer, Qt::QueuedConnection);
    connect(ui->followSelectedToggle, &QPushButton::toggled, ui->universe, &UniverseWidget::SetTrackSelectedEntity);

    /// Graph Controlls
    connect(ui->graphs, &QTabWidget::tabCloseRequested, [&](int index)
    {
        if (index != 0) {
            ui->graphs->removeTab(index);
        }
    });

    ResetGraphs();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::ResetGraphs()
{
    QString oldTitle = ui->graphs->tabText(ui->graphs->currentIndex());

    // Can't remove the current tab, so set to 1
    ui->graphs->setCurrentIndex(0);
    while (ui->graphs->count() > 1) {
        ui->graphs->removeTab(1);
    }

    // AddScatterGraph("Swimmer speciation", "", "", [=](uint64_t tick, ScatterGraph& graph)
    // {
    //     std::vector<std::shared_ptr<Swimmer>> swimmers;
    //
    //     universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
    //     {
    //         if (dynamic_cast<const Swimmer*>(e.get())) {
    //             swimmers.push_back(std::dynamic_pointer_cast<Swimmer>(e));
    //         }
    //     });
    //
    //     std::shared_ptr<Swimmer> swimmerA;
    //     std::shared_ptr<Swimmer> swimmerB;
    //
    //     for (auto& swimmer : swimmers) {
    //         for (auto& otherSwimmer : swimmers) {
    //             swimmerA->
    //         }
    //     }
    //
    //     std::vector<ScatterGraph::DataPoint> points;
    //
    //
    //
    //     graph.SetPoints(std::move(points));
    // });
    AddLineGraph("Lunar Cycle", { {0x010101, "Moon Phase"} }, "Time (tick)", "Full (%)", [=](uint64_t tick, LineGraph& graph)
    {
        graph.AddPoint(0, tick, 50 * (universe_->GetParameters().lunarCycle_ + 1));
    });
    AddLineGraph("Population", { {0x00F100, "Food Pellet"}, {0xFF0000, "Meat Chunk"}, {0x3333FF, "Swimmer"}, {0xFF44FF, "Egg"} }, "Time (tick)", "Number of Entities", [=](uint64_t tick, LineGraph& graph)
    {
        if (tick % 100 == 0) {
            uint64_t foodCount = 0;
            uint64_t chunkCount = 0;
            uint64_t swimmerCount = 0;
            uint64_t eggCount = 0;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (dynamic_cast<const FoodPellet*>(e.get())) {
                    ++foodCount;
                } else if (dynamic_cast<const MeatChunk*>(e.get())) {
                    ++chunkCount;
                } else if (dynamic_cast<const Swimmer*>(e.get())) {
                    ++swimmerCount;
                } else if (dynamic_cast<const Egg*>(e.get())) {
                    ++eggCount;
                }
            });
            graph.AddPoint(0, tick, foodCount);
            graph.AddPoint(1, tick, chunkCount);
            graph.AddPoint(2, tick, swimmerCount);
            graph.AddPoint(3, tick, eggCount);
        }
    });
    AddLineGraph("Total Energy", { {0x00F100, "Food Pellet"}, {0xFF0000, "Meat Chunk"}, {0x3333FF, "Swimmer"}, {0xFF44FF, "Egg"} }, "Time (tick)", "Combined Energy (mj)", [=](uint64_t tick, LineGraph& graph)
    {
        if (tick % 100 == 0) {
            Energy foodEnergy = 0;
            Energy chunkEnergy = 0;
            Energy swimmerEnergy = 0;
            Energy eggEnergy = 0;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (dynamic_cast<const FoodPellet*>(e.get())) {
                    foodEnergy += e->GetEnergy();
                } else if (dynamic_cast<const MeatChunk*>(e.get())) {
                    chunkEnergy += e->GetEnergy();
                } else if (dynamic_cast<const Swimmer*>(e.get())) {
                    swimmerEnergy += e->GetEnergy();
                } else if (dynamic_cast<const Egg*>(e.get())) {
                    eggEnergy += e->GetEnergy();
                }
            });
            graph.AddPoint(0, tick, foodEnergy / 1_mj);
            graph.AddPoint(1, tick, chunkEnergy / 1_mj);
            graph.AddPoint(2, tick, swimmerEnergy / 1_mj);
            graph.AddPoint(3, tick, eggEnergy / 1_mj);
        }
    });
    AddLineGraph("Average Age", { {0x00FC00, "Food"}, {0x3333FF, "Swimmer"} }, "Time (tick)", "Age (tick)", [=](uint64_t tick, LineGraph& graph)
    {
        if (tick % 100 == 0) {
            Tril::RollingStatistics foodStats;
            Tril::RollingStatistics swimmerStats;
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
    AddLineGraph("Max Age", { {0x00FC00, "Food"}, {0x3333FF, "Swimmer"} }, "Time (tick)", "Age (tick)", [=](uint64_t tick, LineGraph& graph)
    {
        if (tick % 100 == 0) {
            Tril::RollingStatistics foodStats;
            Tril::RollingStatistics swimmerStats;
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
    AddLineGraph("Health (%)", { { 0xFFDFDF, "Min (%)" }, { 0xFF0000, "Mean (%)" }, { 0xFFDFDF, "Max (%)" }, { 0x00CF00, "StdDev lowerBound" }, { 0xCF3000, "StdDev upper bound" } }, "Time (tick)", "Swimmer Health (%)", [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
    {
        if (tick % 100 == 0) {
            Tril::RollingStatistics stats;
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
    AddLineGraph("Generation", { { 0xDFDFDF, "Min" }, { 0x0000FF, "Mean" }, { 0xDFDFDF, "Max" }, { 0x00CF00, "StdDev lowerBound" }, { 0xCF3000, "StdDev upper bound" } }, "Time (tick)", "Swimmer Generation", [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
    {
        if (tick % 100 == 0) {
            Tril::RollingStatistics stats;
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
    AddLineGraph("Mutations", { { 0xF0F000, "Mean Gene Mutations" }, { 0xF000FF, "Mean Chromosome Mutations" } }, "Time (tick)", "Mutations per Genome", [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
    {
        if (tick % 100 == 0) {
            Tril::RollingStatistics geneStats;
            Tril::RollingStatistics chromosomeStats;
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
    AddLineGraph("Base Metabolism", { { 0xDFDFDF, "Min (uj)" }, { 0x0000FF, "Mean (uj)" }, { 0xDFDFDF, "Max (uj)" }, { 0x00CF00, "StdDev lowerBound" }, { 0xCF3000, "StdDev upper bound" } }, "Time (tick)", "Energy (uj)",
    [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
    {
        if (tick % 100 == 0) {
            Tril::RollingStatistics stats;
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
    AddLineGraph("Swimmer Velocity", { { 0xDFDFDF, "Min" }, { 0x0000FF, "Mean" }, { 0xDFDFDF, "Max" }, { 0x00CF00, "StdDev lowerBound" }, { 0xCF3000, "StdDev upper bound" } }, "Time (tick)", "Velocity (pixels per tick)",
    [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
    {
        if (tick % 100 == 0) {
            Tril::RollingStatistics stats;
            universe_->ForEach([&](const std::shared_ptr<Entity>& e) -> void
            {
                if (const Swimmer* swimmer = dynamic_cast<const Swimmer*>(e.get()); swimmer != nullptr) {
                    stats.AddValue(std::abs(swimmer->GetVelocity()));
                }
            });
            if (stats.Count() > 0) {
                graph.AddPoint(0, tick, stats.Min());
                graph.AddPoint(1, tick, stats.Mean());
                graph.AddPoint(2, tick, stats.Max());
                // Don't let our lower bound go below min
                graph.AddPoint(3, tick, std::max(stats.Min(), stats.Mean() - stats.StandardDeviation()));
                // Don't let our upper bound go above max
                graph.AddPoint(4, tick, std::min(stats.Max(), stats.Mean() + stats.StandardDeviation()));
            }
        }
    });
    AddLineGraph("Performance", { { 0xFC02DF, "Ticks per Second (Mean)" } }, "Time (tick)", "TPS", [=, previous = std::chrono::steady_clock::now()](uint64_t tick, LineGraph& graph) mutable
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

    for (int i = 0; i < ui->graphs->count(); ++i) {
        if (ui->graphs->tabText(i) == oldTitle) {
            ui->graphs->setCurrentIndex(i);
            break;
        }
    }
}

void MainWindow::AddLineGraph(QString graphTitle, std::vector<std::pair<QRgb, QString> >&& plots, QString xAxisTitle, QString yAxisTitle, std::function<void (uint64_t tick, LineGraph& graph)>&& task)
{
    QPushButton* button = new QPushButton(QString("Create \"%1\" graph").arg(graphTitle));
    ui->newGraphButtonsContainer->layout()->addWidget(button);

    connect(button, &QPushButton::pressed, [=]()
    {
        LineGraph* lineGraph = new LineGraph(nullptr, xAxisTitle, yAxisTitle);
        for (const auto& [ colour, name ] : plots) {
            lineGraph->AddPlot(colour, name);
        }
        auto handle = universe_->AddTask([=, task = std::move(task)](uint64_t tick) { task(tick, *lineGraph); });
        ui->graphs->addTab(new LineGraphContainerWidget(nullptr, std::move(handle), lineGraph), graphTitle);
    });
    emit button->pressed();
}

void MainWindow::AddScatterGraph(QString graphTitle, QString xAxisTitle, QString yAxisTitle, std::function<void (uint64_t, ScatterGraph&)>&& task)
{
    QPushButton* button = new QPushButton(QString("Create \"%1\" graph").arg(graphTitle));
    ui->newGraphButtonsContainer->layout()->addWidget(button);

    connect(button, &QPushButton::pressed, [=]()
    {
        ScatterGraph* scatterGraph = new ScatterGraph(nullptr, graphTitle, xAxisTitle, yAxisTitle);



        // handle is going out of scope here, need to capture it !!!!!!!!

        // NEED to include gene name in json or will never be able to deserialise!!!!!!



        auto handle = universe_->AddTask([=, task = std::move(task)](uint64_t tick) { task(tick, *scatterGraph); });
        ui->graphs->addTab(scatterGraph, graphTitle);
    });
    emit button->pressed();
}
