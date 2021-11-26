#include "p_rsdk/tools/math/haversine.h"
#include <math.h>

#define M_PI			3.14159265358979323846
#define D2R 			M_PI / 180;
#define EARTH_RADIUS  	6371.393f;

double haversine(double latitude_1, double longitude_1, double latitude_2, double longitude_2)
{
	double rlat_1	=  latitude_1 	* D2R;
	double rlat_2	=  latitude_2 	* D2R;
	double rlon_1	=  longitude_1 	* D2R;
	double rlon_2	=  longitude_2 	* D2R;

	double dlat = rlat_2 - rlat_1;
	double dlon = rlon_2 - rlon_1;

	double a = pow(sin(dlat / 2), 2) + cos(rlat_1) * cos(rlat_2) * pow(sin(dlon / 2), 2);
	return 2 * asin(sqrt(a)) * 1000 * EARTH_RADIUS;
}
