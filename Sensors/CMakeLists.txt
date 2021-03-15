add_library(sensors OBJECT)

target_sources(sensors
    PUBLIC
    Sense.cpp
    SenseLunarCycle.cpp
    SenseMagneticField.cpp
    SenseRandom.cpp
    SenseSine.cpp
    SenseTraitsBase.cpp
    SenseTraitsInArea.cpp
    SenseTraitsRaycast.cpp
    SenseTraitsSelf.cpp
    SenseTraitsTouching.cpp
)

target_include_directories(sensors
    PUBLIC
    ${PROJECT_SOURCE_DIR}/Main
    ${PROJECT_SOURCE_DIR}/Utility
    ${PROJECT_SOURCE_DIR}/Genome
    ${PROJECT_SOURCE_DIR}/Effectors
)

target_link_libraries(sensors
    PUBLIC
    Qt5::Widgets
    utility
)
