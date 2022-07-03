add_library(ApplicationMessages src/applicationMessages.cpp include/applicationMessages.h)
add_library(${PROJECT_NAME}::ApplicationMessages ALIAS ApplicationMessages)

target_include_directories(ApplicationMessages PUBLIC include)

add_subdirectory(test)