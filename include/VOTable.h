/// TODO: All the function naming for `get_field_type` etc.
#ifndef _FILEREADING_
#define _FILEREADING_
#pragma once


#include <stdint.h>

#include "Map.h"
#include "Vector.h"


typedef enum {
	FIELD_MISSING = 0,
    Char, // String / char*
    Float,
    Double,
    Short,
    Int,
} VOTABLE_FieldType;

typedef struct {
	const char *name;
	VOTABLE_FieldType type;
} VOTABLE_FieldEntry;

typedef struct {
	// FIELD_MISSING is no-value --> Reading value is UB
    VOTABLE_FieldType type;
    union {
        char   *s;
        int     i;
        float   f;
        double  d;
    } value;
} VOTABLE_FieldResult;


// X-Macro magic
#define FIELD(name, type) +1
enum {
	NUM_FIELD_HEADERS =
#include "VOTABLE_fields.def"
};
#undef FIELD

extern const char *FIELD_HEADERS[NUM_FIELD_HEADERS];
extern const VOTABLE_FieldEntry FIELD_TYPE_MAP[NUM_FIELD_HEADERS];



// TODO: What needs pointers and what doesn't?
// Map allocates by bucket pointers
// so should VOTABLE.rows be a vector of *pointers* or *values*?


// Field definitions from SIMBAD XML files.
typedef struct VOTABLE_Row VOTABLE_Row;
// typedef union {
// 	char* ;
// 	int;
// 	double;
// 	float
// } FieldValue;
struct VOTABLE_Row {
	Map fields; // char* to char*
    // Row(rapidxml::xml_node<> *tablerow);
	uint16_t num_fields; // Set on inialization
};

typedef struct VOTABLE VOTABLE;
struct VOTABLE {
    vector(VOTABLE_Row*) rows; // Row
	// Set on inialization
	size_t num_rows;
};


VOTABLE_FieldType votable_get_field_type(const char *key);

char* votable_get_raw_value_checked(VOTABLE_Row *row, const char *key);
VOTABLE_FieldResult votable_get_field_value_as(VOTABLE_Row *row, const char *key, VOTABLE_FieldType want);
int votable_field_value_to_string(VOTABLE_FieldResult field_result, char *out_str, size_t bufsize);
vector(VOTABLE_Row*) *votable_get_table_data(VOTABLE *table);
// Free maps stored in each row
int votable_deinit(VOTABLE *votable);


#endif
