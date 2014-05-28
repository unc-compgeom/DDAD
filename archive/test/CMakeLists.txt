include_directories(${DEGDRIVENALGODESIGN_SOURCE_DIR}/src)

set(LIBRARIES ${CORE_LIBS})
set(TESTS run_tests)

# build tests
foreach(test ${TESTS})
    add_executable(${test} ${test}.cpp)
    message(STATUS "Libraries are: ${LIBRARIES}")
    target_link_libraries(${test} ${LIBRARIES})
endforeach(test)
