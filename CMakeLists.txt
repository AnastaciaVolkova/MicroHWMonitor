cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)

set(CXX_FLAGS "-Wall")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

project(micro_hardware_monitor)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

# find_package(SDL2 REQUIRED)
# include_directories(${SDL2_INCLUDE_DIRS} src)

add_executable(${PROJECT_NAME} main.cpp tinyxml2.cpp)

target_link_libraries(${PROJECT_NAME})

