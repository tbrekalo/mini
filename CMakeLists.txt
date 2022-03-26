cmake_minimum_required(VERSION 3.17)
project(mini VERSION 1.3.0 LANGUAGES C CXX)

option(MINI_BUILD_TESTS "Build tests" OFF)
option(MINI_DEV_PYTHON_BUILD "Build nanobind in dev mode" OFF)

add_library(mini STATIC ${PROJECT_SOURCE_DIR}/src/mini.cc)
target_include_directories(mini PUBLIC
  $<BUILD_INTERFACE:${CMAKE_CURRENT_LIST_DIR}/include>)
target_compile_features(mini PRIVATE cxx_std_17)
set_target_properties(mini PROPERTIES POSITION_INDEPENDENT_CODE ON)

if (MINI_BUILD_TESTS)
  include(${CMAKE_CURRENT_LIST_DIR}/test/Test.cmake)
endif ()

if (MINI_DEV_PYTHON_BUILD OR SKBUILD)
  if (SKBUILD)
    message(STATUS "Python_VERSION ${PYTHON_VERSION_STRING}")
    message(STATUS "Python_EXECUTABLE ${PYTHON_EXECUTABLE}")
    message(STATUS "Python_INCLUDE_DIR ${PYTHON_INCLUDE_DIR}")
    message(STATUS "Python_LIBRARIES ${PYTHON_LIBRARY}")
    set(Python_VERSION "${PYTHON_VERSION_STRING}")
    set(Python_EXECUTABLE "${PYTHON_EXECUTABLE}")
    set(Python_INCLUDE_DIR "${PYTHON_INCLUDE_DIR}")
    set(Python_LIBRARIES "${PYTHON_LIBRARY}")
  endif ()

  find_package(Python COMPONENTS Interpreter Development)
  include(FetchContent)
  FetchContent_Declare(
    nanobind 
    GIT_REPOSITORY https://github.com/wjakob/nanobind.git
    GIT_TAG master)
  
  FetchContent_MakeAvailable(nanobind)

  nanobind_add_module(minipy_ext NB_STATIC src/bind.cc)
  target_link_libraries(minipy_ext PRIVATE mini)

  install(TARGETS minipy_ext LIBRARY DESTINATION .)
endif ()


