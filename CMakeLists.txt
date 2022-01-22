cmake_minimum_required(VERSION 3.14)
project(robot_sdk)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "RELEASE" CACHE  STRING "SET CMAKE BUILD TYPE" FORCE)
endif()

include(cmake/super_build.cmake)
include_directories(include)

add_subdirectory(src)
add_subdirectory(systems)

install(
    DIRECTORY include/rsdk
    DESTINATION include
)
