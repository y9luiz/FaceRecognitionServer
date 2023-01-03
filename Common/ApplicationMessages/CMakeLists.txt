add_library(ApplicationMessages
    STATIC
        src/applicationMessages.cpp
        include/applicationMessages.h
        src/faceDetectionRequest.cpp
        include/faceDetectionRequest.h
        src/faceDetectionResponse.cpp
        include/faceDetectionResponse.h
        src/faceRecognitionRequest.cpp
        include/faceRecognitionRequest.h
        src/utils.cpp
        src/utils.h
)

add_library(${PROJECT_NAME}::ApplicationMessages
    ALIAS
        ApplicationMessages
)

target_link_libraries(ApplicationMessages
    PUBLIC
        Serializer
        ${PROJECT_NAME}::Types
)

target_include_directories(ApplicationMessages
    PUBLIC
        include
)

add_subdirectory(test)
add_subdirectory(mock)