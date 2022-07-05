cmake_minimum_required(VERSION 3.16)
project(FaceRecognitionServer)

SET(CMAKE_CXX_STANDARD 17)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

find_package(Boost REQUIRED)
find_package(Threads REQUIRED)

if(Boost_FOUND)
  include_directories(${Boost_INCLUDE_DIRS})
endif()

include(gtest)
include(coverage)

add_subdirectory(Common)
add_subdirectory(Server)
add_subdirectory(Client)

add_executable(${PROJECT_NAME} server.cpp)
target_link_libraries(${PROJECT_NAME} ${PROJECT_NAME}::Server)

add_executable(FaceRecognitionClient client.cpp)
target_link_libraries(FaceRecognitionClient ${PROJECT_NAME}::Client)
