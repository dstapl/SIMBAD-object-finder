#include <cstdint>
#define _USE_MATH_DEFINES
#include <cmath>

#include <utility>
#include <tuple>
#include <functional>

#include <stdexcept>

#include "constants.h"

// TODO: How to get DMS type without reincluding the header file?
#include "algorithms.h"

namespace algorithms {
	namespace date {
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
			return jd + constants::astro::deltaT / 86400.0;
		}

		// Julian Year
		double JY(double jd) {
			return (jd - constants::astro::J2000) / 365.25;
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
	}

	namespace astro_helper {
		using algorithms::astro_helper::dms_type;

		std::tuple<double, double> proper_motion(double jd, double pm_ra, double pm_dec) {
			if (pm_ra == 0.0 && pm_dec == 0.0) {
				return { 0, 0 };
			}

			double current_julian_year = algorithms::date::JY(jd);

			// Changes in RA and Dec due to proper motion
			double delta_ra = current_julian_year * pm_ra;
			double delta_dec = current_julian_year * pm_dec;

			// Convert mas to deg
			delta_ra /= 3600000.0;
			delta_dec /= 3600000.0;

			return { delta_ra, delta_dec };
		}

		double quadrant_ra(double ra) {
			return std::fmod(ra, 360.0);
		}
		double quadrant_dec(double dec) {
			// Already in-bounds
			if (-90 <= dec && dec <= 90) {
				return dec;
			}

			// Get remainder after division then wrap to [-90, 90]
			dec = std::fmod(dec, 90.0);
			dec = std::fmod(dec + 90.0, 360.0);

			if (dec > 45) {
				return dec - 90.0;
			}
			else {
				return dec;
			}

		}

		std::tuple<double, double> correct_coordinates(double jd, double ra, double dec, double pm_ra, double pm_dec) {
			auto const& [delta_pm_ra, delta_pm_dec] = proper_motion(jd, pm_ra, pm_dec);

			double delta_ra = 0, delta_dec = 0;
			// TODO: Include effects of solar precession
			// Otherwise can remove delta_ra/dec values as 0

			const double eps_ra = delta_ra + delta_pm_ra;
			const double eps_dec = delta_dec + delta_pm_dec;

			return { quadrant_ra(ra + eps_ra), quadrant_dec(dec + eps_dec) };
		};

		// TODO: Does this assume that the lat is in [0, 360) or [-180, 180)?
		// Similar question for dec?
		double coaltitude(double lat, double dec) {
			return 90.0 - (lat - dec);
		}

		std::tuple<double, bool> transit_altitude(double lat, double dec) {
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
			bool north = alt > 90;
			
			// Wrap into [-90, 90]
			if (north) {
				alt = 90.0 - std::fmod(alt, 90.0);
			}
			else {
				alt = std::fmod(alt, 90.0);
			}

			return { alt, north };
		}

		dms_type deg_to_dms(double angle) {
			// #error "not implemented yet"
			uint32_t deg = std::trunc(angle);
			double rem = std::fmod(angle, 1.0);

			rem = rem * 60.0;
			uint32_t min = std::trunc(rem);
			rem = std::fmod(rem, 1.0);

			uint32_t sec = rem * 60.0;
			
			return { deg, min, sec };
		}
		
		dms_type deg_to_hours(double angle) {
			// 15 degrees / hour
			return deg_to_dms(angle / 15.0);
		}

		double dms_to_deg(uint32_t deg, uint32_t min, double sec) {
			return deg + (min / 60.0) + sec / 3600.0;
		}
		double dms_to_deg(dms_type dms) {
			auto const& [deg, min, sec] = dms;
			return dms_to_deg(deg, min, sec);
		}
		
		uint32_t hours_to_deg(uint32_t hour, uint32_t min, double sec) {
			// 15 degrees / hour
			return static_cast<uint32_t>(dms_to_deg(hour * 15, min, sec));
		}

		uint32_t hours_to_deg(dms_type dms) {
			auto const& [hour, min, sec] = dms;
			return hours_to_deg(hour, min, sec);
		}

		std::string dms_to_string(dms_type dms, AngleFormat format) {
			// TODO: Better name than `big_unit`
			auto& [big_unit, min, sec] = dms;

			// Round sec to 3 decimal places
			// NOTE: This is an approximation
			sec = std::round(sec * 1000.0) / 1000.0;

			// Define string unit characters
			const char* big_unit_str;
			const char* min_str;
			const char* sec_str;

			switch (format) {
				case AngleFormat::DMS:
					big_unit_str = "°";
					min_str = "'";
					sec_str = "\"";
					break;
				case AngleFormat::Hours:
					big_unit_str = "h";
					min_str = "m";
					sec_str = "s";
					break;
				default:
					throw std::invalid_argument("Invalid angle format specified");
			};

			// Convert to string based on format
			std::string result = std::to_string(big_unit) + big_unit_str + " " +
				std::to_string(min) + min_str + " " +
				std::to_string(sec) + sec_str;

			return result;
		}


	}

	namespace astro_algorithms {
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


	}
}