add_library(MockUdpSocket OBJECT mockUdpSocket.cpp mockUdpSocket.h)
add_library(${PROJECT_NAME}::MockUdpSocket ALIAS MockUdpSocket)

target_include_directories(MockUdpSocket 
    PUBLIC 
        ../include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MockUdpSocket
    PRIVATE
        gmock_main
    PUBLIC
        ${PROJECT_NAME}::Types
)