add_subdirectory(Types)
add_subdirectory(Utils)
add_subdirectory(UdpSocket)
add_subdirectory(ApplicationMessages)
add_subdirectory(MessageReceiver)
add_subdirectory(MessageSender)

add_library(Common INTERFACE)
add_library(${PROJECT_NAME}::Common ALIAS Common)

target_link_libraries(Common 
    INTERFACE 
        ${PROJECT_NAME}::UdpSocket
        ${PROJECT_NAME}::ApplicationMessages
        ${PROJECT_NAME}::MessageReceiver
        ${PROJECT_NAME}::MessageSender
        ${PROJECT_NAME}::Utils
)