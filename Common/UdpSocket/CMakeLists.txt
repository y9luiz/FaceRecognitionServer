add_library(UdpSocket 
    STATIC 
        src/udpSocket.cpp
        include/udpSocket.h
)

add_library(${PROJECT_NAME}::UdpSocket
    ALIAS
        UdpSocket
)

target_include_directories(
    UdpSocket 
    PUBLIC 
        include/
)

add_subdirectory(mock)