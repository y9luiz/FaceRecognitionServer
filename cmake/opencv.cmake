find_package(OpenCV 4.5.4 QUIET)
if(NOT ${OpenCV_FOUND})
  find_package(OpenCV 5 REQUIRED)
endif()

include_directories(${OpenCV_INCLUDE_DIRS})