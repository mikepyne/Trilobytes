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
    Effectors/EffectorTail.cpp \
    Egg.cpp \
    Entity.cpp \
    FeedDispenser.cpp \
    FoodPellet.cpp \
    Genome/ChromosomePair.cpp \
    Genome/Gene.cpp \
    Genome/GeneBrain.cpp \
    Genome/GeneEffectorBase.cpp \
    Genome/GeneEffectorTail.cpp \
    Genome/GeneFactory.cpp \
    Genome/GenePigment.cpp \
    Genome/GeneSenseBase.cpp \
    Genome/GeneSenseMagneticField.cpp \
    Genome/GeneSenseRandom.cpp \
    Genome/GeneSenseSine.cpp \
    Genome/GeneSenseTraitsBase.cpp \
    Genome/GeneSenseTraitsInArea.cpp \
    Genome/GeneSenseTraitsRaycast.cpp \
    Genome/GeneSenseTraitsSelf.cpp \
    Genome/GeneSenseTraitsTouching.cpp \
    Genome/Genome.cpp \
    GraphContainerWidget.cpp \
    LineGraph.cpp \
    MeatChunk.cpp \
    NeuralNetwork.cpp \
    NeuralNetworkConnector.cpp \
    NeuralNetworkInspector.cpp \
    QuadTree.cpp \
    Sensors/Sense.cpp \
    Sensors/SenseMagneticField.cpp \
    Sensors/SenseRandom.cpp \
    Sensors/SenseSine.cpp \
    Sensors/SenseTraitsBase.cpp \
    Sensors/SenseTraitsInArea.cpp \
    Sensors/SenseTraitsRaycast.cpp \
    Sensors/SenseTraitsSelf.cpp \
    Sensors/SenseTraitsTouching.cpp \
    Spike.cpp \
    Swimmer.cpp \
    Transform.cpp \
    Universe.cpp \
    UniverseWidget.cpp \
    Utils.cpp \
    main.cpp \
    MainWindow.cpp

HEADERS += \
    Effectors/Effector.h \
    Effectors/EffectorTail.h \
    Egg.h \
    Energy.h \
    Entity.h \
    EntityContainerInterface.h \
    FeedDispenser.h \
    FoodPellet.h \
    Genome/ChromosomePair.h \
    Genome/Gene.h \
    Genome/GeneBrain.h \
    Genome/GeneEffectorBase.h \
    Genome/GeneEffectorTail.h \
    Genome/GeneFactory.h \
    Genome/GenePigment.h \
    Genome/GeneSenseBase.h \
    Genome/GeneSenseMagneticField.h \
    Genome/GeneSenseRandom.h \
    Genome/GeneSenseSine.h \
    Genome/GeneSenseTraitsBase.h \
    Genome/GeneSenseTraitsInArea.h \
    Genome/GeneSenseTraitsRaycast.h \
    Genome/GeneSenseTraitsSelf.h \
    Genome/GeneSenseTraitsTouching.h \
    Genome/Genome.h \
    Genome/Phenotype.h \
    GraphContainerWidget.h \
    LineGraph.h \
    MainWindow.h \
    MeatChunk.h \
    NeuralNetwork.h \
    NeuralNetworkConnector.h \
    NeuralNetworkInspector.h \
    QuadTree.h \
    Random.h \
    Sensors/Sense.h \
    Sensors/SenseMagneticField.h \
    Sensors/SenseRandom.h \
    Sensors/SenseSine.h \
    Sensors/SenseTraitsBase.h \
    Sensors/SenseTraitsInArea.h \
    Sensors/SenseTraitsRaycast.h \
    Sensors/SenseTraitsSelf.h \
    Sensors/SenseTraitsTouching.h \
    Shape.h \
    Spike.h \
    Swimmer.h \
    Transform.h \
    Universe.h \
    UniverseWidget.h \
    Utils.h

FORMS += \
    MainWindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

DISTFILES += \
    How-to-create-Windows-release.txt \
    README.md
