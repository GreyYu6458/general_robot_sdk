
find_package(Boost 1.75 REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message("can not find boost library")
endif()

add_library(
    robot_sdk
    SHARED
    system/RobotSystem.cpp
    system/RobotSystemConfig.cpp
    system/SystemInfoPublisher.cpp
    plugins/PluginGlobalRegister.cpp
    plugins/camera/VideoStream.cpp
    plugins/sensors/regist_sensors.cpp
)

