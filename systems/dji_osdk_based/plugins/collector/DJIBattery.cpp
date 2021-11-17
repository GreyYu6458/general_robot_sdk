#include "DJIBattery.hpp"
#include <dji_battery.hpp>
#include <dji_vehicle.hpp>
#include <cmath>
#include "DJIVehicleSystem.hpp"

#include <thread>
#include <atomic>

class DJIBatteryWrapper::Impl
{
public:
    Impl()
    {
        _message.each_cell = {{},{}};
    }

    BatteryWholeInfo batteryWholeInfo;
    uint16_t    _freq{1};
    std::thread _bg_thread;
    sensor_msg::BatteryInfo _message; // 
    std::atomic<bool> _c{false};
};


DJIBatteryWrapper::DJIBatteryWrapper(const std::shared_ptr<DJIVehicleSystem>& system)
:   DJIPluginBase(system),
    rsdk::collector::BatteryPlugin(system)
{
    _impl = new Impl();
}

DJIBatteryWrapper::~DJIBatteryWrapper()
{
    _impl->_c = false;
    if (_impl->_bg_thread.joinable())
        _impl->_bg_thread.join();
    delete _impl;
}

void DJIBatteryWrapper::setFreqency(uint16_t freq)
{
    _impl->_freq = freq;
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
    return _impl->_c;
}

// TODO move thread from lambda to this method
void DJIBatteryWrapper::collect()
{
    
}

void DJIBatteryWrapper::exec()
{
    std::thread t(
        [this, freq = _impl->_freq]()
        {
            auto wait_millisecond = static_cast<size_t>(1000.0f / freq); // from frequence to millisecond

            bool enableSubscribeBatteryWholeInfo = true;

            vehicle()->djiBattery->subscribeBatteryWholeInfo(enableSubscribeBatteryWholeInfo);

            SmartBatteryDynamicInfo dji_battery_one;
            SmartBatteryDynamicInfo dji_battery_two;

            _impl->_c = true;

            while (_impl->_c)
            {
                {
                    vehicle()->djiBattery->getSingleBatteryDynamicInfo(
                        DJIBattery::RequestSmartBatteryIndex::FIRST_SMART_BATTERY, 
                        dji_battery_one
                    );

                    vehicle()->djiBattery->getSingleBatteryDynamicInfo(
                        DJIBattery::RequestSmartBatteryIndex::SECOND_SMART_BATTERY, 
                        dji_battery_two
                    );
                }
                
                this->_impl->_message.whole_voltage = dji_battery_one.currentVoltage / 1e3;
                this->_impl->_message.each_cell[0] = (dji_battery_one.currentVoltage / 1e3);
                this->_impl->_message.each_cell[1] = (dji_battery_two.currentVoltage / 1e3);

                this->onUpdate(this->_impl->_message);

                std::this_thread::sleep_for(std::chrono::milliseconds(wait_millisecond));
            }
        }
    );

    t.swap(_impl->_bg_thread);
}
