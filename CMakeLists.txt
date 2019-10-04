cmake_minimum_required(VERSION 3.14)
project(irrpolygf2 C CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
add_executable(${PROJECT_NAME} main.cpp Generator.cpp Checker.cpp Polynomial.cpp Random.cpp)
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
