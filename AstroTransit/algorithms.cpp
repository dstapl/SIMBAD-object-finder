#include <cstdint>
#include <cmath>
#include "constants.h"
#include "algorithms.h"

namespace algorithms {
	// Julian Day
	double JD(uint16_t year, uint16_t month, uint16_t day, uint8_t hour, uint8_t minute, uint8_t second) {
		double A = trunc(year / 100.0);
		double B = 2.0 - A + trunc(A / 4.0);
		// Overall day without any further precision
		double jd = trunc(365.25 * (year + 4716)) + trunc(30.6001 * (month + 1)) + day + B - 1524.5;
		
		return jd + (hour - 12) / 24.0 + minute / 1440.0 + second / 86400.0;
	}

	// Julian Ephemeris Day
	
}