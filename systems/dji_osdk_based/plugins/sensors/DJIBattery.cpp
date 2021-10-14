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
    uint16_t    _freq{1};
    std::thread _bg_thread;
    sensor_msg::BatteryInfo _message;
    std::atomic<bool> _c{true};
};


DJIBatteryWrapper::DJIBatteryWrapper(DJIVehicleSystem* system)
: DJIPluginBase(system){
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

rsdk::PIFInvokeRst DJIBatteryWrapper::start() 
{ 
    this->exec(); 
    return rsdk::PIFInvokeRst::SUCCESS;
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

            _impl->_message.each_cell.reserve(2);

            while (_impl->_c)
            {
                BatteryWholeInfo batteryWholeInfo;
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
