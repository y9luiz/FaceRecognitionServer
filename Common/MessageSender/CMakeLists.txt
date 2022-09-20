add_library(MessageSender
    STATIC 
        include/messageSenderInterface.h
        include/udpMessageSender.h
        src/udpMessageSender.cpp
        include/messageSenderFactory.h
        src/messageSenderFactory.cpp
)

add_library(${PROJECT_NAME}::MessageSender
    ALIAS
        MessageSender
)

target_include_directories(MessageSender
    PUBLIC 
        include/
    PRIVATE
        src/
)

target_link_libraries(MessageSender
    PRIVATE 
       ${PROJECT_NAME}::ApplicationMessages
       ${PROJECT_NAME}::UdpSocket
)
