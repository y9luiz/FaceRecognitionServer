cmake_minimum_required(VERSION 3.16)
project(FaceRecognitionServer)

SET(CMAKE_CXX_STANDARD 17)

if(NOT DEFINED CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug)
endif()

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

include(dependencies)

add_subdirectory(Common)
add_subdirectory(Server)
add_subdirectory(Client)

add_executable(server server.cpp)

target_link_libraries(server 
  PRIVATE
    ${PROJECT_NAME}::Server  
    ${PROJECT_NAME}::Common
)

add_executable(client client.cpp)

target_link_libraries(client 
  PRIVATE
    ${PROJECT_NAME}::Client
    ${PROJECT_NAME}::Common
)
