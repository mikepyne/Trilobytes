#include "MainWindow.h"

#include "Random.h"

#include <QApplication>

#include <time.h>

int main(int argc, char *argv[])
{
    Random::Seed(static_cast<unsigned long>(time(nullptr)));

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
