add_library(Client include/udpClient.h src/udpClient.cpp)
add_library(${PROJECT_NAME}::Client ALIAS Client)


target_include_directories(Client PUBLIC include)

target_link_libraries(Client 
    PUBLIC 
        ${Boost_LIBRARIES} 
        Threads::Threads
    PRIVATE
        ${PROJECT_NAME}::Common
)

add_subdirectory(test)