#include <stdio.h>
#define _USE_MATH_DEFINES
#include <math.h>

#include <stdint.h>

#include "astro_helpers.h"
#include "constants.h"


// Julian Day
double JD(uint16_t year, uint16_t month, uint16_t day, uint8_t hour, uint8_t minute, uint8_t second) {
	double A = trunc(year / 100.0);
	double B = 2.0 - A + trunc(A / 4.0);
	// Overall day without any further precision
	double jd = trunc(365.25 * (year + 4716)) + trunc(30.6001 * (month + 1)) + day + B - 1524.5;

	return jd + (hour - 12) / 24.0 + minute / 1440.0 + second / 86400.0;
}

// Julian Ephemeris Day
double JDE(double jd) {
	return jd + deltaT / 86400.;
}

// Julian Year
double JY(double jd) {
	return (jd - J2000) / 365.25;
}

// Julian Ephemeris Year
double JYE(double jd) {
	return JY(JDE(jd));
}

// Julian Century
double JC(double jd) {
	return JY(jd) / 100.0;
}

// Julian Ephemeris Century
double JCE(double jd) {
	return JC(JDE(jd));
}

// Julian Millenium
double JM(double jd) {
	return JC(jd) / 10.0;
}

// Julian Ephemeris Millenium
double JME(double jd) {
	return JM(JDE(jd));
}

//using astro_helpers::dms_type;

angle_pos proper_motion(double jd, double pm_ra, double pm_dec) {
	if (pm_ra == 0.0 && pm_dec == 0.0) {
		// No change
		return (angle_pos){0, 0};
	}

	double current_julian_year = JY(jd);

	// Changes in RA and Dec due to proper motion
	double delta_ra = current_julian_year * pm_ra;
	double delta_dec = current_julian_year * pm_dec;

	// Convert mas to deg
	// angle/(360*10,000)
	delta_ra /= 3600000.0;
	delta_dec /= 3600000.0;

	return (angle_pos){ delta_ra, delta_dec };
}

double quadrant_ra(double ra) {
	return fmod(ra, 360.0);
}
double quadrant_dec(double dec) {
	// Already in-bounds
	if (-90 <= dec && dec <= 90) {
		return dec;
	}

	// Get remainder after division then wrap to [-90, 90]
	dec = fmod(dec, 90.0);
	dec = fmod(dec + 90.0, 360.0);

	if (dec > 45) {
		return dec - 90.0;
	}
	else {
		return dec;
	}

}

angle_pos correct_coordinates(double jd, double ra, double dec, double pm_ra, double pm_dec) {
	// auto const& [delta_pm_ra, delta_pm_dec] = proper_motion(jd, pm_ra, pm_dec);
	angle_pos motion = proper_motion(jd, pm_ra, pm_dec);

	angle_pos delta_motion = {0, 0};
	// double delta_ra = 0
	// double delta_dec = 0;
	// TODO: Include effects of solar precession
	// Otherwise can remove delta_ra/dec values as 0

	const double eps_ra = motion.ra + delta_motion.ra;
	const double eps_dec = motion.dec + delta_motion.dec;

	// Wrap angles to the correct quadrant between -360 < angle < 360 deg
	return (angle_pos){ quadrant_ra(ra + eps_ra), quadrant_dec(dec + eps_dec) };
};

// TODO: Does this assume that the lat is in [0, 360) or [-180, 180)?
// Similar question for dec?
double coaltitude(double lat, double dec) {
	return 90.0 - (lat - dec);
}

// Returns altitude and whether 
transit_info transit_altitude(double lat, double dec) {
	/* Transit altitude is 90 - lat + dec, or if | dec - lat | > 90 --> 180 - (90 - lat + dec)
	--> 90 - lat + dec or -90 + lat - dec
	--> 90 + (dec - lat) or -90 + (lat - dec) = - 90 - (dec - lat)
	--> +/- (90 + (dec - lat))
	*/
	double delta = dec - lat; // 90 - coaltitude
	/* Angles greater than 90deg --> Northen transit(Irregular, lower culmination)
		Angles less than 90deg --> Southern transit(Standard, upper culmination)
	*/

	int sign = (delta > -90) * 1 - (delta <= -90) * -1;
	double alt = 90.0 + sign * delta;
	int north = alt > 90; // bool

	// Wrap into [-90, 90]
	if (north) {
		alt = 90.0 - fmod(alt, 90.0);
	}
	else {
		alt = fmod(alt, 90.0);
	}

	return (transit_info){ alt, north };
}

dms_type deg_to_dms(double angle) {
	// #error "not implemented yet"
	uint32_t deg = trunc(angle);
	double rem = fmod(angle, 1.0);

	rem = rem * 60.0;
	uint32_t min = trunc(rem);
	rem = fmod(rem, 1.0);

	uint32_t sec = rem * 60.0;

	return (dms_type){ deg, min, sec };
}

dms_type deg_to_hours(double angle) {
	// 15 degrees / hour
	return deg_to_dms(angle / 15.0);
}

double dms_to_degSeparate(uint32_t deg, uint32_t min, double sec) {
	return deg + (min / 60.0) + sec / 3600.0;
}
double dms_to_degDMS(dms_type dms) {
	return dms_to_deg(dms.deg, dms.min, dms.sec);
}

uint32_t hours_to_degSeparate(uint32_t hour, uint32_t min, double sec) {
	// 15 degrees / hour
	return dms_to_deg(hour * 15, min, sec);
}
uint32_t hours_to_degDMS(dms_type dms) {
	return hours_to_deg(dms.deg, dms.min, dms.sec);
}


int dms_to_string(size_t out_size, char out_str[out_size],
                   dms_type dms, AngleFormat format)
{
    char big_unit_unit, min_unit, sec_unit;

    switch (format) {
    case DMS:
        big_unit_unit = 'o';
        min_unit = '\'';
        sec_unit = '"';
        break;
    case Hours:
        big_unit_unit = 'h';
        min_unit = 'm';
        sec_unit = 's';
        break;
    default:
        out_str[0] = '\0';
        return 1;
    }

	// Round sec to 3 decimal places
	// NOTE: This is an approximation
	double sec = round(dms.sec * 1000.0) / 1000.0;

	// Write out to 
    snprintf(out_str, out_size, "%u%c %u%c %.3f%c",
             dms.deg, big_unit_unit,
             dms.min, min_unit,
             sec, sec_unit);

	return 0;
}

