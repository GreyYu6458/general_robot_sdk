project(mav_based_test)

add_executable(
    mav_link_test
    mav_link_test.cpp
)

add_dependencies(
    mav_link_test
    mav_vehicle
)

add_dependencies(
    mav_link_test
    robot_sdk
    mav_vehicle
)

target_link_libraries(
    mav_link_test
    robot_sdk
    mav_vehicle
)
