cmake_minimum_required(VERSION 3.7)

add_definitions(-std=c++17)


set(CXX_FLAGS "-Wall -pthread")
set(CMAKE_CXX_FLAGS, "${CXX_FLAGS}")

add_definitions(-DWITHOUT_NUMPY)

OPTION(UT "If unit test is on" OFF)
if (UT)
set(MAIN_FILE "./test/main.cpp")
add_definitions(-DUT)
project(micro_hardware_monitor_ut)
else (UT)
set(MAIN_FILE "./src/main.cpp")
project(micro_hardware_monitor)
endif()

find_package(Threads REQUIRED)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")

include_directories(src include /usr/include/python3.8)

add_executable(${PROJECT_NAME} ${MAIN_FILE} ./src/tinyxml2.cpp ./src/read_write.cpp ./src/data_source.cpp ./src/transform.cpp)

target_link_libraries(${PROJECT_NAME} fftw3 Threads::Threads python3.8)

