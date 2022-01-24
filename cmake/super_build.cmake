include(ProcessorCount)

# Pass down parameters
if(NOT SUPERBUILD_ROOT_PATH)
    set(SUPERBUILD_ROOT_PATH    ${CMAKE_BINARY_DIR}/dep/${CMAKE_BUILD_TYPE})
else()
    message("Find Super build root directory from parent project:${SUPERBUILD_ROOT_PATH}")
endif()

# Pass down parameters
if(NOT SUPERBUILD_INSTALL_DIR)
    set(SUPERBUILD_INSTALL_DIR  ${CMAKE_INSTALL_PREFIX}/${CMAKE_BUILD_TYPE})
else()
    message("Find Super build install directory from parent project:${SUPERBUILD_INSTALL_DIR}")
endif()

list(APPEND CMAKE_PREFIX_PATH ${SUPERBUILD_INSTALL_DIR}) # for find_package
message("SUPER BUILD CMAKE PREFIX PATH:${CMAKE_PREFIX_PATH}")

include_directories(${SUPERBUILD_INSTALL_DIR}/include)

function(SET_SUPERBUILD TARGET_NAME CMAKE_CONFIG_PATH)

    set(TARGET_SOURCE_DIR  "${CMAKE_CONFIG_PATH}")
    set(TARGET_BINARY_DIR  "${SUPERBUILD_ROOT_PATH}/${TARGET_NAME}")
    set(TARGET_INSTALL_DIR "${SUPERBUILD_INSTALL_DIR}")

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
            "-DCMAKE_INSTALL_PREFIX:PATH=${TARGET_INSTALL_DIR}"
            "-DSUPER_BUILD_CUSTOMER_COMMAND=${CUSTOM_CONFIG_COMMAND}" # CUSTOMER CONFIG COMMAND
            # SUPER BUILD CONFIG PARAMETERS PASS DOWN
            "-DSUPERBUILD_INSTALL_DIR=${SUPERBUILD_INSTALL_DIR}"
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
    set(ENV{MAKEFLAGS} -j${NUM_PROCS})

    if(MSVC)
        execute_process(COMMAND ${CMAKE_COMMAND} --build . --config ${CMAKE_BUILD_TYPE}
            WORKING_DIRECTORY ${TARGET_BINARY_DIR}
            RESULT_VARIABLE BUILD_FAILED
        )
    else()
        execute_process(COMMAND ${CMAKE_COMMAND} --build .
            WORKING_DIRECTORY ${TARGET_BINARY_DIR}
            RESULT_VARIABLE BUILD_FAILED
        )
    endif()

    if(${BUILD_FAILED})
        message(FATAL_ERROR "${TARGET_BINARY_DIR} failed to build!")
    endif()
endfunction()
