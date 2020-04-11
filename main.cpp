#include "MainWindow.h"

#include "Random.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    Random::Seed();

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
