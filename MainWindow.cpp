#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->universe->SetupGraphs(*ui->graph);
}

MainWindow::~MainWindow()
{
    delete ui;
}

