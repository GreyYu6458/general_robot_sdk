option(BUILD_DJI_SYSTEM "BUILD_DJI_SYSTEM" OFF)

if(BUILD_DJI_SYSTEM)
    if(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        add_subdirectory(dji_osdk_based)
    else()
        message("DJI_OSDK_BASED_SYSTEM will not be built because it only support linux platform")
    endif()
endif()
