
if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    option(BUILD_DJI_SYSTEM "BUILD_DJI_SYSTEM" OFF)    
    if(BUILD_DJI_SYSTEM)

        SET_SUPERBUILD(
            dji_osdk
            ${CMAKE_SOURCE_DIR}/third_party/dji_osdk
        )

        add_subdirectory(dji_osdk_based)

    endif()
else()
    message("DJI_OSDK_BASED_SYSTEM will not be built because it only support linux platform")
endif()

option(BUILD_MAVBASED_SYSTEM "BUILD_MAVBASED_SYSTEM" OFF)
if(BUILD_MAVBASED_SYSTEM)

    SET_SUPERBUILD(
        mavsdk
        ${CMAKE_SOURCE_DIR}/third_party/mavsdk
    CMAKE_ARGS
        "-DSUPERBUILD=OFF"
        "-DBUILD_TESTS=OFF"
        "-DCMAKE_TOOLCHAIN_FILE=${CMAKE_TOOLCHAIN_FILE}"
    )

    add_subdirectory(mavlink_based)
endif()
 