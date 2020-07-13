cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)


set(CXX_FLAGS "-Wall -pthread")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(micro_hardware_monitor)

find_package(Threads REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories(src include)

add_executable(${PROJECT_NAME} ./src/main.cpp ./src/tinyxml2.cpp ./src/read_write.cpp ./src/data_source.cpp ./src/transform.cpp)

target_link_libraries(${PROJECT_NAME} fftw3 Threads::Threads)

