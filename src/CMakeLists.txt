if(WIN32)
add_definitions(-DBOOST_UUID_FORCE_AUTO_LINK)
endif()

add_library(
    robot_sdk
    SHARED
    system/PluginMap.cpp
    system/RobotSystem.cpp
    system/RobotSystemConfig.cpp
    system/SystemInfoPublisher.cpp
    system/SystemEventLoop.cpp
    tools/math/haversine.cpp

    event/REvent.cpp
    robject/RObject.cpp

    plugins/BasePlugin.cpp
    plugins/PluginRegister.cpp
    plugins/camera/VideoStreamPlugin.cpp
    plugins/camera/CameraControlPlugin.cpp
    plugins/gimbal/GimbalControlPlugin.cpp
    plugins/mission/InstancePlugin.cpp
    plugins/mission/MissionTask.cpp
    plugins/mission/waypoint/WPMInstancePlugin.cpp
    plugins/collector/collector_plugin_regist.cpp
    
    proxy/BaseProxy.cpp
    proxy/camera/VideoStream.cpp
    proxy/camera/CameraControlProxy.cpp

    proxy/gimbal/GimbalControlProxy.cpp
    
    proxy/mission/MissionInstanceProxy.cpp
    proxy/mission/waypoint/WaypointItems.cpp
    proxy/mission/waypoint/WPMInstanceProxy.cpp
    proxy/collector/collector.cpp
)

install(
    TARGETS robot_sdk
    DESTINATION lib
)