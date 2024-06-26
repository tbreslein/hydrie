cmake_minimum_required(VERSION 3.25)

project(
  Hydrie
  VERSION 0.0.1
  DESCRIPTION "TODO"
  LANGUAGES CXX)

option(HYDRIE_TEST "Whether to build Hydrie's test suite" OFF)
option(HYDRIE_EXAMPLE
       "Which example to build; defaults to NONE, so no example is being built"
       "NONE")

set(CMAKE_CXX_STANDARD 23)
if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

set(EIGEN_BUILD_TESTING OFF)

include(cmake/CPM.cmake)
# cpmaddpackage("gh:snaipe/criterion@2.4.2")
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

if(NOT HYDRIE_EXAMPLE STREQUAL "NONE")
  add_executable("${HYDRIE_EXAMPLE}" "examples/${HYDRIE_EXAMPLE}.cpp")
  target_link_libraries("${HYDRIE_EXAMPLE}" hydrie::hydrie eigen)
  target_compile_features("${HYDRIE_EXAMPLE}" INTERFACE cxx_std_23)
endif()
