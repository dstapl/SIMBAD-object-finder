#ifndef _ASTROALGORITHMS_
#define _ASTROALGORITHMS_
#pragma once


#include <stdint.h>
#include <math.h>


// size_t
#define NUMBER_OBJECTS 5

// Heliocentric longitude
double L(double jd);
// Heliocentric latitude
double B(double jd);
// Earth's radius vector
double R(double jd);
// Geocentric longitude
double theta(double jd);
// Geocentric latitude;
inline double beta(double jd) { return -B(jd); };
		
// TODO: Rename
// Mean anomaly of all (significant) planets
// Includes Moon (from Sun), Sun*, Moon, Argument of latitude of Moon, Longitude of ascending node of Moon's mean orbit
// *Mean anomaly of the Sun is actually the Earth's anomaly around the Sun
int generate_anomaly_vector(double src_vector[NUMBER_OBJECTS], double jd);

// Nutation in longitude (psi)
double nutation_longitude(double jd);
// Nutation in obliquity (epsilon)
double nutation_obliquity(double jd);

// Mean	obliquity of the ecliptic (in arcseconds)
double mean_obliquity(double jd);
// Apparent solar longitude (in decimal degrees)
double apparent_solar_longitude(double jd);
// Apparent sidereal time at Greenwich (in decimal degrees)
double apparent_sidereal_time(double jd);

// Geocentric right ascension of the Sun (in degrees)
double sun_right_ascension(double jd);
// Geocentric declination of the Sun (in degrees)
double sun_declination(double jd);

// Observer's local hour angle (in degrees)
double local_hour_angle(double jd);

// Solar right ascension parallax (in degrees)
double solar_right_ascension_parallax(double jd);
// Solar declination parallax (in degrees)
double solar_declination_parallax(double jd);

		
// Topocentric solar right ascension (in degrees)
double topocentric_solar_right_ascension(double jd);
// Topocentric solar declination (in degrees)
double topocentric_solar_declination(double jd);

// Topocentric solar hour angle (in degrees)
double topocentric_solar_hour_angle(double jd);
// Topocentric zenith angle (in degrees)
double topocentric_zenith_angle(double jd);
// Topocentric astronomer's azimuth (in degrees)
// TODO: What is the difference between this and azimuth?
double topocentric_astronomers_azimuth(double jd);
// Topocentric azimuth (in degrees)
inline double topocentric_azimuth(double jd) {
	return fmod(topocentric_astronomers_azimuth(jd) + 180.0, 360.0);
}

#endif
