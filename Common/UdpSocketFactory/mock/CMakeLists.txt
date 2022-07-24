add_library(MockUdpSocketFactory OBJECT mockUdpSocketFactory.cpp mockUdpSocketFactory.h)
add_library(${PROJECT_NAME}::MockUdpSocketFactory ALIAS MockUdpSocketFactory)

target_include_directories(MockUdpSocketFactory 
    PUBLIC 
        ../include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MockUdpSocketFactory
    PRIVATE
        gmock_main
)