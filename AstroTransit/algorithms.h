#pragma once
#include <cstdint>
#include <tuple>
#include <string>
#include <functional>
#include <array>

namespace algorithms {
	namespace date {
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
	}

	// TODO: Rewrite using a signatures using a "position" object
	// containing jd, pm values, ra/dec etc.?
	namespace astro_helper {
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
		
		// TODO(Refactor): Redudant code?
		template<typename T, typename U>
		bool between(T x, std::tuple <U, U> y, bool inclusive) {
			/*if (inclusive) {
				return std::get<0>(y) <= x && x <= std::get<0>(y);
			}
			else {
				return std::get<0>(y) < x && x < std::get<0>(y);
			}*/
			return between2(x, y, inclusive);
		}

		// Returns true if x is between the given range y
		// inclusive - whether to include the endpoints of the range in the bounds check
		template<typename T, typename U>
		bool between2(T x, std::tuple<U, U> y, bool inclusive) {
			std::array < std::function<bool(T, U)>, 2> op_arr;
			op_arr = { std::less<T>(), std::less_equal<T>() };

			unsigned int idx = (int)inclusive;
			std::function<bool(T,U)> op = op_arr[idx];
			
			// Allows for LHS element to be greater than RHS element of range
			U first;
			U second;
			std::tie(first, second) = y;
			if (op(second, first)) {
				return !(op(second, x) && op(x, first));
			}
			else {
				return op(first, x) && op(x, second);
			}
		}

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

	}

	// TODO: Name better
	namespace astro_algorithms {
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
		
		// Mean anomaly of all (significant) planets
		// Includes Moon (from Sun), Sun*, Moon, Argument of latitude of Moon, Longitude of ascending node of Moon's mean orbit
		// *Mean anomaly of the Sun is actually the Earth's anomaly around the Sun
		std::array<double, 5> anomaly_vector(double jd);
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
	}
}