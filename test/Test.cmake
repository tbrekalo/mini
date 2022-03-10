find_package(Catch2 3 QUIET)
if (NOT Catch2_FOUND)
  include(FetchContent) 
  FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.0.0-preview4)

  FetchContent_MakeAvailable(Catch2)
  list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
endif ()

add_executable(mini_test ${CMAKE_CURRENT_LIST_DIR}/main.cc)
target_link_libraries(mini_test 
  PRIVATE
    mini
    Catch2::Catch2WithMain)
