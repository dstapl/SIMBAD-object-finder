#ifndef _ASTROHELPERS_
#define _ASTROHELPERS_
#pragma once


#include <stdint.h>

// Julian Day
double JD(uint16_t year, uint16_t month, uint16_t day, uint8_t hour, uint8_t minute, uint8_t second);
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

// using dms_type = std::tuple<uint32_t, uint32_t, double>;
typedef struct {
	uint32_t deg;
	uint32_t min;
	uint32_t sec;
} dms_type;

typedef enum {
	DMS, // Degrees, Minutes, Seconds
	Hours // Hours, Minutes, Seconds
} AngleFormat;

// proper motions measured in mas

typedef struct {
	double ra;
	double dec;
} angle_pos;

angle_pos proper_motion(double jd, double pm_ra, double pm_dec);


// Quadrant on an x-y plane (for atan purposes)

// Returns ra in [0, 360) degrees
double quadrant_ra(double ra);
// Returns dec in [-90, 90] degrees
double quadrant_dec(double dec);

// Corrects (ra, dec) for proper motions
angle_pos correct_coordinates(double jd, double ra, double dec, double pm_ra, double pm_dec);

// Returns true if x is between the given range y
// inclusive(bool) - whether to include the endpoints of the range in the bounds check
#define BETWEEN_WRAPPING(value, start, end, inclusive)            \
    ((inclusive)                                                   \
        ? (((end) <= (start))                                      \
            ? ((value) <= (end) || (start) <= (value))             \
            : ((start) <= (value) && (value) <= (end)))            \
        : (((end) < (start))                                       \
            ? ((value) < (end) || (start) < (value))               \
            : ((start) < (value) && (value) < (end))))



/*
template<typename T>
bool rbetween(T x, T y, bool inclusive = false);
*/

// Returns the coaltitude at a given observer latitude and object declination
double coaltitude(double lat, double dec);

// Returns the object altitude when transiting the observer's meridian
typedef struct {
	double transit_alt;
	int above_horizon; // bool
} transit_info;
transit_info transit_altitude(double lat, double dec);

// Converts decimal angle (degrees) to full DMS (degrees, minutes, seconds)
dms_type deg_to_dms(double angle);
// Converts decimal angle (degrees) to hours, minutes, seconds
// (1 hour = 15 degrees)
dms_type deg_to_hours(double angle);

// TODO: These methods seem duplicated, consider removing one

// Converts DMS (degrees, minutes, seconds) to decimal angle (degrees)
double dms_to_degDMS(dms_type dms);
// Converts DMS (degrees, minutes, seconds) to decimal angle (degrees)
double dms_to_degSeparate(uint32_t deg, uint32_t min, double sec);

#define dms_to_deg(x, ...) \
    _Generic((x), \
        dms_type: dms_to_degDMS, \
        uint32_t: dms_to_degSeparate \
    )(x, ##__VA_ARGS__)

// Converts hours, minutes, seconds to decimal angle (degrees)
uint32_t hours_to_degDMS(dms_type dms);
// Converts hours, minutes, seconds to decimal angle (degrees)
uint32_t hours_to_degSeparate(uint32_t hour, uint32_t min, double sec);

#define hours_to_deg(x, ...) \
    _Generic((x), \
        dms_type: hours_to_degDMS, \
        uint32_t: hours_to_degSeparate \
    )(x, ##__VA_ARGS__)

// Converts DMS (degrees, minutes, seconds) to string representation
// Format: "deg min' sec\"" or "hour h min m sec s"
// char* dms_to_string(dms_type dms, AngleFormat format);
int dms_to_string(size_t out_size, char out[out_size], dms_type dms, AngleFormat format);

#endif
