add_executable(Tests)

target_sources(Tests
    PUBLIC
    main.cpp
    TestShape.cpp
)

target_include_directories(Tests
    PUBLIC
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(Tests
    PRIVATE
    Catch2::Catch2
)

add_custom_command(TARGET Tests POST_BUILD
    COMMAND Tests
)
