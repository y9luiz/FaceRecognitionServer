add_library(MockApplicationMessages
    OBJECT
        mockApplicationMessages.cpp
        mockApplicationMessages.h
)

add_library(${PROJECT_NAME}::MockApplicationMessages
    ALIAS
        MockApplicationMessages
)

target_include_directories(MockApplicationMessages 
    PUBLIC 
        ../include
        ${CMAKE_CURRENT_SOURCE_DIR}
)

target_link_libraries(MockApplicationMessages
    PRIVATE
        gmock_main
    PUBLIC
        ${PROJECT_NAME}::Types
)