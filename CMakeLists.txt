cmake_minimum_required(VERSION 3.26)

project(
  Hydrie
  VERSION 0.0.1
  DESCRIPTION "Riemann-based hydrodynamics solver"
  LANGUAGES CXX)

set(HYDIE_VERSION ${CMAKE_PROJECT_VERSION})

set(EXAMPLES
    ""
    CACHE STRING "; separated list of examples to compile")

set(BUILD_TEST
    OFF
    CACHE BOOL "whether to build the test binaries")

set(BUILD_BENCHES
    OFF
    CACHE BOOL "whether to build the bench binaries")

if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
  set(CMAKE_INSTALL_PREFIX
      "${CMAKE_CURRENT_SOURCE_DIR}/bin"
      CACHE
        PATH
        "Set install folder; defaults to bin subfolder in the project folder."
        FORCE)
endif()
set(CMAKE_INSTALL_MESSAGE NEVER)

# ##############################################################################
# custom targets
# ##############################################################################

file(
  GLOB_RECURSE
  ALL_SOURCE_FILES
  ${PROJECT_SOURCE_DIR}/examples/*.cpp
  ${PROJECT_SOURCE_DIR}/examples/*.hpp
  ${PROJECT_SOURCE_DIR}/test/*.cpp
  ${PROJECT_SOURCE_DIR}/test/*.hpp
  ${PROJECT_SOURCE_DIR}/include/*.hpp)

include(FindPackageHandleStandardArgs)
find_program(cppcheck_EXE NAMES cppcheck)
find_program(clang_format_EXE NAMES clang-format)

find_package_handle_standard_args(cppcheck REQUIRED_VARS cppcheck_EXE)
find_package_handle_standard_args(clang-format REQUIRED_VARS clang_format_EXE)

if(cppcheck_FOUND)
  add_custom_target(
    cppcheck
    COMMENT "run cppcheck un examples, test, and library"
    COMMAND
      --quiet -I ${PROJECT_SOURCE_DIR}/include/ --error-exitcode=1 --enable=all
      --check-config --suppress=missingIncludeSystem ${ALL_SOURCE_FILES})
endif()

if(clang_format_FOUND)
  add_custom_target(
    clang-format
    COMMENT "run clang-format"
    COMMAND clang-format -i ${ALL_SOURCE_FILES})
endif()

set(CMAKE_CONFIGURATION_TYPES "Release;RelWithDebInfo;Debug")
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(DEFAULT_COMPILE_DEFINITIONS SYSTEM_${SYSTEM_NAME_UPPER} INTERFACE
                                USE_INTERNAL_SIMD)

set(CMAKE_EXPORT_COMPILE_COMMANDS
    ON
    CACHE INTERNAL "Write compile_commands.json; needed for LSPs")

if(CMAKE_EXPORT_COMPILE_COMMANDS)
  set(CMAKE_CXX_STANDARD_INCLUDE_DIRECTORIES
      ${CMAKE_CXX_IMPLICIT_INCLUDE_DIRECTORIES})
endif()

# soft link for compile_commands
file(CREATE_LINK ${CMAKE_BINARY_DIR}/compile_commands.json
     ${CMAKE_CURRENT_LIST_DIR}/compile_commands.json SYMBOLIC)

add_library(hydrie_config INTERFACE)
if("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
  target_compile_options(
    hydrie_config
    INTERFACE ${DEFAULT_COMPILE_OPTIONS}
              -fdiagnostics-color=always
              -Wall
              -Wextra
              -Wextra-semi
              -Wcast-align
              -Wduplicated-branches
              -Wduplicated-cond
              -Wformat=2
              -Wformat-truncation
              -Wignored-qualifiers
              -Wlogical-op
              -Wmaybe-uninitialized
              -Wmissing-braces
              -Wmissing-field-initializers
              -Werror=missing-field-initializers
              -Wmisleading-indentation
              -Wnon-virtual-dtor
              -Woverloaded-virtual
              -Wpedantic
              -Wreorder
              -Wreturn-type
              -Wsuggest-override
              -Wuninitialized
              -Wunused
              -Wno-switch-enum
              -march=native
              -floop-parallelize-all
              -ftree-parallelize-loops=4)

elseif("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
  target_compile_options(
    hydrie_config
    INTERFACE ${DEFAULT_COMPILE_OPTIONS}
              -Wall
              -Wcomma
              -Wextra
              -Wformat=2
              -Wignored-qualifiers
              -Wmissing-braces
              -Wpedantic
              -Wreturn-type
              -Wuninitialized
              -Wcast-align
              -Wno-switch-enum
              -Wno-unused-parameter
              -Wno-sign-compare
              -Wno-sign-conversion
              -Wno-deprecated-declarations
              -Wno-unqualified-std-cast-call
              -fcolor-diagnostics
              -march=native)
endif()

include(cmake/CPM.cmake)

cpmaddpackage("gh:romeric/fastor#V0.6.4")
find_package(OpenMP REQUIRED)
set(HYDRIE_DEPS Fastor OpenMP::OpenMP_CXX hydrie_config)

add_library(hydrie INTERFACE src/hydrie.hpp)
target_include_directories(hydrie INTERFACE src)

add_subdirectory(examples)

if(${BUILD_TEST})
  cpmaddpackage("gh:doctest/doctest@2.4.11")
  add_subdirectory(test)
endif()
if(${BUILD_BENCHES})
  cpmaddpackage("gh:doctest/doctest@2.4.11")
  add_subdirectory(bench)
endif()
