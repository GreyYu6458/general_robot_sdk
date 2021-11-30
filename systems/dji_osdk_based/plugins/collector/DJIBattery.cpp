#include "DJIBattery.hpp"
#include "DJIVehicleSystem.hpp"

#include <dji_battery.hpp>
#include <dji_vehicle.hpp>
#include <cmath>
#include <thread>
#include <atomic>

class DJIBatteryWrapper::Impl
{
public:
    Impl()
    {
        _message.each_cell = {{},{}};
    }

    BatteryWholeInfo        _batteryWholeInfo;
    std::thread             _bg_thread;
    sensor_msg::BatteryInfo _message; // 
    std::atomic<bool>       _should_exit{false};
};

DJIBatteryWrapper::DJIBatteryWrapper(const std::shared_ptr<DJIVehicleSystem>& system)
:   DJIPluginBase(system),
    rsdk::collector::BatteryPlugin(system)
{
    _impl           = new Impl();
}

DJIBatteryWrapper::~DJIBatteryWrapper()
{
    _impl->_should_exit = true;
    if (_impl->_bg_thread.joinable())
        _impl->_bg_thread.join();
    delete _impl;
}

DJIVehicleModels DJIBatteryWrapper::supportModel()
{
    return DJIVehicleModels::MODEL_M300;
}

bool DJIBatteryWrapper::start() 
{ 
    this->exec(); 
    return true;
}

bool DJIBatteryWrapper::isStarted()
{
    return _impl->_should_exit;
}

// TODO move thread from lambda to this method
void DJIBatteryWrapper::collect()
{
    auto wait_millisecond = static_cast<size_t>(1000.0f / updateRate());
    
    bool enableSubscribeBatteryWholeInfo = true;

    vehicle()->djiBattery->subscribeBatteryWholeInfo(enableSubscribeBatteryWholeInfo);

    SmartBatteryDynamicInfo dji_battery_one;
    SmartBatteryDynamicInfo dji_battery_two;

    while (!_impl->_should_exit)
    {
        if(this->_dji_system->DJIAPIMutex().try_lock())
        {
            vehicle()->djiBattery->getSingleBatteryDynamicInfo(
                DJIBattery::RequestSmartBatteryIndex::FIRST_SMART_BATTERY, 
                dji_battery_one
            );
            vehicle()->djiBattery->getSingleBatteryDynamicInfo(
                DJIBattery::RequestSmartBatteryIndex::SECOND_SMART_BATTERY, 
                dji_battery_two
            );
            this->_dji_system->DJIAPIMutex().unlock();
            _impl->_message.whole_voltage = dji_battery_one.currentVoltage / 1e3;
            _impl->_message.each_cell[0] = (dji_battery_one.currentVoltage / 1e3);
            _impl->_message.each_cell[1] = (dji_battery_two.currentVoltage / 1e3);
            onUpdate(this->_impl->_message);
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(wait_millisecond));
    }
}

void DJIBatteryWrapper::exec()
{
    std::thread( std::bind(&DJIBatteryWrapper::collect, this) ).swap(_impl->_bg_thread);
}
