cmake_minimum_required(VERSION 3.14)
project(berlekamp C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -g")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

find_package(Threads REQUIRED)
add_executable(${PROJECT_NAME} main.cpp Generator.cpp Checker.cpp Polynomial.cpp)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
