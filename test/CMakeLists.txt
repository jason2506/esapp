find_package(GTest REQUIRED)

set(${PROJECT_NAME}_TESTS
)

include_directories(${GTEST_INCLUDE_DIRS})

enable_testing()
foreach(test ${${PROJECT_NAME}_TESTS})
    add_executable(${test} ${test}.cpp)

    target_link_libraries(${test}
        ${GTEST_BOTH_LIBRARIES}
        ${PROJECT_NAME}
    )

    add_test(NAME ${test}
        COMMAND ${test} --gtest_color=yes
    )
endforeach(test ${test_SRCS})
