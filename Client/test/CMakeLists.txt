add_executable(test_udp_client test_udp_client.cpp)

target_link_libraries(test_udp_client 
    PUBLIC
        gmock_main 
        ${PROJECT_NAME}::Client
        ${PROJECT_NAME}::MockUdpSocket
        ${PROJECT_NAME}::MockApplicationMessages
        ${PROJECT_NAME}::MockMessageReceiver
        ${PROJECT_NAME}::MessageSender
)

addToCoverageReport(test_udp_client)
gtest_discover_tests(test_udp_client)