add_library(ApplicationMessages
    src/applicationMessages.cpp
    include/applicationMessages.h
    src/faceDetectionRequest.cpp
    include/faceDetectionRequest.h
)

add_library(${PROJECT_NAME}::ApplicationMessages ALIAS ApplicationMessages)

target_link_libraries(ApplicationMessages PUBLIC Serializer)
target_include_directories(ApplicationMessages PUBLIC include)

add_subdirectory(test)
add_subdirectory(mock)