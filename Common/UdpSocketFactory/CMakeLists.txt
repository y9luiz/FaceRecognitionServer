add_library(UdpSocketFactory src/udpSocketFactory.cpp include/udpSocketFactory.h)
add_library(${PROJECT_NAME}::UdpSocketFactory ALIAS UdpSocketFactory)

target_include_directories(UdpSocketFactory PUBLIC include)

add_subdirectory(test)
add_subdirectory(mock)