cmake_minimum_required(VERSION 3.16)
project(mini VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(MINIPY_EXT_DIR "Specify python extension install dir" OFF)

find_package(pybind11 QUIET)
if (NOT pybind11_FOUND)
  include(FetchContent)  
  FetchContent_Declare(
    pybind11
    GIT_REPOSITORY https://github.com/pybind/pybind11
    GIT_TAG v2.9.1)

    FetchContent_MakeAvailable(pybind11)
endif ()

pybind11_add_module(minipy ${PROJECT_SOURCE_DIR}/src/bind.cc)
if (MINIPY_EXT_DIR) 
  install(TARGETS minipy
          LIBRARY DESTINATION ${MINIPY_EXT_DIR})
endif()
