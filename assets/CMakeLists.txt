add_library(Assets INTERFACE)

add_library(${PROJECT_NAME}::Assets
  ALIAS
    Assets
)

target_include_directories(Assets
  INTERFACE
    include
)

include(assets.cmake)

configure_file (assets.h.in ${CMAKE_CURRENT_SOURCE_DIR}/include/assets.h @ONLY)
