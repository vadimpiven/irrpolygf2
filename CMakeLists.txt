cmake_minimum_required(VERSION 3.14)
project(irrpolygf2 C CXX)

set(CMAKE_CXX_STANDARD 17)

find_package(Threads REQUIRED)
file(GLOB SOURCES *.cpp *.hpp)
add_executable("${PROJECT_NAME}" "${SOURCES}")
target_link_libraries(${PROJECT_NAME} ${CMAKE_THREAD_LIBS_INIT})
