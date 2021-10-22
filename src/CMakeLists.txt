
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
    system/SystemEventLoop.cpp
    system/SystemManager.cpp

    event/REvent.cpp
    robject/RObject.cpp

    interfaces/BasePlugin.cpp
    interfaces/PluginRegister.cpp

    proxy/BaseProxy.cpp
    
    proxy/camera/VideoStream.cpp
    proxy/telemetry/regist_sensors.cpp
    proxy/mission/flight/waypoint/WPMContext.cpp
    proxy/mission/flight/waypoint/WPMission.cpp
    proxy/mission/flight/waypoint/WPMExecutor.cpp
    proxy/mission/MissionDescription.cpp
    proxy/mission/BaseExecutor.cpp
    proxy/mission/MissionContext.cpp
    proxy/mission/MissionEvent.cpp
)

