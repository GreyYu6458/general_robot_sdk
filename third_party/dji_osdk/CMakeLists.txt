cmake_minimum_required(VERSION 3.1)

set(SUPER_BUILD_PROJECT_NAME ex-dji_osdk)

project(${SUPER_BUILD_PROJECT_NAME})
include(ExternalProject)

list(APPEND CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
    "-DCMAKE_POSITION_INDEPENDENT_CODE=ON"
    "-DADVANCED_SENSING=ON"
)

if(IOS)
    list(APPEND CMAKE_ARGS
        "-DPLATFORM=${PLATFORM}"
        "-DDEPLOYMENT_TARGET=${DEPLOYMENT_TARGET}"
        "-DENABLE_STRICT_TRY_COMPILE=${ENABLE_STRICT_TRY_COMPILE}"
    )
endif()

message(STATUS "Preparing external project \"${SUPER_BUILD_PROJECT_NAME}\" with args:")
foreach(CMAKE_ARG ${CMAKE_ARGS})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach()

message(STATUS "Preparing external project \"${SUPER_BUILD_PROJECT_NAME}\" with customer args:")
foreach(CMAKE_ARG ${SUPER_BUILD_CUSTOMER_COMMAND})
    message(STATUS "-- ${CMAKE_ARG}")
endforeach()

set(GIT_REMOTE_PATH git@gitee.com:insky-cache/insky-dji-osdk.git)
set(GIT_CLONE_PATH  ${SUPERBUILD_ROOT_PATH}/src/${SUPER_BUILD_PROJECT_NAME})

execute_process(
    COMMAND git ls-remote ${GIT_CLONE_PATH} -q
    RESULT_VARIABLE CHECK_FAILED
    OUTPUT_QUIET
)

if(NOT ${CHECK_FAILED})
    set(CUSTOM_CLONE_COMMAND "")
endif()

ExternalProject_Add(
        ${SUPER_BUILD_PROJECT_NAME}
    GIT_REPOSITORY 
        ${GIT_REMOTE_PATH}
    GIT_TAG 
        "insky"
        CMAKE_ARGS 
        "${CMAKE_ARGS}"
        "${SUPER_BUILD_CUSTOMER_COMMAND}"
    SOURCE_DIR
        ${GIT_CLONE_PATH}
    PREFIX
        ${SUPERBUILD_ROOT_PATH}/${CMAKE_BUILD_TYPE}
    GIT_SHALLOW
        TRUE
    GIT_PROGRESS 
        TRUE
    DOWNLOAD_COMMAND
        ${CUSTOM_CLONE_COMMAND}
)
