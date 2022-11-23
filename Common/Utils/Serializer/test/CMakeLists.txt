add_executable(test_serializer test_serializer.cpp)
target_link_libraries(test_serializer
    PRIVATE
        gmock_main
        ${PROJECT_NAME}::Serializer
        Assets
)

gtest_discover_tests(test_serializer)
