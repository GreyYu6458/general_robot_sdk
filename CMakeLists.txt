cmake_minimum_required(VERSION 3.14)
project(robot_sdk)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_CXX_STANDARD 17)

include(cmake/super_build.cmake)
include_directories(include)

find_package(Boost 1.75 REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message("can not find boost library")
endif()

find_package(Threads REQUIRED)

add_subdirectory(src)
add_subdirectory(systems)

install(
    DIRECTORY include/rsdk
    DESTINATION include
)
