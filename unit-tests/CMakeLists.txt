add_executable(unit-tests unit-tests.cxx)

target_link_libraries(unit-tests PRIVATE ${GTEST_LIBRARIES} PRIVATE ${CMAKE_THREAD_LIBS_INIT})