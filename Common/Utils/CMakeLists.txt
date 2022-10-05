add_subdirectory(Serializer)

add_library(Utils INTERFACE)
add_library(${PROJECT_NAME}::Utils
    ALIAS
        Utils
)

target_link_libraries(Utils
    INTERFACE
        ${PROJECT_NAME}::Serializer
        ${PROJECT_NAME}::Types
)