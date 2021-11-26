#pragma once
#include <array>

using Vector3 = std::array<double ,3>;

/**
 * @brief 
 * 
 * @param lat 
 * @param lon 
 * @param alt 
 * @return Vector3 
 */
Vector3 LLA2ECEF(double lat, double lon, double alt);

/**
 * @brief 
 * 
 * @param lla 
 * @return Vector3 
 */
Vector3 LLA2ECEF(const Vector3& lla);
