add_executable(test_endpoint test_endpoint.cpp)

target_link_libraries(test_endpoint 
    PRIVATE
        gmock_main 
        ${PROJECT_NAME}::Types
)

gtest_discover_tests(test_endpoint)