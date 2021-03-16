cmake_minimum_required(VERSION 3.16)

project(Trilobytes LANGUAGES CXX)

include(FetchContent)

enable_testing()

set(CMAKE_INCLUDE_CURRENT_DIR ON)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#find_package(QT NAMES Qt6 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Widgets Gui REQUIRED)

include(json)
include(catch2)

set(EXTRA_FILES
        README.md
        How-to-create-Windows-release.txt
)

# This line is just to make these files appear in QtCreator
add_custom_target(Other SOURCES ${EXTRA_FILES})

add_subdirectory(Effectors)
add_subdirectory(Genome)
add_subdirectory(Sensors)
add_subdirectory(Utility)
add_subdirectory(Main)
add_subdirectory(Tests)
