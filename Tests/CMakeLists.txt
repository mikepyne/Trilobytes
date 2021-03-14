INCLUDE_DIRECTORIES(
    ${PROJECT_SOURCE_DIR}
)

set(TEST_SOURCES
        TestShape.cpp
)

qt_add_executable(Tests
    main.cpp
    ${TEST_SOURCES}
)

target_link_libraries(Tests PRIVATE
    Catch
    Json
)
