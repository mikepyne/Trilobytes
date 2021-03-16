add_library(effector OBJECT)

target_sources(effector
    PUBLIC
    Effector.cpp
    EffectorFilterMouth.cpp
    EffectorProboscisMouth.cpp
    EffectorSpike.cpp
    EffectorSpringTail.cpp
    EffectorTail.cpp
)

target_include_directories(effector
    PUBLIC
    ${PROJECT_SOURCE_DIR}/Main
    ${PROJECT_SOURCE_DIR}/Utility
)

target_include_directories(effector
    PUBLIC
    ${PROJECT_SOURCE_DIR}/Utility
    ${PROJECT_SOURCE_DIR}/Genome
    ${PROJECT_SOURCE_DIR}/Sensors
    ${PROJECT_SOURCE_DIR}/Main
)

target_link_libraries(effector
    PUBLIC
    Qt5::Widgets
    utility
)
