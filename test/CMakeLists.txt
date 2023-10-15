add_executable(unit_test unit_test.cpp)
target_compile_definitions(unit_test PRIVATE ${DEFAULT_COMPILE_DEFINITIONS})
target_link_libraries(unit_test ${HYDRIE_DEPS} doctest)
install(TARGETS unit_test DESTINATION ${PROJECT_SOURCE_DIR}/bin/test)
