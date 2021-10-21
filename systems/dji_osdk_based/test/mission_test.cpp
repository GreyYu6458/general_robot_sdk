#include <iostream>
#include <thread>
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/system/SystemLinkMethods.hpp"
#include "rsdk/plugins/mission/flight/waypoint/WPMExecutor.hpp"
#include "rsdk/plugins/mission/flight/waypoint/WPMItem.hpp"

#include "rsdk/plugins/PluginsIndex.hpp"
#include "rsdk/plugins/mission/EventSubIndex.hpp"
#include "rsdk/plugins/mission/MissionEvent.hpp"

#include "DJIVehicleSystem.hpp"
#include "rsdk/plugins/telemetry/GNSSReceiver.hpp"
#include <condition_variable>
#include <mutex>

static uint32_t longitude;
static uint32_t latitude;
static double   altitude;

namespace callbacks{

    using namespace rsdk::telemetry;
    void onGNSSDataUpdate(const GNSSReceiverProxy::msg_type& msg)
    {
        longitude = static_cast<uint32_t>( msg.longitude * 1e7 );
        latitude  = static_cast<uint32_t>( msg.latitude * 1e7 );
        altitude  = msg.altitude;
        /*
        std::cout   << "coordinate data "
                    << " altitude:"  << altitude
                    << " latitude:"  << latitude
                    << " longitude:" << longitude << " " << msg.longitude
                    << std::endl;
        */
    }
}

int main()
{
    rsdk::SystemConfig config;
    rsdk::SerialMethod usb_serial_method;
    rsdk::SerialMethod acm_serial_method;

    usb_serial_method.dev_path = "/dev/ttyUSB0";
    usb_serial_method.baudrate = 921600;

    acm_serial_method.dev_path = "/dev/ttyACM0";
    acm_serial_method.baudrate = 921600;

    config.addParameter("usb", usb_serial_method);
    config.addParameter("acm", acm_serial_method);

    auto dji_system = std::make_shared<DJIVehicleSystem>();

    dji_system->subscribeSystemInfo(
        [](const rsdk::SystemInfo& msg)
        {
            std::cout   << "Received Messgae level :" 
                        << static_cast<int>(msg.level) 
                        << " content:" 
                        << msg.data << std::endl;
        }
    );
    
    dji_system->link(config);

    rsdk::telemetry::GNSSReceiverProxy gnss_proxy(dji_system);
    gnss_proxy.subscribe(&callbacks::onGNSSDataUpdate);
    std::cout << "is uav linked: " << (gnss_proxy.system()->isLink() ? "True" : "False") << std::endl;
    gnss_proxy.start();

    namespace rmfw = rsdk::mission::flight::waypoint;

    std::this_thread::sleep_for(std::chrono::seconds(3));

    rmfw::WPMExecutorProxy mission_proxy(dji_system);

    std::mutex              wait_finish_mutex;
    std::condition_variable wait_finish_cv;

    mission_proxy.start();

    std::shared_ptr<rmfw::WPMission> mission = std::make_shared<rmfw::WPMission>();

    rmfw::TakeOffItem takeoff_point(0);
    takeoff_point.set_x(latitude);
    takeoff_point.set_y(longitude);
    takeoff_point.set_z(20);
    mission->addItem(takeoff_point);

    // 1000 most equal 11.1m

    rmfw::WaypointItem wp_1(1);
    wp_1.set_hold_time(2);
    wp_1.set_x(latitude + 1000);
    wp_1.set_y(longitude);
    wp_1.set_z(20);
    mission->addItem(wp_1);

    rmfw::WaypointItem wp_2(2);
    wp_2.set_hold_time(2);
    wp_2.set_x(latitude + 1000);
    wp_2.set_y(longitude + 1000);
    wp_2.set_z(20);
    mission->addItem(wp_2);

    rmfw::WaypointItem wp_3(3);
    wp_3.set_hold_time(2);
    wp_3.set_x(latitude);
    wp_3.set_y(longitude + 1000);
    wp_3.set_z(20);
    mission->addItem(wp_3);

    rmfw::WaypointItem wp_4(4);
    wp_4.set_hold_time(2);
    wp_4.set_x(latitude);
    wp_4.set_y(longitude);
    wp_4.set_z(20);
    mission->addItem(wp_4);

    rmfw::LandItem land(5);
    land.set_x(latitude);
    land.set_y(longitude);
    land.set_z(altitude);
    mission->addItem(land);

    rmfw::ExecuteRst rst;
    mission_proxy.launch(mission, rst);
    
    std::cout << rst.detail << std::endl;

    std::unique_lock<std::mutex> lck(wait_finish_mutex);
    wait_finish_cv.wait(lck);

    return 0;
}
