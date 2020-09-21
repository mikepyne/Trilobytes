QT       += core gui widgets

CONFIG += c++17

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Effectors/Effector.cpp \
    Egg.cpp \
    EnergyPool.cpp \
    Entity.cpp \
    FeedDispenser.cpp \
    FoodPellet.cpp \
    Genome/ChromosomePair.cpp \
    Genome/Gene.cpp \
    Genome/GeneFactory.cpp \
    Genome/GenePigment.cpp \
    Genome/Genome.cpp \
    LineGraph.cpp \
    NeuralNetwork.cpp \
    NeuralNetworkConnector.cpp \
    NeuralNetworkInspector.cpp \
    QuadTree.cpp \
    Random.cpp \
    Sensors/Sense.cpp \
    Sensors/SenseEntitiesInArea.cpp \
    Sensors/SenseEntitiesTouching.cpp \
    Sensors/SenseEntityRaycast.cpp \
    Sensors/SenseMagneticField.cpp \
    Sensors/SenseRandom.cpp \
    Swimmer.cpp \
    Universe.cpp \
    Utils.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    Effectors/Effector.h \
    Egg.h \
    Energy.h \
    EnergyPool.h \
    Entity.h \
    EntityContainerInterface.h \
    FeedDispenser.h \
    FoodPellet.h \
    Genome/ChromosomePair.h \
    Genome/Gene.h \
    Genome/GeneFactory.h \
    Genome/GenePigment.h \
    Genome/Genome.h \
    LineGraph.h \
    MainWindow.h \
    NeuralNetwork.h \
    NeuralNetworkConnector.h \
    NeuralNetworkInspector.h \
    QuadTree.h \
    Random.h \
    Sensors/Sense.h \
    Sensors/SenseEntitiesInArea.h \
    Sensors/SenseEntitiesTouching.h \
    Sensors/SenseEntityRaycast.h \
    Sensors/SenseMagneticField.h \
    Sensors/SenseRandom.h \
    Shape.h \
    Swimmer.h \
    Universe.h \
    Utils.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
