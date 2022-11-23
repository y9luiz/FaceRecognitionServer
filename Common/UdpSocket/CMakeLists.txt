add_library(UdpSocket 
    STATIC 
        src/udpSocket.cpp
        include/udpSocket.h
)

add_library(${PROJECT_NAME}::UdpSocket
    ALIAS
        UdpSocket
)

target_include_directories(UdpSocket
    PUBLIC 
        include/
)

target_link_libraries(UdpSocket
    PUBLIC
        ${PROJECT_NAME}::Types
)

add_subdirectory(mock)
add_subdirectory(test)