project(gherkin-cpp)
cmake_minimum_required(VERSION 3.0)

option(BUILD_SHARED_LIBS "Build shared library." OFF)

LIST(APPEND GHERKIN_CPP_SRS
        src/compiler.cpp
        src/parser.cpp
)

ADD_LIBRARY(gherkin-cpp ${GHERKIN_CPP_SRS})
ADD_LIBRARY(gherkin-cpp::gherkin-cpp ALIAS gherkin-cpp)

TARGET_INCLUDE_DIRECTORIES(gherkin-cpp
        PUBLIC ${PROJECT_SOURCE_DIR}/include
)

ADD_SUBDIRECTORY(libs/fmem)
TARGET_LINK_LIBRARIES(gherkin-cpp PUBLIC fmem)

ADD_SUBDIRECTORY(libs/gherkin-c)
TARGET_LINK_LIBRARIES(gherkin-cpp PUBLIC gherkin)

ADD_SUBDIRECTORY(test)
