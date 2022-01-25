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
if(NOT SUPERBUILD_ROOT_PATH)
    set(SUPERBUILD_ROOT_PATH    ${CMAKE_BINARY_DIR}/superbuild/${CMAKE_BUILD_TYPE})
    message("SUPER BUILD DEFAULT  ROOT DIRECTORY:${SUPERBUILD_ROOT_PATH}")
else()
    message("SUPER BUILD CUSTOMER ROOT DIRECTORY:${SUPERBUILD_ROOT_PATH}")
endif()

# Default install prefix set to binary directory
if(CMAKE_INSTALL_PREFIX_INITIALIZED_TO_DEFAULT)
    set(
        CMAKE_INSTALL_PREFIX 
        ${SUPERBUILD_ROOT_PATH}/install
        CACHE  STRING 
        "SET CMAKE INSTALL PREFIX" FORCE
    )
    message("SUPER BUILD DEFAULT  INSTALL DIRECTORY:${CMAKE_INSTALL_PREFIX}")
else()
    message("SUPER BUILD CUSTOMER INSTALL DIRECTORY:${CMAKE_INSTALL_PREFIX}")
endif()

if(CMAKE_PREFIX_PATH) # CMAKE_PREFIX_PATH has been set
    if(NOT ${CMAKE_INSTALL_PREFIX} IN_LIST CMAKE_PREFIX_PATH)
        list(APPEND CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX}) # for find_package
    endif()
else()
    list(APPEND CMAKE_PREFIX_PATH ${CMAKE_INSTALL_PREFIX}) # for find_package
endif()

message("SUPER BUILD CMAKE PREFIX PATH:${CMAKE_PREFIX_PATH}")

include_directories(${CMAKE_INSTALL_PREFIX}/include)

function(SET_SUPERBUILD TARGET_NAME CMAKE_CONFIG_PATH)

    set(BUILD_DONE_NAME ${TARGET_NAME}_BUILD_DONE)

    if(DEFINED ${BUILD_DONE_NAME})
        if(${BUILD_DONE_NAME})
            message("SUPER BUILD HAD ALREADY BUILT: ${TARGET_NAME}")
            return()
        endif()
    endif()

    message("SUPER BUILD START: ${TARGET_NAME}")

    set(TARGET_SOURCE_DIR  "${CMAKE_CONFIG_PATH}")
    set(TARGET_BINARY_DIR  "${SUPERBUILD_ROOT_PATH}/${TARGET_NAME}")

    file(MAKE_DIRECTORY ${TARGET_BINARY_DIR})

    if(NOT ${ARGC} LESS 3)
        if(${ARGV2} STREQUAL "CMAKE_ARGS")
            set(CUSTOM_CONFIG_COMMAND)
            foreach(ARG_NUM RANGE 3 ${ARGC})
                list(APPEND CUSTOM_CONFIG_COMMAND ${ARGV${ARG_NUM}})
            endforeach()
        endif()
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND}
            # "-G${CMAKE_GENERATOR}"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
            "-DSUPER_BUILD_CUSTOMER_COMMAND=${CUSTOM_CONFIG_COMMAND}" # CUSTOMER CONFIG COMMAND
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
        message(FATAL_ERROR "${TARGET_BINARY_DIR} failed to build!")
    else()
        message("SUPER BUILD SUCCESS: ${TARGET_NAME}, ${BUILD_DONE_NAME} will be set to true")
        set(${BUILD_DONE_NAME} TRUE CACHE BOOL "Did ${TARGET_NAME} has built" FORCE)
    endif()
endfunction()
