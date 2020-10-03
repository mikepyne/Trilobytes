#include "MainWindow.h"

#include "Random.h"

#include <QApplication>

#include <time.h>

int main(int argc, char *argv[])
{
    auto seed = static_cast<unsigned long>(time(nullptr));
    Random::Seed(seed);
    std::cout << "Seed: " << seed << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.show();
    return a.exec();
}
