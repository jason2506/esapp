cmake_minimum_required(VERSION 3.1)

project(esapp
    VERSION 0.4.1
    LANGUAGES CXX
)

option(ENABLE_CONAN "Enable conan package manager" OFF)
if(ENABLE_CONAN)
    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
    conan_basic_setup()
endif(ENABLE_CONAN)

find_package(DICT CONFIG REQUIRED)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(${PROJECT_NAME}_INCLUDE_DIR ${PROJECT_SOURCE_DIR}/include)
install(DIRECTORY ${${PROJECT_NAME}_INCLUDE_DIR}/
    DESTINATION include
    PATTERN *.hpp
)

add_library(${PROJECT_NAME} INTERFACE)

target_link_libraries(${PROJECT_NAME} INTERFACE DICT::DICT)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        $<BUILD_INTERFACE:${${PROJECT_NAME}_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
)

add_subdirectory(wrapper)
add_subdirectory(example)

include(cmake/esappPackage.cmake)
