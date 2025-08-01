#pragma once
#include <cstdint>
#include <tuple>
#include <string>

// Julian Day
double JD(uint16_t year, uint16_t month, uint16_t day, uint8_t hour = 0, uint8_t minute = 0, uint8_t second = 0);
// Julian Ephemeris Day
double JDE(double jd);
// Julian Year
double JY(double jd);
// Julian Ephemeris Year
double JYE(double jd);
// Julian Century
double JC(double jd);
// Julian Ephemeris Century
double JCE(double jd);
// Julian Millenium
double JM(double jd);
// Julian Ephemeris Millenium
double JME(double jd);

// TODO: Rewrite using a signatures using a "position" object
// containing jd, pm values, ra/dec etc.?

using dms_type = std::tuple<uint32_t, uint32_t, double>;

enum class AngleFormat {
	DMS, // Degrees, Minutes, Seconds
	Hours // Hours, Minutes, Seconds
};

// proper motions measured in mas
std::tuple<double, double> proper_motion(double jd, double pm_ra, double pm_dec);

// Quadrant on an x-y plane (for atan purposes)

// Returns ra in [0, 360) degrees
double quadrant_ra(double ra);
// Returns dec in [-90, 90] degrees
double quadrant_dec(double dec);

// Corrects (ra, dec) for proper motions
std::tuple<double, double> correct_coordinates(double jd, double ra, double dec, double pm_ra, double pm_dec);

// Returns true if x is between the given range y
// inclusive - whether to include the endpoints of the range in the bounds check
template<typename T, typename U>
bool between_wrapping(T x, std::tuple<U, U> range, bool inclusive) {
	const auto& [start, end] = range;
	
	if (inclusive) {
		const bool wrapping = end <= start;
		if (wrapping) {
			return (x <= end) || (start <= x);
		}
		else {
			return (start <= x) && (x <= end);
		};
	}
	else {
		const bool wrapping = end < start;
		if (wrapping) {
			return (x < end) || (start < x);
		}
		else {
			return (start < x) && (x < end);
		};
	};
};

/*
template<typename T>
bool rbetween(T x, T y, bool inclusive = false);
*/

// Returns the coaltitude at a given observer latitude and object declination
double coaltitude(double lat, double dec);

// Returns the object altitude when transiting the observer's meridian
std::tuple<double, bool> transit_altitude(double lat, double dec);

// Converts decimal angle (degrees) to full DMS (degrees, minutes, seconds)
dms_type deg_to_dms(double angle);
// Converts decimal angle (degrees) to hours, minutes, seconds
// (1 hour = 15 degrees)
dms_type deg_to_hours(double angle);

// TODO: These methods seem duplicated, consider removing one

// Converts DMS (degrees, minutes, seconds) to decimal angle (degrees)
double dms_to_deg(dms_type dms);
// Converts DMS (degrees, minutes, seconds) to decimal angle (degrees)
double dms_to_deg(uint32_t deg, uint32_t min, double sec);

// Converts hours, minutes, seconds to decimal angle (degrees)
uint32_t hours_to_deg(uint32_t hour, uint32_t min, double sec);
// Converts hours, minutes, seconds to decimal angle (degrees)
uint32_t hours_to_deg(dms_type dms);

// Converts DMS (degrees, minutes, seconds) to string representation
// Format: "deg° min' sec\"" or "hour h min m sec s"
std::string dms_to_string(dms_type dms, AngleFormat format = AngleFormat::DMS);

