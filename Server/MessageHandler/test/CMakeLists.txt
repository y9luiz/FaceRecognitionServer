add_executable(test_message_handler test_message_handler.cpp)

target_link_libraries(test_message_handler
  PRIVATE
    gmock_main
    ${PROJECT_NAME}::MessageHandler
    ${PROJECT_NAME}::ApplicationMessages
    ${PROJECT_NAME}::Types
)

addToCoverageReport(test_message_handler)
gtest_discover_tests(test_message_handler)