#define _USE_MATH_DEFINES

#include <math.h>
#include <string.h> // For memcpy only...

#include "constants.h"
// TODO: How to get DMS type without reincluding the header file?
#include "algorithms.h"
#include "astro_helpers.h"


// TODO: Put these in the header file?
double rad2deg(double rad) {
	// Convert radians to degrees
	return rad * (180.0 / M_PI);
}

double deg2rad(double deg) {
	// Convert degrees to radians
	return deg * (M_PI / 180.0);
}

double arcsec2rad(double arcsec) {
	// Convert arcseconds to radians
	// Good approximation
	return arcsec * (M_PI / 206265);
}

double L(double jd) {
	size_t N = sizeof(L_values[0]);
	size_t M = sizeof(L_values[0][0]); 

	double L_angle = constructValue(N, M, &L_values, jd);

	return fmod(rad2deg(L_angle), 360.0); // Range 0-360
}

double B(double jd) {
	size_t N = sizeof(B_values[0]);
	size_t M = sizeof(B_values[0][0]); 

	double B_angle = constructValue(N, M, &B_values, jd);
	return rad2deg(B_angle);
}

double R(double jd) {
	size_t N = sizeof(R_values[0]);
	size_t M = sizeof(R_values[0][0]); 

	double R_angle = constructValue(N, M, &R_values, jd);
	return rad2deg(R_angle);
}

double theta(double jd) {
	return fmod(
		L(jd) + 180.0,
		360.0
	);
}


// Implementing Julia's @evalpoly macro;
// Calculating powers of value x
double evalpoly(size_t n, double x, double coeffs[n])
{
    double result = coeffs[n - 1];
    for (size_t i = n - 1; i > 0; i--) {
        result = coeffs[i] + x * result;
    }
    return result;
}

// #define EVALPOLY(x, ...) evalpoly(sizeof((double[]){__VA_ARGS__}), x, (double[]){__VA_ARGS__})
#define EVALPOLY(x, ...) ({                                 \
    double _coeffs[] = { __VA_ARGS__ };                     \
    evalpoly(sizeof(_coeffs)/sizeof(_coeffs[0]), x, _coeffs); \
})



// Copies the angle anomaly for each factor into the provided src_vector
int generate_anomaly_vector(double src_vector[NUMBER_OBJECTS], double jd) {
	const double curr_JCE = JCE(jd);

	// Mean elongation of the moon from the sun
	const double moon_sun_elongation = EVALPOLY(curr_JCE, 297.85036, 445267.111480, -0.0019142, 1.0 / 189474.0);

	// Mean anomaly of the sun (*Earth)
	const double sun_mean_anomaly = EVALPOLY(curr_JCE, 357.52772, 35999.050340, -0.0001603, -1.0 / 300000.0);

	// Mean anomaly of the moon
	const double moon_mean_anomaly = EVALPOLY(curr_JCE, 134.96298, 477198.867398, 0.0086972, 1.0 / 56250.0);

	// Argument of latitude (Moon)
	const double moon_argument_latitude = EVALPOLY(curr_JCE, 93.27191, 483202.017538, -0.0036825, 1.0 / 327270.0);

	//Longitude of ascending node of the moon's mean orbit
	// (On Ecliptic, measured from mean Equinox on current date)
	const double moon_mean_longitude_ascending = EVALPOLY(curr_JCE, 125.04452, -1934.136261, 0.0020708, 1.0 / 450000.0);

	// TODO: Check number of {} entries == NUMBER_OBJECTS
	// Create initial array of results
	double tmp[NUMBER_OBJECTS] = { moon_sun_elongation, sun_mean_anomaly, moon_mean_anomaly, moon_argument_latitude, moon_mean_longitude_ascending};
	// Restrict all values from range of [0,360] degrees
	for (size_t i = 0; i < NUMBER_OBJECTS; i++) {
		tmp[i] = fmod(tmp[i], 360.0);
	}

	// Copy over to source
	memcpy(src_vector, tmp, sizeof(tmp) );

	return 0;
}
