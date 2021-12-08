#pragma
#include <tuple>

/**
 * @brief 给出基于椭球面两坐标点(LLA)之间的距离以及方位角
 * 
 * @param latp      第一个点的纬度
 * @param latc      第二个点的纬度
 * @param longp     第一个点的经度
 * @param longc     第二个点的经度
 * @return std::tuple<double, double> first 是 距离 second 是方位角
 */
std::tuple<double, double> vinc(double latp, double latc, double longp, double longc);

