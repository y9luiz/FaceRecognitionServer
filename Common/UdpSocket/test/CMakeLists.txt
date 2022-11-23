add_executable(test_udp_socket test_udp_socket.cpp)
target_link_libraries(test_udp_socket gmock_main ${PROJECT_NAME}::UdpSocket)

gtest_discover_tests(test_udp_socket)
