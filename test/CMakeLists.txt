add_executable(Tests)

target_sources(Tests
    PUBLIC
    main.cpp
    TestShape.cpp
)

target_include_directories(Tests
    PUBLIC
    ${PROJECT_SOURCE_DIR}
    ${PROJECT_SOURCE_DIR}/src
)

target_link_libraries(Tests
    PRIVATE
    Catch2::Catch2
)
