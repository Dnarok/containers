cmake_minimum_required(VERSION 3.20)

set(MY_PROJECT_NAME containers)

project(
    ${MY_PROJECT_NAME}
    LANGUAGES CXX
    VERSION   1.0
)

set(CMAKE_CXX_STANDARD 23)

option(BUILD_TESTS "Build the tests" ON)
add_subdirectory(external)

add_library(${MY_PROJECT_NAME} INTERFACE
    include/containers/dynamic_buffer.hpp
)

target_include_directories(${MY_PROJECT_NAME}
    INTERFACE include
)

target_link_libraries(${MY_PROJECT_NAME}
    INTERFACE spdlog::spdlog
    INTERFACE fmt::fmt
    INTERFACE contract::contract
)

if(BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()