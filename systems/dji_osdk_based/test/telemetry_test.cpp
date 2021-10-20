#include <iostream>
#include <thread>
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/system/SystemLinkMethods.hpp"
#include "DJIVehicleSystem.hpp"

#include "rsdk/plugins/telemetry/Attitude.hpp"
#include "rsdk/plugins/telemetry/Avoid.hpp"
#include "rsdk/plugins/telemetry/Battery.hpp"
#include "rsdk/plugins/telemetry/GNSSReceiver.hpp"
#include "rsdk/plugins/telemetry/GNSSUncertainInfo.hpp"
#include "rsdk/plugins/telemetry/FlyingRbtSt.hpp"

namespace callbacks{

    using namespace rsdk::telemetry;

void onAttitudeDataUpdate(const AttitudeProxy::msg_type& msg)
{
    std::cout   << "attitude data " 
                << " q0:"   << msg.q0 
                << " q1:"   << msg.q1 
                << " q2:"   << msg.q2
                << " q3:"   << msg.q3
                << std::endl;
}

void onFlightStateDataUpdate(const FlyingRobotStatusProxy::msg_type& msg)
{
    std::cout   << "flight state data " 
                << " state:"   << static_cast<uint32_t>(msg)
                << std::endl;
}


void onAvoidanceDataUpdata(const AvoidanceProxy::msg_type& msg)
{
    std::cout   << "avoidance data "
                << " back:"  << msg.back.lenght
                << " down:"  << msg.down.lenght
                << " front:" << msg.front.lenght
                << " left"  << msg.left.lenght
                << " right:" << msg.right.lenght
                << " up:"    << msg.up.lenght
                << std::endl;
}

void onGNSSUncertainUpdate(const GNSSUncertainInfoProxy::msg_type& msg)
{
    std::cout   << "gnss uncertain data "
                << " gnss sat:"  << static_cast<uint32_t>(msg.sat_num)
                << " gnss fix type:" << static_cast<uint32_t>(msg.fix_type)
                << std::endl;
}

void onBatteryDataUpdata(const BatteryProxy::msg_type& msg)
{
    std::cout   << "battery data whole:" << msg.whole_voltage;
    size_t i = 0;
    for (const auto& cell : msg.each_cell)
    {
        std::cout << " " << i << ":" << cell;
    }
    std::cout << std::endl;
}

void onGNSSDataUpdate(const GNSSReceiverProxy::msg_type& msg)
{
    std::cout   << "coordinate data "
                << " altitude:"  << msg.altitude
                << " longitude:"  << msg.longitude
                << " latitude:" << msg.latitude
                << std::endl;
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

    rsdk::telemetry::AttitudeProxy              attitude_proxy(dji_system);
    rsdk::telemetry::AvoidanceProxy             avoidance_proxy(dji_system);
    rsdk::telemetry::BatteryProxy               battery_proxy(dji_system);
    rsdk::telemetry::GNSSReceiverProxy          gnss_proxy(dji_system);
    rsdk::telemetry::GNSSUncertainInfoProxy     gnssu_proxy(dji_system);
    rsdk::telemetry::FlyingRobotStatusProxy     fs_proxy(dji_system);
    
    attitude_proxy.subscribe(&callbacks::onAttitudeDataUpdate);
    avoidance_proxy.subscribe(&callbacks::onAvoidanceDataUpdata);
    battery_proxy.subscribe(&callbacks::onBatteryDataUpdata);
    gnss_proxy.subscribe(&callbacks::onGNSSDataUpdate);
    gnssu_proxy.subscribe(&callbacks::onGNSSUncertainUpdate);
    fs_proxy.subscribe(&callbacks::onFlightStateDataUpdate);

    std::cout << "is uav linked: " << (attitude_proxy.system()->isLink() ? "True" : "False") << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    attitude_proxy.start();
    avoidance_proxy.start();
    battery_proxy.start();
    gnss_proxy.start();
    gnssu_proxy.start();
    fs_proxy.start();

    char key = 0;
    while(key != 'q')
    {
        std::cin >> key;
    }

    return 0;
}