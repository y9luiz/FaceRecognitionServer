add_library(MockMessageSender
  STATIC
    mockUdpMessageSender.cpp
    mockUdpMessageSender.h
)
add_library(${PROJECT_NAME}::MockMessageSender
  ALIAS
    MockMessageSender
)

target_include_directories(MockMessageSender 
    PUBLIC 
        ../include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MockMessageSender
    PRIVATE
        gmock_main
        ${PROJECT_NAME}::MockApplicationMessages
        ${PROJECT_NAME}::MockUdpSocket
        ${PROJECT_NAME}::Types
)