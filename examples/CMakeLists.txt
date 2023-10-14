foreach(name ${EXAMPLES})
  add_executable(${name} "${CMAKE_CURRENT_SOURCE_DIR}/${name}.cpp")
  target_compile_definitions(${name} PRIVATE ${DEFAULT_COMPILE_DEFINITIONS})
  set_property(TARGET ${name} PROPERTY CXX_STANDARD 23)
  target_link_libraries(
    ${name}
    PUBLIC hydrie
    PRIVATE ${HYDRIE_DEPS})
  install(TARGETS ${name} DESTINATION ${PROJECT_SOURCE_DIR}/bin/examples)
endforeach()
