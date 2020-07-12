cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(micro_hardware_monitor)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# find_package(SDL2 REQUIRED)
include_directories(src include)

add_executable(${PROJECT_NAME} ./src/main.cpp ./src/tinyxml2.cpp ./src/reader.cpp ./src/data_source.cpp)

target_link_libraries(${PROJECT_NAME})

