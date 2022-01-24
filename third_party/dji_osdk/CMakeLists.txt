cmake_minimum_required(VERSION 3.1)

set(SUPER_BUILD_PROJECT_NAME external-dji_osdk)

project(${SUPER_BUILD_PROJECT_NAME})
include(ExternalProject)

list(APPEND CMAKE_ARGS
    "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
    "-DCMAKE_POSITION_INDEPENDENT_CODE=ON"
    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
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

ExternalProject_Add(
        ${SUPER_BUILD_PROJECT_NAME}
    GIT_REPOSITORY 
        git@gitee.com:insky-cache/insky-dji-osdk.git
    GIT_TAG 
        "insky_log_callback"
    PREFIX 
        superbuild
    CMAKE_ARGS 
        "${CMAKE_ARGS}"
        "${SUPER_BUILD_CUSTOMER_COMMAND}"
    GIT_PROGRESS 
        TRUE
)
