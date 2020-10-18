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

    connect(ui->graphs, &QTabWidget::tabCloseRequested, [&](int index)
    {
        // TODO perhaps the handle idea could be a little more RAII (destruction of handle automatically removes task from Universe)
        universe_->RemoveTask(graphTaskHandles_.at(index));
        graphTaskHandles_.erase(index);
        ui->graphs->removeTab(index);
    });

    AddGraph("Energy", { {0x33FF33, "Food"}, {0x3333FF, "Swimmer"} }, "Time (tick)", "Combined Energy (mj)", [=](uint64_t tick, LineGraph& graph)
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
    AddGraph("Mean Age", { {0x33FF33, "Food"}, {0x3333FF, "Swimmer"} }, "Time (tick)", "Average Age (tick)", [=](uint64_t tick, LineGraph& graph)
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
    AddGraph("Oldest", { {0x33FF33, "Food"}, {0x3333FF, "Swimmer"} }, "Time (tick)", "Oldest Individual (tick)", [=](uint64_t tick, LineGraph& graph)
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
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetSwimmerToInspect(Swimmer& swimmer, EntityContainerInterface& container)
{
    ui->inspector->SetSwimmer(swimmer, container);
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
