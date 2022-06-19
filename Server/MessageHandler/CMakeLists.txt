add_library(MessageHandler src/messageHandler.cpp include/messageHandler.h)
add_library(${PROJECT_NAME}::MessageHandler ALIAS MessageHandler)

target_include_directories(MessageHandler PUBLIC include)
target_link_libraries(MessageHandler ${Boost_LIBRARIES})