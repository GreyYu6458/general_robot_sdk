#include "DJIGPSTime.hpp"

DJIGPSTime::DJIGPSTime(const std::shared_ptr<DJIVehicleSystem>& system)
: dji::gps_time::BaseType(system)
{
}

#include <iostream>
void DJIGPSTime::convert(const DJIGPSTime::pkg_msg_type& date)
{
    _dji_system->updateSystemTime(std::get<0>(date));
    std::cout << std::get<0>(date) << std::endl;
}
