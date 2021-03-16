add_library(genome OBJECT)

target_sources(genome
    PUBLIC
    ChromosomePair.cpp
    Gene.cpp
    GeneBrain.cpp
    GeneEffectorBase.cpp
    GeneEffectorFilterMouth.cpp
    GeneEffectorProboscisMouth.cpp
    GeneEffectorSpike.cpp
    GeneEffectorSpringTail.cpp
    GeneEffectorTail.cpp
    GeneFactory.cpp
    GenePigment.cpp
    GeneSenseBase.cpp
    GeneSenseLunarCycle.cpp
    GeneSenseMagneticField.cpp
    GeneSenseRandom.cpp
    GeneSenseSine.cpp
    GeneSenseTraitsBase.cpp
    GeneSenseTraitsInArea.cpp
    GeneSenseTraitsRaycast.cpp
    GeneSenseTraitsSelf.cpp
    GeneSenseTraitsTouching.cpp
    Genome.cpp
)

target_include_directories(genome
    PUBLIC
    ${PROJECT_SOURCE_DIR}/Main
    ${PROJECT_SOURCE_DIR}/Utility
    ${PROJECT_SOURCE_DIR}/Sensors
    ${PROJECT_SOURCE_DIR}/Effectors
)

target_link_libraries(genome
    PUBLIC
    utility
    sensors
)
