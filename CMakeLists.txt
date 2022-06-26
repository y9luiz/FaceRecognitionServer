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

add_subdirectory(Server)

add_executable(server main.cpp)
target_link_libraries(server ${PROJECT_NAME}::Server ${Boost_LIBRARIES} Threads::Threads)
