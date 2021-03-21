#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "Universe.h"
#include "ScatterGraph.h"

#include <QMainWindow>
#include <QImage>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class Swimmer;
class EntityContainerInterface;

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    Ui::MainWindow *ui;
    std::shared_ptr<Universe> universe_;

    void ResetGraphs();
    void AddLineGraph(QString graphTitle, std::vector<std::pair<QRgb, QString>>&& plots, QString xAxisTitle, QString yAxisTitle, std::function<void(uint64_t tick, LineGraph& graph)>&& task);
    void AddScatterGraph(QString graphTitle, QString xAxisTitle, QString yAxisTitle, std::function<void(uint64_t tick, ScatterGraph& graph)>&& task);
};
#endif // MAINWINDOW_H
