ENABLE_TESTING ()
FIND_PACKAGE (GTest REQUIRED)
INCLUDE_DIRECTORIES (${GTEST_INCLUDE_DIRS})

FILE (GLOB test_SRCS "*_test.cpp")

FOREACH(file ${test_SRCS})
    GET_FILENAME_COMPONENT (file_name ${file} NAME)
    STRING (REPLACE ".cpp" "" test_BIN ${file_name} )
    ADD_EXECUTABLE (${test_BIN} ${file})
    TARGET_LINK_LIBRARIES (${test_BIN} ${GTEST_BOTH_LIBRARIES})
    ADD_TEST (test ${test_BIN})
ENDFOREACH(file ${test_SRCS})
