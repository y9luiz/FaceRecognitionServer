add_subdirectory(SocketManager)
add_subdirectory(ApplicationMessages)


add_library(Common INTERFACE)
add_library(${PROJECT_NAME}::Common ALIAS Common)

target_link_libraries(Common INTERFACE ${PROJECT_NAME}::SocketManager ${PROJECT_NAME}::ApplicationMessages)