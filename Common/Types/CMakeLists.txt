add_library(Types INTERFACE)

add_library(${PROJECT_NAME}::Types ALIAS Types)

target_include_directories(Types 
  INTERFACE
    ${CMAKE_CURRENT_SOURCE_DIR}
)