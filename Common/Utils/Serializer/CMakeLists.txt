add_library(Serializer include/serializer.h src/serializer.cpp)

add_library(${PROJECT_NAME}::Serializer
    ALIAS
        Serializer
)

target_include_directories(Serializer
    PUBLIC
        include
)

target_link_libraries(Serializer
    PUBLIC
        ${OpenCV_LIBS}
        ${PROJECT_NAME}::Types
)

add_subdirectory(test)