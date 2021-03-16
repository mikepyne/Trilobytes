#include "MainWindow.h"

#include "Random.h"

#include <QApplication>

#include <time.h>

int main(int argc, char *argv[])
{
    auto seed = static_cast<unsigned long>(time(nullptr));
    // seed = 1606064410; // Good seed for fluctuating food/swimmer energy early on
    Random::Seed(seed);
    std::cout << "Seed: " << seed << std::endl;

    QApplication a(argc, argv);
    MainWindow w;
    w.setGeometry(0, 0, 1920, 1080);
    w.show();
    return a.exec();
}
