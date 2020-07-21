cmake_minimum_required(VERSION 3.16)

enable_testing()

set(project_name http_test)

project(${project_name})

set(CMAKE_CXX_COMPILER "c++")
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS -g -Wall)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: " "${CMAKE_CXX_FLAGS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

message(STATUS "source dir: " ${PROJECT_SOURCE_DIR})
message(STATUS "binary dir: " ${PROJECT_BINARY_DIR})
message(STATUS "output dir: " "${PROJECT_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "all source files: " ${SOURCE_DIR})

include_directories(.)
aux_source_directory(. SOURCE_DIR)
link_directories(archive/)

set(static_lib_source_file http/http.cpp http/http.h)
add_library(http_static STATIC ${static_lib_source_file})


add_executable(trim_test test/1.trim_test.cc)
target_link_libraries(trim_test http_static)
add_test(trim_test ./bin/trim_test)
set_tests_properties(trim_test PROPERTIES PASS_REGULAR_EXPRESSION "pass")


add_executable(header_test test/2.header_test.cc)
target_link_libraries(header_test http_static)
add_test(header_test ./bin/header_test)
set_tests_properties(header_test PROPERTIES PASS_REGULAR_EXPRESSION "pass")

add_executable(request_test test/3.request_test.cc)
target_link_libraries(request_test http_static)
add_test(request_test ./bin/request_test)
set_tests_properties(request_test PROPERTIES PASS_REGULAR_EXPRESSION "pass")



