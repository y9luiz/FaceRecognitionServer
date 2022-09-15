add_executable(test_serializer test_serializer.cpp)
target_link_libraries(test_serializer
    PRIVATE
        gmock_main
        ${PROJECT_NAME}::Serializer
)

gtest_discover_tests(test_serializer)
addToCoverageReport(test_serializer)
