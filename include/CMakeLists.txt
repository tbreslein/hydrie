add_library(hydrie INTERFACE hydrie.hpp)
# target_include_directories(hydrie PUBLIC .)

target_include_directories(
  hydrie
  INTERFACE # headers to include when building from source
            $<BUILD_INTERFACE:${CMakeDemo_SOURCE_DIR}/include>
            $<BUILD_INTERFACE:${CMakeDemo_BINARY_DIR}/include>
            # headers to include when installing (implicitly prefixes with
            # ${CMAKE_INSTALL_PREFIX}).
            $<INSTALL_INTERFACE:include>)
