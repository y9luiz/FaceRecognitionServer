add_library(Client
    STATIC
        include/udpClient.h
        include/faceDetectionUdpClient.h
        src/udpClient.cpp
        src/faceDetectionUdpClient.cpp
)

add_library(${PROJECT_NAME}::Client
    ALIAS
        Client
)


target_include_directories(Client
    PUBLIC
        include
)

target_link_libraries(Client 
    PUBLIC 
        ${Boost_LIBRARIES} 
        Threads::Threads
    PRIVATE
        ${PROJECT_NAME}::Common
)

add_subdirectory(test)