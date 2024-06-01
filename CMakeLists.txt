cmake_minimum_required(VERSION 3.25)

project(
  Hydrie
  VERSION 0.0.1
  DESCRIPTION "TODO"
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

set(EIGEN_BUILD_TESTING OFF)

include(cmake/CPM.cmake)
# cpmaddpackage("gh:catchorg/Catch2@3.4.0")
cpmaddpackage(
  GITLAB_REPOSITORY
  libeigen/eigen
  GIT_TAG
  3.4
  OPTIONS
  "EIGEN_BUILD_PKGCONFIG OFF"
  "EIGEN_BUILD_DOC OFF")

add_library(hydrie_hydrie INTERFACE)
add_library(hydrie::hydrie ALIAS hydrie_hydrie)

set_property(TARGET hydrie_hydrie PROPERTY EXPORT_NAME hydrie)

target_include_directories(
  hydrie_hydrie INTERFACE "\$<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>")

target_compile_features(hydrie_hydrie INTERFACE cxx_std_23)

add_executable(minimal examples/minimal.cpp)
target_link_libraries(minimal hydrie::hydrie eigen)
target_compile_features(minimal INTERFACE cxx_std_23)
