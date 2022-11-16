cmake_minimum_required(VERSION 3.10)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(client VERSION 1.0)

set(INCLUDE_PATH ${CMAKE_CURRENT_SOURCE_DIR})
include_directories(${INCLUDE_PATH})
add_executable(client main.cpp BTSerialPortBinding.cpp DeviceINQ.cpp Enums.cpp)
target_link_libraries(client bluetooth)