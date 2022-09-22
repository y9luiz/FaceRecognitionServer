add_library(Types
  STATIC
    include/endpoint.h
    src/endpoint.cpp
)

add_library(${PROJECT_NAME}::Types
  ALIAS
    Types
)

target_include_directories(Types 
  PUBLIC
    include
)

target_link_libraries(Types
  PRIVATE
    Serializer
)