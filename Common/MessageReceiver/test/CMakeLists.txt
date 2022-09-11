add_executable(test_message_receiver test_message_receiver.cpp)
target_link_libraries(test_message_receiver
    PRIVATE
        gmock_main
        ${PROJECT_NAME}::MessageReceiver
        ${PROJECT_NAME}::ApplicationMessages
        ${PROJECT_NAME}::MockUdpSocket
)

addToCoverageReport(test_message_receiver)
gtest_discover_tests(test_message_receiver)