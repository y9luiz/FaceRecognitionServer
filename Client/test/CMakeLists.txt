add_executable(test_udp_client test_udp_client.cpp)

target_link_libraries(test_udp_client 
    PUBLIC
        gmock_main 
        ${PROJECT_NAME}::Client
        ${PROJECT_NAME}::MockUdpSocketFactory
)

gtest_discover_tests(test_udp_client)