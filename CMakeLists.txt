cmake_minimum_required(VERSION 3.16)
project(FaceRecognitionServer)

SET(CMAKE_CXX_STANDARD 17)

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

add_subdirectory(Server)

add_executable(server main.cpp)
target_link_libraries(server ${PROJECT_NAME}::Server ${Boost_LIBRARIES} Threads::Threads)