add_library(MessageReceiver 
    STATIC 
        src/messageReceiverInterface.cpp
        include/messageReceiverInterface.h
        src/udpMessageReceiver.cpp
        src/udpMessageReceiver.h
        src/messageReceiverBuilder.cpp
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