cmake_minimum_required(VERSION 3.1)

project(esapp
    VERSION 0.4.1
    LANGUAGES CXX
)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/src)
install(DIRECTORY ${${PROJECT_NAME}_INCLUDE_DIR}/
    DESTINATION include
    PATTERN *.hpp
)

add_subdirectory(src)
add_subdirectory(example)

include(CTest)
if(BUILD_TESTING)
    add_subdirectory(test)
endif(BUILD_TESTING)

include(cmake/esappPackage.cmake)
