#include <iostream>
#include <thread>
#include "rsdk/system/RobotSystem.hpp"
#include "rsdk/system/SystemLinkMethods.hpp"
#include "MavBasedVehicleSystem.hpp"

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
    
    if(!mav_system->link(config))
    {
        return 0;
    }

    std::cout << "SYSTEM UNIQUE CODE:" << mav_system->uniqueCode() << std::endl;

    std::this_thread::sleep_for(std::chrono::seconds(1));

    char key = 0;
    while(key != 'q')
    {
        std::cin >> key;
    }

    return 0;
}