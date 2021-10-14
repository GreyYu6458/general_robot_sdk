project(dji_osdk_test)

add_executable(
    dji_telemetry_test
    telemetry_test.cpp
)

add_dependencies(
    dji_telemetry_test
    dji_vehicle
)

target_link_libraries(
    dji_telemetry_test
    dji_vehicle
)