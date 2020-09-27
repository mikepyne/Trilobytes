#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

    void SetSwimmerToInspect(Swimmer& swimmer, EntityContainerInterface& container);

private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
