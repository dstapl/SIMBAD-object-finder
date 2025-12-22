#ifndef _ASTROCONSTANTS_
#define _ASTROCONSTANTS_
#pragma once


#include <math.h>


// TODO: value type
// TODO: keys for RapidXML parser may not be the same as Julia
typedef struct {
	char *xml_type;
	char *c_type;
} TypeEntry;


// Names to display to user
extern const char *name_list[];
// Type mapping from ? to c_types
extern const TypeEntry type_map[];

// TODO: Implement RapidXML functions
// 12:00 Noon 1st Jan 2000
extern const double J2000;
// Mean longitude of the Sun at J2000
extern const double L2000;
// Mean anomaly of the Sun at J2000
extern const double M2000;
// Orbital eccentricity of Earth
extern const double e_earth;
// Obliquity of Earth's orbit at J2000
extern const double obl_earth_2000;
// See https://en.wikipedia.org/wiki/%CE%94T_(timekeeping)
extern const double deltaT;

// Earth periodic angle terms
extern const double L_values[6][64][4];
extern const double B_values[2][5][4];
extern const double R_values[5][40][4];
extern const double Y_values[63][5];
extern const double psi_epsilon_values[63][4];


// Helper function to extract a column from a 2D array
// double *row[M][4]
void get_column(size_t M, const double (*row)[M][4], size_t col_idx, double *out);

// Inner product of two arrays length n
double inner_product(size_t n, const double (*A)[n], const double (*B)[n]);

// Constructs the final values given the set of values
// NOTE: Only used for R_values, L_values, and B_values
// double *T[N][M][4]
double constructValue(size_t N, size_t M, const double (*T)[N][M][4], double jd);

#endif
