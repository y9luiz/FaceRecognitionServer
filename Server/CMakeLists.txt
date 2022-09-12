add_library(Server src/udpServer.cpp include/udpServer.h src/faceDetectionUdpServer.cpp include/faceDetectionUdpServer.h)
add_library(${PROJECT_NAME}::Server ALIAS Server)

add_subdirectory(MessageHandler)

target_include_directories(Server PUBLIC include)
target_link_libraries(Server  
    PUBLIC 
        ${Boost_LIBRARIES} 
        ${OpenCV_LIBS}
        Threads::Threads 
        ${PROJECT_NAME}::MessageHandler
        ${PROJECT_NAME}::Common
)