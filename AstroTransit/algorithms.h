#pragma once
#include <cstdint>
#include <tuple>
#include <string>
#include <functional>
#include <array>

namespace algorithms {
	namespace date {
		double JD(uint16_t year, uint16_t month, uint16_t day, uint8_t hour = 0, uint8_t minute = 0, uint8_t second = 0);
		double JDE(double jd);
		double JY(double jd);
		double JYE(double jd);
		double JC(double jd);
		double JCE(double jd);
		double JM(double jd);
		double JME(double jd);
	}

	// TODO: Rewrite using a signatures using a "position" object
	// containing jd, pm values, ra/dec etc.?
	namespace astro_helper {
		using dms_type = std::tuple<uint32_t, uint32_t, double>;

		// proper motions measured in mas
		std::tuple<double, double> proper_motion(double jd, double pm_ra, double pm_dec);
		
		// Quadrant on an x-y plane (for atan purposes)

		double quadrant_ra(double ra);
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

		template<typename T>
		bool rbetween(T x, T y, bool inclusive = false);

		double coaltitude(double lat, double dec);

		std::tuple<double, bool> transit_altitude(double dec);
	
		dms_type deg_to_dms(double angle);
		dms_type deg_to_hours(double angle);

		double dms_to_deg(dms_type dms);
		double dms_to_deg(uint32_t deg, uint32_t min, double sec);

		std::string dms_to_string(dms_type dms);
		


	}
}