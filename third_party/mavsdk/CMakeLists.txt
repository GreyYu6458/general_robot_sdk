cmake_minimum_required(VERSION 3.1)

set(SUPER_BUILD_PROJECT_NAME ex-mavsdk)

project(${SUPER_BUILD_PROJECT_NAME})
include(ExternalProject)

list(APPEND CMAKE_ARGS
    "-DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}"
    "-DCMAKE_INSTALL_PREFIX:PATH=${CMAKE_INSTALL_PREFIX}"
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

if(EXISTS ${GIT_CLONE_DIRECTORY})
    execute_process(
        COMMAND git ls-remote ${GIT_CLONE_DIRECTORY} -q
        RESULT_VARIABLE CHECK_FAILED
    )

    if(NOT CHECK_FAILED)
        set(SHOULD_CLONE_REPO "DOWNLOAD_COMMAND \"\"")
    endif()
endif()

ExternalProject_Add(
        ${SUPER_BUILD_PROJECT_NAME}
    GIT_REPOSITORY 
        https://github.com/mavlink/MAVSDK.git
    GIT_TAG 
        main
    CMAKE_ARGS 
        "${CMAKE_ARGS}"
        "${SUPER_BUILD_CUSTOMER_COMMAND}"
    PREFIX
        ${SUPERBUILD_ROOT_PATH}/${CMAKE_BUILD_TYPE}
    SOURCE_DIR
        ${SUPERBUILD_ROOT_PATH}/src/${SUPER_BUILD_PROJECT_NAME}
    GIT_PROGRESS 
        TRUE
)