add_executable(Trilobytes)

target_sources(Trilobytes
    PRIVATE
        MainWindow.cpp
        MainWindow.ui
        Egg.cpp
        Entity.cpp
        FeedDispenser.cpp
        FoodPellet.cpp
        LineGraphContainerWidget.cpp
        LineGraph.cpp
        MeatChunk.cpp
        NeuralNetwork.cpp
        NeuralNetworkConnector.cpp
        NeuralNetworkInspector.cpp
        QuadTree.cpp
        ScatterGraph.cpp
        Spike.cpp
        Swimmer.cpp
        Transform.cpp
        Universe.cpp
        UniverseWidget.cpp
        Utils.cpp
)

target_link_libraries(Trilobytes
    PRIVATE
    Qt5::Widgets
    effectors
    genome
    sensors
    utility
)

target_include_directories(Trilobytes
    PUBLIC
    {$PROJECT_SOURCE_DIR}/Effectors
    {$PROJECT_SOURCE_DIR}/Genome
    {$PROJECT_SOURCE_DIR}/Sensors
    {$PROJECT_SOURCE_DIR}/Utility
)

# Ensure our release contains our saved genomes
add_custom_command(TARGET Trilobytes POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${PROJECT_SOURCE_DIR}/SavedGenomes/ ${PROJECT_BINARY_DIR}/SavedGenomes/
)
