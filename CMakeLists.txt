cmake_minimum_required(VERSION 2.8)
project(ConstantineQAQ)

include (cmake/utils.cmake)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_FLAGS "-rdynamic -O3 -g -std=c++11 -Wall -Wno-deprecated -Wno-unused-function")

include_directories(.)

set(LIB_SRC
    src/log.cpp
    src/util.cpp
    src/config.cpp
    src/thread.cpp
    )

add_library(constantineQAQ SHARED ${LIB_SRC})
force_redefine_file_macro_for_sources(constantineQAQ) # __FILE__

find_package(Boost REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

set(
    LIB_LIB
    constantineQAQ
    pthread
    yaml-cpp
)

#add_library(constantineQAQ SHARED ${LIB_SRC})
#add_library(constantineQAQ_static STATIC ${LIB_SRC})
#SET_TARGET_PROPERTIES(constantineQAQ_static PROPERTIES OUTPUT_NAME "constantineQAQ")

add_executable(test tests/test.cpp)
add_dependencies(test constantineQAQ)
force_redefine_file_macro_for_sources(test) # __FILE__
target_link_libraries(test constantineQAQ ${LIB_LIB})

add_executable(test_config tests/test_config.cpp)
add_dependencies(test_config constantineQAQ)
force_redefine_file_macro_for_sources(test_config) # __FILE__
target_link_libraries(test_config constantineQAQ ${LIB_LIB})

add_executable(test_thread tests/test_thread.cpp)
add_dependencies(test_thread constantineQAQ)
force_redefine_file_macro_for_sources(test_thread) # __FILE__
target_link_libraries(test_thread constantineQAQ ${LIB_LIB})


SET(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)
