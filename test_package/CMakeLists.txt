cmake_minimum_required(VERSION 3.1)

project(ESA++Test VERSION 0.1.0)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(ESA++ CONFIG REQUIRED)

add_executable(example example.cpp)
target_link_libraries(example ESA++::ESA++)
