add_library(SocketManager src/socketManager.cpp include/socketManager.h)
add_library(${PROJECT_NAME}::SocketManager ALIAS SocketManager)

target_include_directories(SocketManager PUBLIC include)

add_subdirectory(test)