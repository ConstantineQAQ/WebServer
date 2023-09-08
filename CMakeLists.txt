cmake_minimum_required(VERSION 2.8)
project(ConstantineQAQ)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Werror -Wno-unused-function")

set(LIB_SRC
    src/log.cpp
    )

add_library(constantineQAQ SHARED ${LIB_SRC})
#add_library(constantineQAQ_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(constantineQAQ_static PROPERTIES OUTPUT_NAME "constantineQAQ")

add_executable(test tests/test.cpp)
add_dependencies(test constantineQAQ)
target_link_libraries(test constantineQAQ)

SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)