include(ProcessorCount)

set(DEPS_ROOT_PATH          ${CMAKE_BINARY_DIR})
set(SUPERBUILD_INSTALL_DIR  ${CMAKE_BINARY_DIR}/dep_install)
include_directories(${SUPERBUILD_INSTALL_DIR}/include)
set(CMAKE_PREFIX_PATH       ${SUPERBUILD_INSTALL_DIR}) # for find_package
set(SUPERBUILD_PACKAGE_DIR) # package list

function(SET_SUPERBUILD CMAKE_CONFIG_PATH TARGET_NAME)
    set(TARGET_SOURCE_DIR  "${CMAKE_CONFIG_PATH}")
    set(TARGET_BINARY_DIR  "${DEPS_ROOT_PATH}/${TARGET_NAME}")
    set(TARGET_INSTALL_DIR "${SUPERBUILD_INSTALL_DIR}")

    file(MAKE_DIRECTORY ${TARGET_BINARY_DIR})

    if(DEFINED CMAKE_GENERATOR_PLATFORM AND NOT "${CMAKE_GENERATOR_PLATFORM}" STREQUAL "")
        set(PLATFORM_ARGUMENT "-A${CMAKE_GENERATOR_PLATFORM}")
    endif()

    execute_process(
        COMMAND ${CMAKE_COMMAND}
            "-G${CMAKE_GENERATOR}"
            "${PLATFORM_ARGUMENT}"
            "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
            "-DCMAKE_TOOLCHAIN_FILE:PATH=${CMAKE_TOOLCHAIN_FILE}"
            "-DCMAKE_INSTALL_PREFIX:PATH=${TARGET_INSTALL_DIR}"
            "-DCMAKE_PREFIX_PATH:PATH=${CMAKE_PREFIX_PATH}"
            "-DPLATFORM=${PLATFORM}" # for iOS toolchain
            "-DDEPLOYMENT_TARGET=${DEPLOYMENT_TARGET}" # for iOS toolchain
            "-DENABLE_STRICT_TRY_COMPILE=${ENABLE_STRICT_TRY_COMPILE}" # for iOS toolchain
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
