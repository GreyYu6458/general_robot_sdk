project(dji_vehicle)

set(CMAKE_CXX_STANDARD 17)

include(cmake/findMAVSDK.cmake)

add_library(
    mav_vehicle
    SHARED
    MavBasedVehicleSystem.cpp
)

add_dependencies(
  mav_vehicle
  mavsdk_project
)

target_link_libraries(
    mav_vehicle
    PUBLIC
    robot_sdk
    mavsdk
    boost_thread
    pthread
)

install(
    TARGETS mav_vehicle
    DESTINATION lib
)

install(
    FILES "MavBasedVehicleSystem.hpp"
    DESTINATION include/rsdk/systems/mav_system
)