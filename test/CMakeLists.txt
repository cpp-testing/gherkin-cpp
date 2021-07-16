ADD_EXECUTABLE(gherkin-cpp-test main.cpp)
TARGET_LINK_LIBRARIES(gherkin-cpp-test PRIVATE gherkin-cpp)

FILE(COPY testdata DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
ADD_CUSTOM_TARGET(gherkin-cpp-test.run DEPENDS gherkin-cpp-test)
ADD_CUSTOM_COMMAND(
        TARGET gherkin-cpp-test.run
        POST_BUILD
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run_tests.py
        WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
        COMMENT "Running the tests"
)