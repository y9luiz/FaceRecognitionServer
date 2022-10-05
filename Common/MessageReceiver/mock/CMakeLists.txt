add_library(MockMessageReceiver
    OBJECT
        mockMessageReceiver.cpp
        mockMessageReceiver.h
        mockMessageReceiverFactory.cpp
        mockMessageReceiverFactory.h
)

add_library(${PROJECT_NAME}::MockMessageReceiver
    ALIAS
        MockMessageReceiver
)

target_include_directories(MockMessageReceiver 
    PUBLIC 
        ../include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MockMessageReceiver
    PRIVATE
        gmock_main
        ${PROJECT_NAME}::MockApplicationMessages
)