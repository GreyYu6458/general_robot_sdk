project(dji_osdk_test)

add_executable(
    dji_telemetry_test
    telemetry_test.cpp
)

add_executable(
    dji_mission_test
    mission_test.cpp
)

add_dependencies(
    dji_telemetry_test
    dji_vehicle
)

add_dependencies(
    dji_mission_test
    robot_sdk
    dji_vehicle
)

target_link_libraries(
    dji_telemetry_test
    robot_sdk
    dji_vehicle
)

target_link_libraries(
    dji_mission_test
    robot_sdk
    dji_vehicle
)