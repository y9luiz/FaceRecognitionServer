# Author github.com/y9luiz

option(ENABLE_CODE_COVERAGE "ENABLE CODE COVERAGE" OFF)

if(ENABLE_CODE_COVERAGE)
    if(NOT CMAKE_BUILD_TYPE STREQUAL "Debug")
        message(FATAL_ERROR ">>>> YOU MUST SET THE BUILD TYPE to Debug, in order to use code coverage!")
    endif()

    set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -fprofile-arcs -ftest-coverage")
    set(CMAKE_EXE_LINKER_FLAGS  "${CMAKE_EXE_LINKER_FLAGS} -lgcov")

    find_program(CTEST ctest REQUIRED)
    find_program(LCOV lcov REQUIRED)
    find_program(HTML_GENERATOR genhtml REQUIRED)

    message(INFO ">>>> Now you can use lcov to generate your code coverage report")
endif()
