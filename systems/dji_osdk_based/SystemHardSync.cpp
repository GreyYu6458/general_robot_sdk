#include "SystemHardSync.hpp"
#include "DJIVehicleSystem.hpp"

#include <iostream>
#include <dji_vehicle.hpp>

#include <cstdlib>
#include <cmath>
#include <cstring>
#include <chrono>

#define HEXDIGIT_CHAR(d) ((char)((d) + (((d) < 0xA) ? '0' : 'A'-0xA)))

static int64_t getTimeFromGXRMC(const uint8_t *data, size_t len)
{
    if( memcmp(data + 3, "RMC,", 4) != 0 )
    {
        return INT64_MAX;
    }

	uint8_t checksum = 0;

	// checksum
	int checksum_len = len - 2;
	const uint8_t* buffer = data + 1;
	const uint8_t* bufend = data + len - 3;
	for (; buffer < bufend; buffer++) { checksum ^= *buffer;}
	if ((HEXDIGIT_CHAR(checksum >> 4) != *(data + len - 2)) ||
    (HEXDIGIT_CHAR(checksum & 0x0F) != *(data + len - 1))) 
    {
		std::cout << std::string((char*)data, len) << std::endl;
		std::cout << "check sum error" << std::endl;
		return INT64_MAX;
    }

    char *endp;
    char *bufptr = (char *)(data + 6);
    
	double 	utc_time 		= 0.0;
	char 	Status 			= 'V';
	double 	lat 			= 0.0,
			lon 			= 0.0;
	float 	ground_speed_K 	= 0.f;
	float 	track_true 		= 0.f;
	int 	nmea_date 		= 0;
	float 	Mag_var 		= 0.f;
	char 	ns 				= '?',
			ew 				= '?';

	if (bufptr && *(++bufptr) != ',') { utc_time = strtod(bufptr, &endp); bufptr = endp; }
	if (bufptr && *(++bufptr) != ',') { Status = *(bufptr++); }
	if (bufptr && *(++bufptr) != ',') { lat = strtod(bufptr, &endp); bufptr = endp; }
	if (bufptr && *(++bufptr) != ',') { ns = *(bufptr++); }
	if (bufptr && *(++bufptr) != ',') { lon = strtod(bufptr, &endp); bufptr = endp; }
	if (bufptr && *(++bufptr) != ',') { ew = *(bufptr++); }
	if (bufptr && *(++bufptr) != ',') { ground_speed_K = strtof(bufptr, &endp); bufptr = endp; }
	if (bufptr && *(++bufptr) != ',') { track_true = strtof(bufptr, &endp); bufptr = endp; }
	if (bufptr && *(++bufptr) != ',') { nmea_date = static_cast<int>(strtol(bufptr, &endp, 10)); bufptr = endp; }
	if (bufptr && *(++bufptr) != ',') { Mag_var = strtof(bufptr, &endp); bufptr = endp; }

	if (ns == 'S') {
		lat = -lat;
	}
	if (ew == 'W') {
		lon = -lon;
	}
	float track_rad = track_true * M_PI / 180.0f; // rad in range [0, 2pi]

	if (track_rad > M_PI) {
		track_rad -= 2.f * M_PI; // rad in range [-pi, pi]
	}

	float velocity_ms 		= ground_speed_K / 1.9438445f;
	float velocity_north 	= velocity_ms * cosf(track_rad);
	float velocity_east  	= velocity_ms * sinf(track_rad);
	int utc_hour 			= static_cast<int>(utc_time / 10000);
	int utc_minute 			= static_cast<int>((utc_time - utc_hour * 10000) / 100);
	double utc_sec 			= static_cast<double>(utc_time - utc_hour * 10000 - utc_minute * 100);
	int nmea_day 			= static_cast<int>(nmea_date / 10000);
	int nmea_mth 			= static_cast<int>((nmea_date - nmea_day * 10000) / 100);
	int nmea_year 			= static_cast<int>(nmea_date - nmea_day * 10000 - nmea_mth * 100);
	/* convert from degrees, minutes and seconds to degrees */
    /*
	_gps_position->lat = static_cast<int>((int(lat * 0.01) + (lat * 0.01 - int(lat * 0.01)) * 100.0 / 60.0) * 10000000);
	_gps_position->lon = static_cast<int>((int(lon * 0.01) + (lon * 0.01 - int(lon * 0.01)) * 100.0 / 60.0) * 10000000);
	_gps_position->vel_m_s = velocity_ms;
	_gps_position->vel_n_m_s = velocity_north;
	_gps_position->vel_e_m_s = velocity_east;
	_gps_position->cog_rad = track_rad;
	_gps_position->vel_ned_valid = true; // Flag to indicate if NED speed is valid
	_gps_position->c_variance_rad = 0.1f;
	_gps_position->s_variance_m_s = 0;
	_gps_position->timestamp = gps_absolute_time();
	_last_timestamp_time     = gps_absolute_time();
    if (Status == 'V') {_gps_position->fix_type = 0;}
    */

    if (Status == 'V')
    {
        return INT64_MAX;
    }
	/*
	 * convert to unix timestamp
	 */

	static constexpr int64_t china_time_zone = 3600 * 1e6 * 8;

	std::tm timeinfo;
	timeinfo.tm_year 		= nmea_year + 100;
	timeinfo.tm_mon  		= nmea_mth - 1;
	timeinfo.tm_mday 		= nmea_day;
	timeinfo.tm_hour 		= utc_hour;
	timeinfo.tm_min  		= utc_minute;
	timeinfo.tm_sec  		= int(utc_sec);
	timeinfo.tm_isdst 		= 0;
	int64_t t 	 			= std::mktime(&timeinfo);
	if(t == -1) return INT64_MAX; // value is meaningful
	
	int64_t gps_timestamp 		= t * 1000 + (int(utc_sec) - utc_sec) * 1000;
	
	return gps_timestamp * 1000 + china_time_zone; // to mircosecond
}

class SystemHardSync::Impl
{
public:
    Impl(DJIVehicleSystem* system)
    {
        _system = system;
    }

    void startSync()
    {
        _system->trace("start gps time sync");
        _system->vehicle()->hardSync->subscribeNMEAMsgs(
            NMEACallback, this
        );
    }

    static void NMEACallback(Vehicle *vehiclePtr,
                  RecvContainer recvFrame,
                  UserData userData)
    {
		using namespace std::chrono;

        auto* impl = (SystemHardSync::Impl*)(userData);

        int length = recvFrame.recvInfo.len - OpenProtocol::PackageMin - 4;
		
        int64_t rmc_timestamp = getTimeFromGXRMC(recvFrame.recvData.raw_ack_array, length);
		if(rmc_timestamp != INT64_MAX)
		{
			impl->_system->updateSystemBias(
				duration_cast<microseconds>(system_clock::now().time_since_epoch()).count() - rmc_timestamp
			);
		}
    }

    DJIVehicleSystem* _system;
};

SystemHardSync::SystemHardSync(DJIVehicleSystem* system)
{
    _impl = new Impl(system);
}

SystemHardSync::~SystemHardSync()
{
    delete _impl;
}

void SystemHardSync::startSync()
{
    _impl->startSync();
}
