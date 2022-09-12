add_library(Serializer include/serializer.h src/serializer.cpp)

target_include_directories(Serializer
    PUBLIC
        include
)

target_link_libraries(Serializer
    PUBLIC
        ${OpenCV_LIBS}
)