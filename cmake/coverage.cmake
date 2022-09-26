# Author github.com/y9luiz

set(COVERAGE_ENABLED OFF)
set_property(GLOBAL PROPERTY COVERAGE_ENABLED)

if(NOT CMAKE_BUILD_TYPE STREQUAL "")
    string(TOLOWER ${CMAKE_BUILD_TYPE} BUILD_TYPE)
else()
    set(BUILD_TYPE "Debug")
endif(NOT CMAKE_BUILD_TYPE STREQUAL "")

if ((${BUILD_TYPE} STREQUAL "coverage") AND (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"))
    set(CMAKE_CXX_FLAGS "-fprofile-instr-generate -fcoverage-mapping")
    
    set(COVERAGE_ENABLED ON)

    message(STATUS ">>>> COVERAGE BUILD CONFIGURED")
elseif( (${BUILD_TYPE} STREQUAL "coverage") AND NOT (${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang"))
    message(STATUS ">>>> COVERAGE BUILD ARE ENABLE JUST FOR Clang COMPILER")
else()
    message(STATUS ">>>> COVERAGE BUILD DISABLED")
endif()

function (addToCoverageReport TARGET_NAME)
    if (COVERAGE_ENABLED)
        generateProfrawFile(${TARGET_NAME})

        set(PROF_RAW_FILENAME "default.profraw")

        if (DEFINED ENV{LLVM_PROFILE_FILE})
            set(PROF_RAW_FILENAME $ENV{LLVM_PROFILE_FILE})
        endif()

        set(PROF_DATA_FILENAME ${TARGET_NAME}.profdata)

        generateProfDataFile(${TARGET_NAME} ${PROF_RAW_FILENAME} ${PROF_DATA_FILENAME})

	generateHtmlPageFromProfileFile(${TARGET_NAME} ${PROF_DATA_FILENAME})

	generateReportFile(${TARGET_NAME} ${PROF_DATA_FILENAME})
    endif()
endfunction(addToCoverageReport)

function(generateProfrawFile TARGET_NAME)
    if( NOT ${COVERAGE_ENABLED})
        message(FATAL_ERROR ">>>> COVERAGE NOT ENABLED")
    endif()

    set(EXECUTABLE_NAME ${TARGET_NAME})
    
    if (WIN32)    
        set(EXECUTABLE_NAME ${TARGET_NAME}.exe)
    endif()


    add_custom_command(
        TARGET  ${TARGET_NAME}
        POST_BUILD
        COMMAND  ./${EXECUTABLE_NAME})
endfunction(generateProfrawFile)

function(generateProfDataFile TARGET_NAME PROF_RAW_FILENAME PROF_DATA_FILENAME)
    add_custom_command(
            TARGET ${TARGET_NAME}
            COMMAND  llvm-profdata merge -sparse ${PROF_RAW_FILENAME} -o ${PROF_DATA_FILENAME}  
            POST_BUILD)
endfunction(generateProfDataFile)

function(generateHtmlPageFromProfileFile TARGET_NAME PROF_DATA_FILENAME)
    set(EXECUTABLE_NAME ${TARGET_NAME})
    
    if (WIN32)    
        set(EXECUTABLE_NAME ${TARGET_NAME}.exe)
    endif()

    set(HTML_PAGE_FILENAME CodeCoverage_${TARGET_NAME}.html)

    add_custom_command(
            TARGET ${TARGET_NAME}
            COMMAND  llvm-cov-12 show ${EXECUTABLE_NAME} -instr-profile=${PROF_DATA_FILENAME} -format=html -ignore-filename-regex=boost > ${HTML_PAGE_FILENAME}
            POST_BUILD)
endfunction(generateHtmlPageFromProfileFile)

function(generateReportFile TARGET_NAME PROF_DATA_FILENAME)
    set(REPORT_FILENAME CodeCoverage_${TARGET_NAME})

    add_custom_command(
            TARGET ${TARGET_NAME}
            COMMAND  llvm-cov-12 report -instr-profile=${PROF_DATA_FILENAME} -ignore-filename-regex=boost ${TARGET_NAME} | tee ${CMAKE_CURRENT_BINARY_DIR}/${REPORT_FILENAME}.txt
            POST_BUILD)
endfunction(generateReportFile)
