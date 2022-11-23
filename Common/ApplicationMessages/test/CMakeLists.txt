add_executable(test_application_messages test_application_messages.cpp)

target_link_libraries(test_application_messages
  PRIVATE
    gmock_main
    ${PROJECT_NAME}::ApplicationMessages
    ${PROJECT_NAME}::Assets
)

gtest_discover_tests(test_application_messages)