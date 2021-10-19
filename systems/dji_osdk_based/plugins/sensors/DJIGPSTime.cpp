#include "DJIGPSTime.hpp"

DJIGPSTime::DJIGPSTime(DJIVehicleSystem* system)
: dji::gps_time::BaseType(system)
{}

void DJIGPSTime::convert(const DJIGPSTime::pkg_msg_type& date)
{
    _dji_system->updateSystemTime(std::get<0>(date));
}
