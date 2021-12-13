project(dji_vehicle)

set(CMAKE_CXX_STANDARD 17)

include(cmake/findMAVSDK.cmake)


include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

add_library(
    mav_vehicle
    SHARED
    MavBasedVehicleSystem.cpp
    plugins/collectror/MavAttitude.cpp
    plugins/collectror/MavBasedCollector.cpp
    plugins/collectror/MavBattery.cpp
    plugins/collectror/MavFlightStatus.cpp
    plugins/collectror/MavGNSSReceiver.cpp
    plugins/collectror/MavGNSSUncertain.cpp

    plugins/mission/MavMissionInstance.cpp
    plugins/mission/MavMissionEventHandler.cpp
    plugins/mission/tasks/MavMainTask.cpp
    plugins/mission/interpreter/MavWPInterpreter.cpp
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
    mavsdk_action
    mavsdk_action_server
    mavsdk_calibration
    mavsdk_camera
    mavsdk_failure
    mavsdk_follow_me
    mavsdk_ftp
    mavsdk_geofence
    mavsdk_gimbal
    mavsdk_log_files
    mavsdk_manual_control
    mavsdk_mission_raw_server
    mavsdk_mocap
    mavsdk_offboard
    mavsdk_param
    mavsdk_param_server
    mavsdk_server_utility
    mavsdk_shell
    mavsdk_tracking_server
    mavsdk_transponder
    mavsdk_tune
    mavsdk_info
    mavsdk_telemetry
    mavsdk_mission
    mavsdk_mission_raw
    mavsdk_mavlink_passthrough
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

if(BUILD_MAV_TEST)
  add_subdirectory(test)
endif()
