add_library(MessageSender
    STATIC 
        include/udpMessageSender.h
        src/udpMessageSender.cpp
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

add_subdirectory(test)
add_subdirectory(mock)