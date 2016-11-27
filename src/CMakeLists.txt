find_package(literator CONFIG REQUIRED)
find_package(desa CONFIG REQUIRED)

add_library(${PROJECT_NAME}
    segmenter.cpp
)

target_link_libraries(${PROJECT_NAME} literator desa)

target_include_directories(${PROJECT_NAME}
    PUBLIC
        $<BUILD_INTERFACE:${${PROJECT_NAME}_INCLUDE_DIR}>
        $<INSTALL_INTERFACE:include>
)

set_target_properties(${PROJECT_NAME}
    PROPERTIES
        VERSION ${PROJECT_VERSION}
        OUTPUT_NAME ${PROJECT_NAME}
        CLEAN_DIRECT_OUTPUT 1
)

install(TARGETS ${PROJECT_NAME}
    EXPORT ${PROJECT_NAME}Targets
    ARCHIVE DESTINATION lib
    LIBRARY DESTINATION lib
    RUNTIME DESTINATION bin
    INCLUDES DESTINATION include
)
