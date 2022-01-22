project(dji_vehicle)

set(CMAKE_CXX_STANDARD 17)

include_directories(
  ${CMAKE_CURRENT_SOURCE_DIR}
)

find_package(MAVSDK REQUIRED)

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

target_link_libraries(
    mav_vehicle
    PUBLIC
    robot_sdk
    MAVSDK::mavsdk
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
