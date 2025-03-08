#include <cstdint>
#include <cmath>

#include <utility>
#include <tuple>
#include <functional>

#include "constants.h"
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
			double delta_pm_ra, delta_pm_dec;
			std::tie(delta_pm_ra, delta_pm_dec) = proper_motion(jd, pm_ra, pm_dec);
			
			double delta_ra = 0, delta_dec = 0;
			// TODO: Include effects of solar precession
			// Otherwise can remove delta_ra/dec values as 0

			double eps_ra = delta_ra + delta_pm_ra;
			double eps_dec = delta_dec + delta_pm_dec;

			return { quadrant_ra(ra + eps_ra), quadrant_dec(dec + eps_dec) };
		};

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
			double alt = 90.0 + delta * sign;
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
			return dms_to_deg(std::get<0>(dms), std::get<1>(dms), std::get<2>(dms));
			//return std::apply(deg_to_dms, dms);
		}

		
	}
}