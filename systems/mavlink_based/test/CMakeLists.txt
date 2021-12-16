project(mav_based_test)

add_executable(
    mav_link_test
    mav_link_test.cpp
)

add_executable(
    mav_mission_test
    mav_mission_test.cpp
)

add_dependencies(
    mav_mission_test
    mav_vehicle
)

add_dependencies(
    mav_mission_test
    robot_sdk
    mav_vehicle
)

target_link_libraries(
    mav_mission_test
    robot_sdk
    mav_vehicle
)
