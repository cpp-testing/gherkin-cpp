project(gherkin-cpp)
cmake_minimum_required(VERSION 3.0)

option(BUILD_SHARED_LIBS "Build shared library." OFF)

file(GLOB_RECURSE GHERKIN_CPP_SRS
        src/*.cpp
)

file(GLOB_RECURSE GHERKIN_H_SRS
        include/*.hpp
)

ADD_LIBRARY(gherkin-cpp ${GHERKIN_CPP_SRS})
ADD_LIBRARY(gherkin-cpp::gherkin-cpp ALIAS gherkin-cpp)

TARGET_INCLUDE_DIRECTORIES(gherkin-cpp
        PUBLIC ${PROJECT_SOURCE_DIR}/include
)

ADD_SUBDIRECTORY(libs/fmem)
TARGET_LINK_LIBRARIES(gherkin-cpp PRIVATE fmem)

ADD_SUBDIRECTORY(libs/gherkin-c)
TARGET_LINK_LIBRARIES(gherkin-cpp PRIVATE gherkin)

ADD_SUBDIRECTORY(test)

install(TARGETS gherkin-cpp
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
        LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
        ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR})

install(FILES
        ${GHERKIN_H_SRS}
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/gherkin-cpp)