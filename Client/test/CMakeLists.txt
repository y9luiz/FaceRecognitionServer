add_executable(test_udp_client test_udp_client.cpp)

target_link_libraries(test_udp_client 
    PRIVATE
        gmock_main 
        ${PROJECT_NAME}::ApplicationMessages
        ${PROJECT_NAME}::Client
        ${PROJECT_NAME}::MockUdpSocket
        ${PROJECT_NAME}::MockMessageReceiver
        ${PROJECT_NAME}::MockMessageSender
        ${PROJECT_NAME}::Types
)

gtest_discover_tests(test_udp_client)