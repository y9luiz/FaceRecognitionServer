add_library(MockMessageReceiver OBJECT mockMessageReceiver.cpp mockMessageReceiver.h)
add_library(${PROJECT_NAME}::MockMessageReceiver ALIAS MockMessageReceiver)

target_include_directories(MockMessageReceiver 
    PUBLIC 
        ../include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MockMessageReceiver
    PRIVATE
        ${PROJECT_NAME}::MockApplicationMessages
        gmock_main
)