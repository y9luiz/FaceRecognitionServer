add_library(MessageReceiver 
    STATIC 
        include/messageReceiverInterface.h
        include/parallelMessageReceiver.h
        src/parallelMessageReceiver.cpp
        src/udpMessageReceiver.cpp
        src/udpMessageReceiver.h
        src/udpParallelMessageReceiver.cpp
        src/udpParallelMessageReceiver.h
        include/messageReceiverFactory.h
        src/messageReceiverFactory.cpp
)

add_library(${PROJECT_NAME}::MessageReceiver
    ALIAS
        MessageReceiver
)

target_include_directories(MessageReceiver
    PUBLIC 
        include/
    PRIVATE
        src/
)

target_link_libraries(MessageReceiver
    PRIVATE 
       ${PROJECT_NAME}::ApplicationMessages
       ${PROJECT_NAME}::UdpSocket
)

add_subdirectory(test)
add_subdirectory(mock)