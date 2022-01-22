project(dji_vehicle)

set(CMAKE_CXX_STANDARD 17)
include(cmake/djisystem_compile_option.cmake)

# FIND LIBUSB-1.0
find_path(LIBUSB_INCLUDE_PATH   libusb-1.0/libusb.h)
find_library(LIBUSB_1_0         usb-1.0)

# FIND DJI OSDK
find_library(OSDK_CORE_LIBRARY      djiosdk-core        PATHS   ${SUPERBUILD_INSTALL_DIR}/lib)
find_library(OSDK_ADVANCED_LIBRARY  advanced-sensing    PATHS   ${SUPERBUILD_INSTALL_DIR}/lib)
find_library(OSDK_LINKER_LIBRARY    dji-linker          PATHS   ${SUPERBUILD_INSTALL_DIR}/lib)

add_library(
    dji_vehicle
    SHARED
    DJIVehicleSystem.cpp
    SystemHardSync.cpp
    DJICameraManager.cpp
    _regist_dji_message.cpp
    dji_linux_helper/DJIInitializationTool.cpp
    dji_linux_helper/osdkhal_linux.c
    dji_linux_helper/osdkosal_linux.c
  
    plugins/camera/DJIVideoStream.cpp
    plugins/camera/DJICameraControl.cpp

    plugins/gimbal/DJIGimbalControl.cpp

    plugins/mission/tasks/DJIWPMMainTask.cpp
    plugins/mission/tasks/DownloadPhotoTask.cpp
    plugins/mission/DJIEventWrapper.cpp
    plugins/mission/DJIWPMInstance.cpp
    plugins/mission/DJIWPMission.cpp
    plugins/mission/exif.cpp

    plugins/collector/DJIBattery.cpp
    plugins/collector/DJIGPSTime.cpp
    plugins/collector/DJISensors_Impl.cpp

    plugins/mission/interpreter/parse_common_wp.cpp
    plugins/mission/interpreter/parse_photo_item.cpp
    plugins/mission/interpreter/STDWPInterpreter.cpp
)

target_compile_definitions(
    dji_vehicle
    PRIVATE
    ADVANCED_SENSING
)

target_link_libraries(
    dji_vehicle
    PUBLIC
    robot_sdk
    ${OSDK_CORE_LIBRARY}
    ${OSDK_ADVANCED_LIBRARY}
    ${OSDK_LINKER_LIBRARY}
    ${LIBUSB_1_0}
    boost_thread
    pthread
)

target_include_directories(
    dji_vehicle
    PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LIBUSB_INCLUDE_PATH}/libusb-1.0
)

if(DJI_SYSTEM_SIM_ENABLE)
    target_compile_definitions(
        dji_vehicle
        PRIVATE
        DJI_SYSTEM_SIM_ENABLE=1
    )
endif()

install(
    TARGETS dji_vehicle
    DESTINATION lib
)

install(
    FILES "DJIVehicleSystem.hpp"
          "DJICameraManager.hpp"
    DESTINATION include/rsdk/systems/dji_system
)

if(BUILD_DJI_TEST)
  add_subdirectory(test)
endif()
