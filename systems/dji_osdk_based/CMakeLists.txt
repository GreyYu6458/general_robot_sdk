project(dji_vehicle)

set(CMAKE_CXX_STANDARD 17)
include(cmake/findDJIOSDK.cmake)

# FIND FFMPEG
find_path(AVCODEC_INCLUDE_DIR libavcodec/avcodec.h)
find_library(AVCODEC_LIBRARY avcodec)

find_path(AVFORMAT_INCLUDE_DIR libavformat/avformat.h)
find_library(AVFORMAT_LIBRARY avformat)

find_path(AVUTIL_INCLUDE_DIR libavutil/avutil.h)
find_library(AVUTIL_LIBRARY avutil)

find_path(AVDEVICE_INCLUDE_DIR libavdevice/avdevice.h)
find_library(AVDEVICE_LIBRARY avdevice)

find_path(AVDEVICE_INCLUDE_DIR libswscale/swscale.h)
find_library(SWSCALE_LIBRARY swscale)

# OpenCV
find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

include_directories(
    ${AVCODEC_INCLUDE_DIR}
    ${AVFORMAT_INCLUDE_DIR}
    ${AVUTIL_INCLUDE_DIR}
    ${AVDEVICE_INCLUDE_DIR}
    ${AVDEVICE_INCLUDE_DIR}
)

# FIND LIBUSB-1.0
find_path(LIBUSB_INCLUDE_PATH libusb-1.0/libusb.h)
find_library(LIBUSB_1_0 usb-1.0)

include_directories(
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${LIBUSB_INCLUDE_PATH}
    ${LIBUSB_INCLUDE_PATH}/libusb-1.0
)

# FIND TINYXML2
find_library(LIBTINYXML2 tinyxml2)

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

add_dependencies(
  dji_vehicle
  dji_osdk
)

target_link_libraries(
    dji_vehicle
    PUBLIC
    robot_sdk
    ${OSDK_CORE_LIBRARY}
    ${OSDK_ADVANCED_LIBRARY}
    ${OSDK_LINKER_LIBRARY}
    ${LIBUSB_1_0}
    # FFMPEG
    ${AVCODEC_LIBRARY}
    ${AVFORMAT_LIBRARY}
    ${AVUTIL_LIBRARY}
    ${AVDEVICE_LIBRARY}
    ${SWSCALE_LIBRARY}
    ${OpenCV_LIBS}
    ${LIBTINYXML2}
    boost_thread
    pthread
)

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
