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

    message(INFO ">>>> Compile the code to generate the code coverage report")

    add_custom_command(OUTPUT ctestLogs.txt
                COMMAND ${CTEST}
                POST_BUILD)
    
    add_custom_command(OUTPUT lcovLogs.txt
                COMMAND  ${LCOV} -c -d . -o coverage.info && lcov --remove coverage.info '/usr/*' '*google*' '*mock*' '*test*' --output-file coverage.info
                POST_BUILD)

    add_custom_command(OUTPUT genHtmlLogs.txt
                COMMAND  genhtml coverage.info --output-directory=./report
                POST_BUILD)

    add_custom_target(dummyTarget ALL
                DEPENDS ctestLogs.txt lcovLogs.txt genHtmlLogs.txt
                COMMENT "Your code coverage report it's available at ${CMAKE_CURRENT_SOURCE_DIR}/report folder")
endif()