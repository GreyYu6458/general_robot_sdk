
find_package(Boost 1.75 REQUIRED)
if(Boost_FOUND)
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message("can not find boost library")
endif()

option(_USE_SIM_ENV "BUILD FOR SIM ENVIRONMENT" OFF)

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

    plugins/BasePlugin.cpp
    plugins/PluginRegister.cpp
    plugins/camera/VideoStreamPlugin.cpp
    plugins/mission/InstancePlugin.cpp
    plugins/mission/MissionTask.cpp
    plugins/mission/waypoint/WPMInstancePlugin.cpp
    plugins/collector/collector_plugin_regist.cpp
    
    proxy/BaseProxy.cpp
    proxy/camera/VideoStream.cpp
    proxy/mission/MissionInstanceProxy.cpp
    proxy/mission/waypoint/WaypointItems.cpp
    proxy/mission/waypoint/WPMInstanceProxy.cpp
    proxy/collector/collector.cpp
)

install(
    TARGETS robot_sdk
    DESTINATION lib
)