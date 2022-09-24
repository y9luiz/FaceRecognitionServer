add_library(MessageHandler
  STATIC
    src/messageHandler.cpp
    include/messageHandler.h
)

add_library(${PROJECT_NAME}::MessageHandler
  ALIAS
    MessageHandler
)

target_include_directories(MessageHandler
  PUBLIC
    include
  )

target_link_libraries(MessageHandler
  PUBLIC
    ${Boost_LIBRARIES}
  PRIVATE
    ${PROJECT_NAME}::ApplicationMessages
    ${PROJECT_NAME}::Types
)

add_subdirectory(test)