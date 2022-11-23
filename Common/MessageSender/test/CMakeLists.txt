add_executable(test_message_sender test_message_sender.cpp)
target_link_libraries(test_message_sender
    PRIVATE
        gmock_main
        ${PROJECT_NAME}::MessageSender
        ${PROJECT_NAME}::Serializer
        ${PROJECT_NAME}::MockApplicationMessages
        ${PROJECT_NAME}::MockUdpSocket
)

gtest_discover_tests(test_message_sender)