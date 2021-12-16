#include <iostream>
#include <thread>
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/system/SystemLinkMethods.hpp"
#include "rsdk/proxy/mission/waypoint/WaypointItems.hpp"
#include "rsdk/proxy/mission/waypoint/WPMInstanceProxy.hpp"

#include "rsdk/event/MissionEvents.hpp"

#include "MavBasedVehicleSystem.hpp"
#include "rsdk/proxy/collector/GNSSReceiver.hpp"
#include <condition_variable>
#include <mutex>

static int32_t longitude;
static int32_t latitude;
static double   altitude;

namespace callbacks{

    using namespace rsdk::collector;
    void onGNSSDataUpdate(const GNSSReceiverProxy::msg_type& msg)
    {
        longitude = static_cast<int32_t>( msg.longitude * 1e7 );
        latitude  = static_cast<int32_t>( msg.latitude * 1e7 );
        altitude  = msg.altitude;
        // 473977509
        // 85456077
    }
}

int main()
{
    rsdk::SystemConfig  config;
    rsdk::UDPMethod     udp_method;

    udp_method.ip   = "127.0.0.1";
    udp_method.port = 14540;

    config.addParameter("udp", udp_method);

    auto mav_system = std::make_shared<MavBasedVehicleSystem>();

    mav_system->subscribeSystemInfo(
    [](const rsdk::SystemInfo& msg)
    {
        std::cout   << "Received Messgae level :"
                    << static_cast<int>(msg.level)
                    << " content:"
                    << msg.data << std::endl;
    }
    );

    mav_system->link(config);

    rsdk::collector::GNSSReceiverProxy gnss_proxy(mav_system);
    gnss_proxy.subscribe(&callbacks::onGNSSDataUpdate);
    std::cout << "is uav linked: " << (gnss_proxy.system()->isLink() ? "True" : "False") << std::endl;
    gnss_proxy.start();

    namespace rmw = rsdk::mission::waypoint;

    std::this_thread::sleep_for(std::chrono::seconds(3));

    rmw::WPMInstanceProxy   mission_proxy(mav_system);
    std::mutex              wait_finish_mutex;
    std::condition_variable wait_finish_cv;

    mission_proxy.start();  // proxy start work
    mission_proxy.setEventListener(
    [&wait_finish_cv](::rsdk::event::REventParam event)
    {
        using namespace rsdk::event;
        std::cout << "listened event: " + std::string(event->event_name()) <<  std::endl;
        if( event->type() == static_cast<uint64_t>(mission::MissionEvent::MISSION_SUCCESS_FINIHED) or
            event->type() == static_cast<uint64_t>(mission::MissionEvent::MISSION_FAILED)
        )
        {
            mission::MissionInfo* info = (mission::MissionInfo*)event->payload_unsafe();
            std::cout << "is interruptted :" << (info->is_interrupted ? "yes" : "no") << std::endl;
            wait_finish_cv.notify_all();
        }
        if(event->type() == static_cast<uint64_t>(mission::MissionEvent::MISSION_START_FAILED_EVNET))
        {
            auto _failed_event = rsdk::event::REventCast<mission::MissionFailedEvent>(event);
            std::cout << _failed_event->payload().detail << std::endl;
            wait_finish_cv.notify_all();
        }
    }
    );

    std::this_thread::sleep_for(std::chrono::seconds(1));

    mission_proxy.setMediaRootPath("/home/chenhui/");
    rsdk::mission::waypoint::WaypointItems mission;

    rmw::TakeOffItem takeoff_point;
    takeoff_point.set<rmw::ItemParam::AUTO_CONTINUE>(true);
    takeoff_point.set_x(latitude);
    takeoff_point.set_y(longitude);
    takeoff_point.set_z(5);
    mission.addItem(takeoff_point);

    // 1000 most equal 11.1m


    rmw::WaypointItem wp_1;
    wp_1.set_hold_time(1);
    wp_1.set<rmw::ItemParam::AUTO_CONTINUE>(true);
    wp_1.set_accept_radius(0);
    wp_1.set_pass_radius(0);
    wp_1.set_x(latitude + 1000);
    wp_1.set_y(longitude);
    wp_1.set_z(5);
    mission.addItem(wp_1);


    rmw::WaypointItem wp_2;
    wp_2.set_hold_time(5);
    wp_2.set<rmw::ItemParam::AUTO_CONTINUE>(true);
    wp_2.set_accept_radius(0);
    wp_2.set_pass_radius(0);
    wp_2.set_x(latitude + 1000);
    wp_2.set_y(longitude + 1000);
    wp_2.set_z(5);
    mission.addItem(wp_2);

    rmw::WaypointItem wp_3;
    wp_3.set<rmw::ItemParam::AUTO_CONTINUE>(true);
    wp_3.set_hold_time(5);
    wp_3.set_accept_radius(0);
    wp_3.set_pass_radius(0);
    wp_3.set_x(latitude);
    wp_3.set_y(longitude + 1000);
    wp_3.set_z(5);
    mission.addItem(wp_3);

    rmw::WaypointItem wp_4;
    wp_4.set<rmw::ItemParam::AUTO_CONTINUE>(true);
    wp_4.set_hold_time(5);
    wp_4.set_accept_radius(0);
    wp_4.set_pass_radius(0);
    wp_4.set_x(latitude);
    wp_4.set_y(longitude);
    wp_4.set_z(5);
    mission.addItem(wp_4);

    rmw::LandItem land;
    land.set<rmw::ItemParam::AUTO_CONTINUE>(true);
    land.set_abort_alt(0);
    land.set_land_mode(0);
    land.set_yaw(NAN);
    land.set_x(latitude);
    land.set_y(longitude);
    land.set_z(0);
    mission.addItem(land);

    mission_proxy.setWaypointItems(mission);
    mission_proxy.setMediaRootPath("/home/chenhui/");
    mission_proxy.startMission();

    std::unique_lock<std::mutex> lck(wait_finish_mutex);
    wait_finish_cv.wait(lck);

    std::this_thread::sleep_for(std::chrono::seconds(5));

    return 0;
}
