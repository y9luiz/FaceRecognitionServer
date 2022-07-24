add_subdirectory(UdpSocketFactory)
add_subdirectory(ApplicationMessages)


add_library(Common INTERFACE)
add_library(${PROJECT_NAME}::Common ALIAS Common)

target_link_libraries(Common INTERFACE ${PROJECT_NAME}::UdpSocketFactory ${PROJECT_NAME}::ApplicationMessages)