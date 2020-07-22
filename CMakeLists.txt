cmake_minimum_required(VERSION 3.16)

enable_testing()

set(project_name http_test)

project(${project_name})

set(CMAKE_CXX_COMPILER "c++")
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS -g -W)
string(REPLACE ";" " " CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
message(STATUS "CMAKE_CXX_FLAGS: " "${CMAKE_CXX_FLAGS}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/)

message(STATUS "source dir: " ${PROJECT_SOURCE_DIR})
message(STATUS "binary dir: " ${PROJECT_BINARY_DIR})
message(STATUS "output dir: " "${PROJECT_BINARY_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message(STATUS "all source files: " ${SOURCE_DIR})

include_directories(.)
include_directories(/usr/local/include)
aux_source_directory(. SOURCE_DIR)
link_directories(archive/)
link_directories(/usr/local/lib)

set(static_lib_source_file
        http/http_request.cc http/http_request.h
        http/header.cc http/header.h
        http/httputils.cc http/httputils.h
        http/errors.cc http/errors.h
        http/http_response.cc http/http_response.h
        )

add_library(http_static SHARED ${static_lib_source_file})

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

set(test4_name split_test)
set(test4_src test/4.split_test.cc)
add_executable(${test4_name} ${test4_src})
target_link_libraries(${test4_name} http_static)
add_test(${test4_name} ./bin/${test4_name})
set_tests_properties(${test4_name} PROPERTIES PASS_REGULAR_EXPRESSION "pass")


set(test5_name response_test)
set(test5_src test/5.response_test.cc)
add_executable(${test5_name} ${test5_src})
target_link_libraries(${test5_name} http_static)
add_test(${test5_name} ./bin/${test5_name})
set_tests_properties(${test5_name} PROPERTIES PASS_REGULAR_EXPRESSION "pass")