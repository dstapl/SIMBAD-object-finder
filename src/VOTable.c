#include <stdio.h>
#include <stdlib.h>

#include "VOTable.h"
#include "Map.h"
#include "Vector.h"
#include "dev-utils.h"



const VOTABLE_FieldEntry FIELD_TYPE_MAP[NUM_FIELD_HEADERS] = {
#define FIELD(name, type) { #name, type },
#include "VOTABLE_fields.def"
#undef FIELD
};

const char *FIELD_HEADERS[NUM_FIELD_HEADERS] = {
#define FIELD(name, type) #name,
#include "VOTABLE_fields.def"
#undef FIELD
};


VOTABLE_FieldType votable_get_field_type(const char *key) {
	for (size_t i=0; i<sizeof(FIELD_TYPE_MAP)/sizeof(FIELD_TYPE_MAP[0]); i++)
	{
		VOTABLE_FieldEntry entry = FIELD_TYPE_MAP[i];
		// Match found
		if (entry.name == key) {
			return entry.type;
		}
	}

	// Can't find a match
	return FIELD_MISSING;
}

char* votable_get_raw_value_checked(VOTABLE_Row *row, const char *key) {
	if ( (row == NULL) || (row->fields.buckets == NULL) ) {
		return NULL;
	} 

	return map_get(&(row->fields), key);
}


VOTABLE_FieldResult votable_get_field_value_as(VOTABLE_Row *row, const char *key, VOTABLE_FieldType want) {
	VOTABLE_FieldResult res = { FIELD_MISSING, {0} };

	char *value = votable_get_raw_value_checked(row, key);

	if (value == NULL) {
		return res; // Currently defaulted to missing
	}

	// Find actual type from variant or default to string
	VOTABLE_FieldType actual = votable_get_field_type(key);
	if (actual != want) {
		return res;
	}

	// Check if template type matches the enum variant for this value
	res.type = want;
	switch (actual) {
	case Float:
		res.value.f = (float)atof(value);
		break;
	case Int:
		res.value.i = atoi(value);
		break;
	case Double:
		res.value.d = atof(value);
		break;
	case Char: // String
		res.value.s = value;
		break;
	default:
		// No matches so must be a missing field
		res.type = FIELD_MISSING;
		break;
	}

	return res;
}


// Convert value of field_result into a string.
//
// Error if out_str is not allocated with enough length.
int votable_field_value_to_string(VOTABLE_FieldResult field_result, char *out_str, size_t bufsize) {
    switch (field_result.type) {
        case Char:
            snprintf(out_str, bufsize, "%s", field_result.value.s);
            break;
        case Int:
            snprintf(out_str, bufsize, "%d", field_result.value.i);
            break;
        case Float:
            snprintf(out_str, bufsize, "%.3f", field_result.value.f);
            break;
        case Double:
            snprintf(out_str, bufsize, "%.3lf", field_result.value.d);
            break;
        default:
            snprintf(out_str, bufsize, "MISSING");
            break;
    }
    return 0;
}


int votable_deinit(VOTABLE *votable) {
	int error_occured_flag = 0;

	size_t number_rows = vector_count(votable->rows);
	for (size_t i = 0; i < number_rows; i++) {
		int error = map_deinit(&votable->rows[i]->fields);

		if (error != 0) { // TODO: Replace with bit-or toggle?
			error_occured_flag = 1;
		}
	}
	
	return error_occured_flag;
}

vector(VOTABLE_Row*) *votable_get_table_data(VOTABLE *table) {
    return &table->rows;
}

