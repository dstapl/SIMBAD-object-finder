#define _USE_MATH_DEFINES
#include <cmath>
#include <cstdint>

#include <utility>
#include <tuple>
#include <functional>
#include <algorithm>

#include <stdexcept>

#include "constants.h"
// TODO: How to get DMS type without reincluding the header file?
#include "algorithms.h"
#include "astro_helpers.h"

// TODO: Put these in the header file?
static double rad2deg(double rad) {
	// Convert radians to degrees
	return rad * (180.0 / M_PI);
}

static double deg2rad(double deg) {
	// Convert degrees to radians
	return deg * (M_PI / 180.0);
}

static double arcsec2rad(double arcsec) {
	// Convert arcseconds to radians
	// Good approximation
	return arcsec * (M_PI / 206265);
}

double L(double jd) {
	return fmod(
		rad2deg(
			constants::astro::constructValue(
				constants::astro::L_values,
				jd
			)
		),
		360.0
	);
}

double B(double jd) {
	return rad2deg(
		constants::astro::constructValue(
			constants::astro::B_values,
			jd
		)
	);
}

double R(double jd) {
	return constants::astro::constructValue(
		constants::astro::R_values,
		jd
	);
}

double theta(double jd) {
	return fmod(
		L(jd) + 180.0,
		360.0
	);
}


// Implementing Julia's @evalpoly macro
template <typename T>
constexpr T evalpoly(T x, T a0) {
	return a0;
}

template <typename T, typename... Coeffs>
constexpr T evalpoly(T x, T a0, Coeffs... rest) {
	return a0 + x * evalpoly(x, static_cast<T>(rest)...);
}
#define EVALPOLY(x, ...) evalpoly(x, __VA_ARGS__)



std::array<double, 5> anomaly_vector(double jd) {
	const double curr_JCE = JCE(jd);

	// Mean elongation of the moon from the sun
	const double moon_sun_elongation = EVALPOLY(curr_JCE, 297.85036, 445267.111480, -0.0019142, 1 / 189474);

	// Mean anomaly of the sun (*Earth)
	const double sun_mean_anomaly = EVALPOLY(curr_JCE, 357.52772, 35999.050340, -0.0001603, -1 / 300000);

	// Mean anomaly of the moon
	const double moon_mean_anomaly = EVALPOLY(curr_JCE, 134.96298, 477198.867398, 0.0086972, 1 / 56250);

	// Argument of latitude (Moon)
	const double moon_argument_latitude = EVALPOLY(curr_JCE, 93.27191, 483202.017538, -0.0036825, 1 / 327270);

	//Longitude of ascending node of the moon's mean orbit
	// (On Ecliptic, measured from mean Equinox on current date)
	const double moon_mean_longitude_ascending = EVALPOLY(curr_JCE, 125.04452, -1934.136261, 0.0020708, 1 / 450000);

	// TODO: Name
	// Create initial array of results
	auto foo = std::array<double, 5>{ moon_sun_elongation, sun_mean_anomaly, moon_mean_anomaly, moon_argument_latitude, moon_mean_longitude_ascending };

	// Restrict all values from range of [0,360] degrees
	std::transform(foo.cbegin(), foo.cend(), foo.begin(), [](double angle) {return fmod(angle, 360.0); });

	return foo;
}
