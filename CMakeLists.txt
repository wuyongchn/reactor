cmake_minimum_required(VERSION 3.1)
project(echo_server)

set(echo_server_VERSION_MAJOR 1)
set(echo_server_VERSION_MINOR 0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRE TRUE)

find_package(Threads)

include_directories(${PROJECT_SOURCE_DIR})

add_subdirectory(reactor)
add_subdirectory(utils)

add_executable(server server.cc)
add_executable(client client.cc)
target_link_libraries(server reactor utils)
target_link_libraries(client ${CMAKE_THREAD_LIBS_INIT} utils ncurses)