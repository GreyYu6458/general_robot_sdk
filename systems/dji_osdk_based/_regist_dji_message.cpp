#include "DJIVehicleSystem.hpp"
//  must include here to ensure DJI::OSDK::Log::instance() has been create
//  Singleton<T>::singleInstance
#include <dji_singleton.hpp> 
#include <dji_log.hpp>

void DJIVehicleSystem::_regist_osdk_info()
{
    DJI::OSDK::Log::instance().onMessage(
        [this](char* msg)
        {
            this->publishInfo<rsdk::SystemInfoLevel::LOWLAYER>(std::string(msg));
        }
    );
}