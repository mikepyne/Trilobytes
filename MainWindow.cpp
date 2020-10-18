#include "MainWindow.h"
#include "ui_MainWindow.h"

#include "Swimmer.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    universe_ = std::make_shared<Universe>(*(ui->universe));
    ui->universe->SetUniverse(universe_);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::SetSwimmerToInspect(Swimmer& swimmer, EntityContainerInterface& container)
{
    ui->inspector->SetSwimmer(swimmer, container);
}
