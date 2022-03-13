cmake_minimum_required(VERSION 3.16)
project(mini VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MINI_BUILD_TESTS "Build tests" OFF)
option(MINI_BUILD_PYTHON "Build python bindings for minimizer extraction" OFF)

add_library(mini ${PROJECT_SOURCE_DIR}/src/mini.cc)
add_library(${PROJECT_NAME}::mini ALIAS mini)
target_include_directories(mini PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>
  $<INSTALL_INTERFACE:include>)

if (MINI_BUILD_TESTS)
  include(${CMAKE_CURRENT_LIST_DIR}/test/Test.cmake)
endif ()

if (BUILD_PYTHON)
endif ()
