cmake_minimum_required(VERSION 3.5)

project(Trilobytes LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt${QT_VERSION_MAJOR} COMPONENTS Widgets REQUIRED)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        Effectors/Effector.cpp
        Effectors/EffectorFilterMouth.cpp
        Effectors/EffectorSpike.cpp
        Effectors/EffectorSpringTail.cpp
        Effectors/EffectorTail.cpp
        Egg.cpp
        Entity.cpp
        FeedDispenser.cpp
        FoodPellet.cpp
        Genome/ChromosomePair.cpp
        Genome/Gene.cpp
        Genome/GeneBrain.cpp
        Genome/GeneEffectorBase.cpp
        Genome/GeneEffectorFilterMouth.cpp
        Genome/GeneEffectorSpike.cpp
        Genome/GeneEffectorSpringTail.cpp
        Genome/GeneEffectorTail.cpp
        Genome/GeneFactory.cpp
        Genome/GenePigment.cpp
        Genome/GeneSenseBase.cpp
        Genome/GeneSenseLunarCycle.cpp
        Genome/GeneSenseMagneticField.cpp
        Genome/GeneSenseRandom.cpp
        Genome/GeneSenseSine.cpp
        Genome/GeneSenseTraitsBase.cpp
        Genome/GeneSenseTraitsInArea.cpp
        Genome/GeneSenseTraitsRaycast.cpp
        Genome/GeneSenseTraitsSelf.cpp
        Genome/GeneSenseTraitsTouching.cpp
        Genome/Genome.cpp
        GraphContainerWidget.cpp
        LineGraph.cpp
        MeatChunk.cpp
        NeuralNetwork.cpp
        NeuralNetworkConnector.cpp
        NeuralNetworkInspector.cpp
        QuadTree.cpp
        Sensors/Sense.cpp
        Sensors/SenseLunarCycle.cpp
        Sensors/SenseMagneticField.cpp
        Sensors/SenseRandom.cpp
        Sensors/SenseSine.cpp
        Sensors/SenseTraitsBase.cpp
        Sensors/SenseTraitsInArea.cpp
        Sensors/SenseTraitsRaycast.cpp
        Sensors/SenseTraitsSelf.cpp
        Sensors/SenseTraitsTouching.cpp
        Spike.cpp
        Swimmer.cpp
        Transform.cpp
        Universe.cpp
        UniverseWidget.cpp
        Utils.cpp
)

# This line is just to make these files appear in QtCreator
add_custom_target(Other SOURCES README.md How-to-create-Windows-release.txt)

qt_add_executable(Trilobytes
    ${PROJECT_SOURCES}
)

target_link_libraries(Trilobytes PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
