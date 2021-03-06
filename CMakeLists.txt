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

# Nlhomann JSON library
set(JsonLibDir "Libs/nlohmann")
file(MAKE_DIRECTORY ${JsonLibDir})
file(DOWNLOAD https://raw.githubusercontent.com/nlohmann/json/develop/single_include/nlohmann/json.hpp ${CMAKE_SOURCE_DIR}/${JsonLibDir}/json.hpp)

set(PROJECT_SOURCES
        main.cpp
        MainWindow.cpp
        Effectors/Effector.cpp
        Effectors/EffectorFilterMouth.cpp
        Effectors/EffectorProboscisMouth.cpp
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
        Genome/GeneEffectorProboscisMouth.cpp
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
        LineGraphContainerWidget.cpp
        LineGraph.cpp
        MeatChunk.cpp
        NeuralNetwork.cpp
        NeuralNetworkConnector.cpp
        NeuralNetworkInspector.cpp
        QuadTree.cpp
        ScatterGraph.cpp
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
        Utility/JsonHelpers.cpp
        Utility/RangeConverter.cpp
        Utils.cpp
)

set(PROJECT_HEADERS
        MainWindow.h
        Effectors/Effector.h
        Effectors/EffectorFilterMouth.h
        Effectors/EffectorProboscisMouth.h
        Effectors/EffectorSpike.h
        Effectors/EffectorSpringTail.h
        Effectors/EffectorTail.h
        Egg.h
        Energy.h
        Entity.h
        EntityContainerInterface.h
        FeedDispenser.h
        FoodPellet.h
        Genome/ChromosomePair.h
        Genome/Gene.h
        Genome/GeneBrain.h
        Genome/GeneEffectorBase.h
        Genome/GeneEffectorFilterMouth.h
        Genome/GeneEffectorProboscisMouth.h
        Genome/GeneEffectorSpike.h
        Genome/GeneEffectorSpringTail.h
        Genome/GeneEffectorTail.h
        Genome/GeneFactory.h
        Genome/GenePigment.h
        Genome/GeneSenseBase.h
        Genome/GeneSenseLunarCycle.h
        Genome/GeneSenseMagneticField.h
        Genome/GeneSenseRandom.h
        Genome/GeneSenseSine.h
        Genome/GeneSenseTraitsBase.h
        Genome/GeneSenseTraitsInArea.h
        Genome/GeneSenseTraitsRaycast.h
        Genome/GeneSenseTraitsSelf.h
        Genome/GeneSenseTraitsTouching.h
        Genome/Genome.h
        Genome/Phenotype.h
        LineGraphContainerWidget.h
        LineGraph.h
        MeatChunk.h
        NeuralNetwork.h
        NeuralNetworkConnector.h
        NeuralNetworkInspector.h
        QuadTree.h
        Random.h
        ScatterGraph.h
        Sensors/Sense.h
        Sensors/SenseLunarCycle.h
        Sensors/SenseMagneticField.h
        Sensors/SenseRandom.h
        Sensors/SenseSine.h
        Sensors/SenseTraitsBase.h
        Sensors/SenseTraitsInArea.h
        Sensors/SenseTraitsRaycast.h
        Sensors/SenseTraitsSelf.h
        Sensors/SenseTraitsTouching.h
        Shape.h
        Spike.h
        Swimmer.h
        Transform.h
        Universe.h
        UniverseParameters.h
        UniverseWidget.h
        Utility/JsonHelpers.h
        Utility/Range.h
        Utility/RangeConverter.h
        Utils.h
        ${JsonLibDir}/json.hpp
)

set(PROJECT_DESIGNER_FILES
        MainWindow.ui
)

set(EXTRA_FILES
        README.md
        How-to-create-Windows-release.txt
)

# This line is just to make these files appear in QtCreator
add_custom_target(Other SOURCES ${EXTRA_FILES})

qt_add_executable(Trilobytes
    ${PROJECT_SOURCES}
    ${PROJECT_HEADERS}
    ${PROJECT_DESIGNER_FILES}
)

target_link_libraries(Trilobytes PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
