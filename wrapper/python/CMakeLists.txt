cmake_minimum_required(VERSION 3.1)

project(ESA++Python
    VERSION 0.1.0
    LANGUAGES CXX
)

find_package(pybind11 CONFIG REQUIRED)

pybind11_add_module(${PROJECT_NAME} esapp.cpp)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        OUTPUT_NAME "esapp_python"
)

target_link_libraries(${PROJECT_NAME}
    PRIVATE
        ESA++
        pybind11::module
)
