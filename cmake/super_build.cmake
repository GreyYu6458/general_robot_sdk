#[[
    SUPER BUILD TOOL
    CREATE BY CHENHUI.YU 1.1.2020
]]
include(ProcessorCount)

# Default CMAKE_BUILD_TYPE
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RELEASE" CACHE STRING "SET CMAKE BUILD TYPE" FORCE)
    message("SUPER BUILD DEFAILT  BUILD TYPE:${CMAKE_BUILD_TYPE}")
else()
    message("SUPER BUILD CUSTOMER BUILD TYPE:${CMAKE_BUILD_TYPE}")
endif()

# Pass down parameters
# SUPERBUILD_ROOT_PATH must pass down
if(NOT SUPERBUILD_ROOT_PATH)
    set(SUPERBUILD_ROOT_PATH ${CMAKE_BINARY_DIR}/superbuild)
    message("SUPER BUILD DEFAULT  ROOT DIRECTORY:${SUPERBUILD_ROOT_PATH}")
else()
    message("SUPER BUILD CUSTOMER ROOT DIRECTORY:${SUPERBUILD_ROOT_PATH}")
endif()

set(SUPERBUILD_BUILD_TYPE_PATH  ${SUPERBUILD_ROOT_PATH}/${CMAKE_BUILD_TYPE})
set(SUPERBUILD_WORK_PATH        ${SUPERBUILD_BUILD_TYPE_PATH}/ws)

set(
    CMAKE_INSTALL_PREFIX ${SUPERBUILD_BUILD_TYPE_PATH}/install
    CACHE  STRING 
    "SET CMAKE INSTALL PREFIX" FORCE
)
message("SUPER BUILD INSTALL DIRECTORY:${CMAKE_INSTALL_PREFIX}")

if(CMAKE_PREFIX_PATH) # CMAKE_PREFIX_PATH has been set
    if(NOT ${CMAKE_INSTALL_PREFIX} IN_LIST CMAKE_PREFIX_PATH)
        list(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_INSTALL_PREFIX}) # for find_package, fist search path
    endif()
else()
    list(INSERT CMAKE_PREFIX_PATH 0 ${CMAKE_INSTALL_PREFIX}) # for find_package, fist search path
endif()

message("SUPER BUILD CMAKE PREFIX PATH:${CMAKE_PREFIX_PATH}")

include_directories(${CMAKE_INSTALL_PREFIX}/include)


function(SET_SUPERBUILD TARGET_NAME CMAKE_CONFIG_PATH)

    set(options             "placeholder_opt")
    set(one_value_args      "placeholder_one_value")
    set(multi_value_args    "CMAKE_ARGS")
    set(BUILD_DONE_NAME     ${TARGET_NAME}_${CMAKE_BUILD_TYPE}_BUILD_DONE)
    set(TARGET_SOURCE_DIR  "${CMAKE_CONFIG_PATH}")
    set(TARGET_BINARY_DIR  "${SUPERBUILD_WORK_PATH}/${TARGET_NAME}")

    if(DEFINED ${BUILD_DONE_NAME})
        if(${BUILD_DONE_NAME})
            message("SUPER BUILD HAD ALREADY BUILT: ${TARGET_NAME}")
            return()
        else()
            message("SUPER BUILD START: ${TARGET_NAME}")
        endif()
    endif()

    file(MAKE_DIRECTORY ${TARGET_BINARY_DIR})

    list(SUBLIST ARGN 1 -1 LIST_ARGN)

    cmake_parse_arguments(
        PARSE_RESULT
        "${options}" 
        "${one_value_args}"
        "${multi_value_args}"
        ${ARGN}
    )

    execute_process(
        COMMAND ${CMAKE_COMMAND}
            # "-G${CMAKE_GENERATOR}"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
            "-DSUPER_BUILD_CUSTOMER_COMMAND=${PARSE_RESULT_CMAKE_ARGS}" # CUSTOMER CONFIG COMMAND
            # SUPER BUILD CONFIG PARAMETERS PASS DOWN
            "-DSUPERBUILD_ROOT_PATH=${SUPERBUILD_ROOT_PATH}"
            "${TARGET_SOURCE_DIR}"
        WORKING_DIRECTORY 
            "${TARGET_BINARY_DIR}"
        RESULT_VARIABLE CONFIGURE_FAILED
    )

    if(${CONFIGURE_FAILED})
        message(FATAL_ERROR "${TARGET_SOURCE_DIR} failed to configure!")
    endif()

    ProcessorCount(NUM_PROCS)

    if(MSVC)
        execute_process(
            COMMAND ${CMAKE_COMMAND} --build . --parallel ${NUM_PROCS}  --config ${CMAKE_BUILD_TYPE}
            WORKING_DIRECTORY ${TARGET_BINARY_DIR}
            RESULT_VARIABLE BUILD_FAILED
        )
    else()
        execute_process(
            COMMAND ${CMAKE_COMMAND} --build . --parallel ${NUM_PROCS}
            WORKING_DIRECTORY ${TARGET_BINARY_DIR}
            RESULT_VARIABLE BUILD_FAILED
        )
    endif()

    if(${BUILD_FAILED})
        message(FATAL_ERROR "${SUPERBUILD_WORK_PATH} failed to build!")
    else()
        message("SUPER BUILD SUCCESS: ${TARGET_NAME}, ${BUILD_DONE_NAME} will be set to true")
        set(${BUILD_DONE_NAME} TRUE CACHE BOOL "Did ${TARGET_NAME} has built" FORCE)
    endif()
endfunction()
