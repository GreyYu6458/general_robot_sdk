#include "DJIGNSS.hpp"
#include "DJIGNSSUncertain.hpp"
#include "DJIFlightStatus.hpp"
#include "DJIAttitude.hpp"
#include "DJIAvoid.hpp"
#include "DJIFlightStatus.hpp"
#include <cmath>

// rsdk::PIFInvokeRst DJIFlightStatus::start() {  }

DJIAttitude::DJIAttitude(DJIVehicleSystem *system)
    : dji::attitude::BaseType(system) {}

DJIAvoid::DJIAvoid(DJIVehicleSystem *system)
    : dji::avoid::BaseType(system) {}

DJIGNSSReceiver::DJIGNSSReceiver(DJIVehicleSystem *system)
    : dji::gnss::BaseType(system) {}

DJIGNSSUncertain::DJIGNSSUncertain(DJIVehicleSystem *system)
    : dji::gnss_uncertain::BaseType(system) {}

DJIFlightStatus::DJIFlightStatus(DJIVehicleSystem* system)
    : dji::flight_status::BaseType(system) {}


bool DJIFlightStatus::start()
{ this->exec(); return true; }

bool DJIAttitude::start()      
{ this->exec(); return true; }

bool DJIAvoid::start()         
{ this->exec(); return true; }

bool DJIGNSSReceiver::start()  
{ this->exec(); return true; }

bool DJIGNSSUncertain::start() 
{ this->exec(); return true; }


bool DJIFlightStatus::isStarted()
{ return this->_is_started; }

bool DJIAttitude::isStarted()  
{ return this->_is_started; }

bool DJIAvoid::isStarted()      
{ return this->_is_started; }

bool DJIGNSSReceiver::isStarted()
{ return this->_is_started; }

bool DJIGNSSUncertain::isStarted()
{ return this->_is_started; }


// 
void DJIAttitude::convert(const DJIAttitude::pkg_msg_type& data)
{
    auto &raw_q = std::get<0>(data);

    _sensor_msg.q0 = raw_q.q0;
    _sensor_msg.q1 = raw_q.q1;
    _sensor_msg.q2 = raw_q.q2;
    _sensor_msg.q3 = raw_q.q3;

    onUpdate(_sensor_msg);
}

void DJIAvoid::convert(const DJIAvoid::pkg_msg_type& data)
{
    auto &raw_data = std::get<0>(data);

    _sensor_msg.up.lenght       = raw_data.up;
    _sensor_msg.down.lenght     = raw_data.down;
    _sensor_msg.left.lenght     = raw_data.left;
    _sensor_msg.right.lenght    = raw_data.right;
    _sensor_msg.front.lenght    = raw_data.front;
    _sensor_msg.back.lenght     = raw_data.back;

    _sensor_msg.up.is_used      = raw_data.upHealth;
    _sensor_msg.down.is_used    = raw_data.downHealth;
    _sensor_msg.left.is_used    = raw_data.leftHealth;
    _sensor_msg.right.is_used   = raw_data.rightHealth;
    _sensor_msg.front.is_used   = raw_data.frontHealth;
    _sensor_msg.back.is_used    = raw_data.backHealth;
    onUpdate(_sensor_msg);
}

/*
    Current Define In OSDK
    enum  {
      STOPED = 0,
      ON_GROUND = 1,
      IN_AIR = 2
    }
*/
void DJIFlightStatus::convert(const DJIFlightStatus::pkg_msg_type& data)
{
    switch(std::get<0>(data))
    {
        case 0:
            _sensor_msg = sensor_msg::FlightEnum::STOPED;
            break;
        case 1:
            _sensor_msg = sensor_msg::FlightEnum::ON_GROUND;
            break;
        case 2:
            _sensor_msg = sensor_msg::FlightEnum::IN_AIR;
            break;
        default:
            _sensor_msg = sensor_msg::FlightEnum::UNKNOWN;
    }
    onUpdate(_sensor_msg);
}

void DJIGNSSReceiver::convert(const DJIGNSSReceiver::pkg_msg_type& data)
{
    auto &raw_gps = std::get<0>(data);
    auto &raw_fused_altitude = std::get<1>(data);
    auto &raw_rtk_status = std::get<2>(data);
    auto &raw_rtk = std::get<3>(data);

    if (!raw_rtk_status.rtkConnected)
    {
        _sensor_msg.latitude    = raw_rtk.latitude * 180 / M_PI;
        _sensor_msg.longitude   = raw_rtk.longitude * 180 / M_PI;
    }
    else
    {
        _sensor_msg.latitude    = raw_gps.latitude  * 180 / M_PI;
        _sensor_msg.longitude   = raw_gps.longitude * 180 / M_PI;
    }

    _sensor_msg.altitude = raw_fused_altitude;
    onUpdate(_sensor_msg);
}

void DJIGNSSUncertain::convert(const DJIGNSSUncertain::pkg_msg_type& data)
{
    auto &raw_gps = std::get<0>(data);

    _sensor_msg.fix_type = raw_gps.fix;
    _sensor_msg.hdop = raw_gps.hdop;
    _sensor_msg.vdop = raw_gps.gnssStatus; // what fuck?
    _sensor_msg.sat_num = raw_gps.NSV;
    onUpdate(_sensor_msg);
}
