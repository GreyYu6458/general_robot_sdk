
find_package(Boost 1.75 REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message("can not find boost library")
endif()

add_library(
    robot_sdk
    SHARED
    system/PluginMap.cpp
    system/RobotSystem.cpp
    system/RobotSystemConfig.cpp
    system/SystemInfoPublisher.cpp

    event/REvent.cpp
    
    plugins/PluginInterface.cpp
    plugins/PluginProxy.cpp
    plugins/PluginInterfaceRegister.cpp

    plugins/camera/VideoStream.cpp

    plugins/telemetry/regist_sensors.cpp
    
    plugins/mission/flight/waypoint/WPMContext.cpp
    plugins/mission/flight/waypoint/WPMission.cpp
    plugins/mission/flight/waypoint/WPMExecutor.cpp
    plugins/mission/MissionDescription.cpp
    plugins/mission/BaseExecutor.cpp
    plugins/mission/MissionContext.cpp
    plugins/mission/MissionEvent.cpp
)

