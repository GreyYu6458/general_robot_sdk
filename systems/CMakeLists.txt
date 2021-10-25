option(BUILD_DJI_SYSTEM "BUILD_DJI_SYSTEM" OFF)

if(BUILD_DJI_SYSTEM)
    add_subdirectory(dji_osdk_based)
endif()

