add_executable(test_message_receiver test_message_receiver.cpp)
target_link_libraries(test_message_receiver
    PRIVATE
        gmock_main
        ${PROJECT_NAME}::MessageReceiver
        ${PROJECT_NAME}::MockApplicationMessages
        ${PROJECT_NAME}::MockUdpSocket
)

gtest_discover_tests(test_message_receiver)