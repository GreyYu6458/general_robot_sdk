#include "p_rsdk/tools/math/LLA2ECEF.hpp"
#include <cmath>

static constexpr double WGS84_A = 6378137;
static constexpr double WGS84_E = 8.1819190842622e-2;
static constexpr double DEG2RAD = M_PI / 180;

Vector3 LLA2ECEF(double lat, double lon, double alt)
{
    /*
    Converts latitude (deg), longitude (deg) and altitude (m) to ECEF frame
    https://en.wikipedia.org/wiki/Geographic_coordinate_conversion#From_geodetic_to_ECEF_coordinates
    
    Returns:
    X, Y, Z in meters
    */
    double clat = cos(lat * DEG2RAD);
    double slat = sin(lat * DEG2RAD);
    double clon = cos(lon * DEG2RAD);
    double slon = sin(lon * DEG2RAD);

    double N = WGS84_A / sqrt(1.0 - (WGS84_E*WGS84_E) * (slat*slat));

    double X = (N + alt) * clat * clon;
    double Y = (N + alt) * clat * slon;
    double Z = (N * (1.0 - WGS84_E * WGS84_E) + alt) * slat;
    
    return {X, Y, Z};
}

Vector3 LLA2ECEF(const Vector3& lla)
{
    return LLA2ECEF(lla[0], lla[1], lla[2]);
}