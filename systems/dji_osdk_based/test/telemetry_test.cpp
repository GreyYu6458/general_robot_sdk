#include <iostream>
#include <thread>
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/system/SystemLinkMethods.hpp"
#include "DJIVehicleSystem.hpp"

#include "rsdk/plugins/sensors/Attitude.hpp"
#include "rsdk/plugins/sensors/Avoid.hpp"
#include "rsdk/plugins/sensors/Battery.hpp"
#include "rsdk/plugins/sensors/GNSSReceiver.hpp"

namespace callbacks{

    using namespace rsdk::sensor;

void onAttitudeDataUpdate(const AttitudeProxy::msg_type& msg)
{
    std::cout   << "attitude data " 
                << " q0:"   << msg.q0 
                << " q1:"   << msg.q1 
                << " q2:"   << msg.q2
                << " q3:"   << msg.q3
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
    std::cout   << "avoidance data "
                << " altitude:"  << msg.altitude
                << " longitude:"  << msg.longitude
                << " altitude:" << msg.altitude
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

    DJIVehicleSystem* dji_system = new DJIVehicleSystem();

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

    rsdk::sensor::AttitudeProxy             attitude_proxy(dji_system);
    rsdk::sensor::AvoidanceProxy            avoidance_proxy(dji_system);
    rsdk::sensor::BatteryProxy              battery_proxy(dji_system);
    rsdk::sensor::GNSSReceiverProxy         gnss_proxy(dji_system);
    
    attitude_proxy.subscribe(&callbacks::onAttitudeDataUpdate);
    avoidance_proxy.subscribe(&callbacks::onAvoidanceDataUpdata);
    battery_proxy.subscribe(&callbacks::onBatteryDataUpdata);
    gnss_proxy.subscribe(&callbacks::onGNSSDataUpdate);

    attitude_proxy.start();
    avoidance_proxy.start();
    battery_proxy.start();
    gnss_proxy.start();

    char key = 0;
    while(key != 'q')
    {
        std::cin >> key;
    }

    delete dji_system;

    return 0;
}