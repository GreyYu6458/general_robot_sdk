include(ExternalProject)

SET(MAVSDK_INSTALL_DIR ${CMAKE_BINARY_DIR}/mavsdk)
SET(MAVSDK_LIBRARY_DIR ${MAVSDK_INSTALL_DIR}/lib)
SET(MAVSDK_INCLUDE_DIR ${MAVSDK_INSTALL_DIR}/include)

SET(
    MAVSDK_LINK_LIBRARIES
    ${MAVSDK_LIBRARY_DIR}/libmavsdk.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_action.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_action_server.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_calibration.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_camera.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_failure.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_follow_me.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_ftp.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_geofence.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_gimbal.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_log_files.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_manual_control.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_mission_raw_server.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_mocap.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_offboard.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_param.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_param_server.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_server_utility.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_shell.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_tracking_server.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_transponder.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_tune.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_info.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_telemetry.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_mission.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_mission_raw.a
    ${MAVSDK_LIBRARY_DIR}/libmavsdk_mavlink_passthrough.a
)

ExternalProject_Add(
    mavsdk_project
    GIT_REPOSITORY
        https://github.com/mavlink/MAVSDK.git
    GIT_TAG
        "v0.50.1"
    GIT_PROGRESS
        1
    UPDATE_COMMAND
        ""
    BUILD_BYPRODUCTS
        ${MAVSDK_LINK_LIBRARIES}
    CMAKE_ARGS
        "-DCMAKE_CXX_COMPILER=${CMAKE_CXX_COMPILER}"
        "-DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}"
        "-DCMAKE_INSTALL_PREFIX:PATH=${MAVSDK_INSTALL_DIR}"
        "-DBUILD_SHARED_LIBS=OFF"
)



link_directories(${MAVSDK_LIBRARY_DIR})
include_directories(${MAVSDK_INCLUDE_DIR})
include_directories(${MAVSDK_INCLUDE_DIR}/mavsdk)
