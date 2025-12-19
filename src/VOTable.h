/// TODO: All the function naming for `get_field_type` etc.
#pragma once
#ifndef _FILEREADING_
#define _FILEREADING_

// Remove if including <stdlib.h>
// #define NULL ((void*)0)

// #include <rapidxml_utils.hpp>
#include "Map.h"
#include "Vector.h"

typedef enum {
	FIELD_MISSING = 0,
    Char, // String / char*
    Float,
    Double,
    Short,
    Int,
} FieldType;

typedef struct {
	const char *name;
	FieldType type;
} FieldEntry;


typedef struct {
	// FIELD_MISSING is no-value --> Reading value is UB
    FieldType type;
    union {
        char   *s;
        int     i;
        float   f;
        double  d;
    } value;
} FieldResult;

#define NUM_FIELD_HEADERS 246
extern const char *FIELD_HEADERS[NUM_FIELD_HEADERS];

// Copied from XML definitions
static const FieldEntry FIELD_TYPE_MAP[NUM_FIELD_HEADERS] = {
    {"TYPED_ID", Char},
    {"MATCHING_ID", Char},
    {"ANG_DIST", Float},
    {"MAIN_ID", Char},
    {"OTYPE_S", Char},
    {"RA_d", Double},
    {"DEC_d", Double},
    {"COO_ERR_MAJA_d", Float},
    {"COO_ERR_MINA_d", Float},
    {"COO_ERR_ANGLE_d", Short},
    {"PMRA", Double},
    {"PMDEC", Double},
    {"PM_ERR_MAJA", Float},
    {"PM_ERR_MINA", Float},
    {"PM_ERR_ANGLE", Short},
    {"PLX_VALUE", Double},
    {"RV_VALUE", Double},
    {"Z_VALUE", Double},
    {"VLSR", Double},
    {"GALDIM_MAJAXIS", Float},
    {"GALDIM_MINAXIS", Float},
    {"GALDIM_ANGLE", Short},
    {"SP_TYPE", Char},
    {"MORPH_TYPE", Char},
    {"NB_REF", Int},
    {"FILTER_NAME_U", Char},
    {"FLUX_U", Float},
    {"FLUX_ERROR_U", Float},
    {"FLUX_SYSTEM_U", Char},
    {"FLUX_BIBCODE_U", Char},
    {"FLUX_VAR_U", Char},
    {"FLUX_MULT_U", Char},
    {"FLUX_QUAL_U", Char},
    {"FLUX_UNIT_U", Char},
    {"FILTER_NAME_B", Char},
    {"FLUX_B", Float},
    {"FLUX_ERROR_B", Float},
    {"FLUX_SYSTEM_B", Char},
    {"FLUX_BIBCODE_B", Char},
    {"FLUX_VAR_B", Char},
    {"FLUX_MULT_B", Char},
    {"FLUX_QUAL_B", Char},
    {"FLUX_UNIT_B", Char},
    {"FILTER_NAME_V", Char},
    {"FLUX_V", Float},
    {"FLUX_ERROR_V", Float},
    {"FLUX_SYSTEM_V", Char},
    {"FLUX_BIBCODE_V", Char},
    {"FLUX_VAR_V", Char},
    {"FLUX_MULT_V", Char},
    {"FLUX_QUAL_V", Char},
    {"FLUX_UNIT_V", Char},
    {"FILTER_NAME_G", Char},
    {"FLUX_G", Float},
    {"FLUX_ERROR_G", Float},
    {"FLUX_SYSTEM_G", Char},
    {"FLUX_BIBCODE_G", Char},
    {"FLUX_VAR_G", Char},
    {"FLUX_MULT_G", Char},
    {"FLUX_QUAL_G", Char},
    {"FLUX_UNIT_G", Char},
    {"FILTER_NAME_R", Char},
    {"FLUX_R", Float},
    {"FLUX_ERROR_R", Float},
    {"FLUX_SYSTEM_R", Char},
    {"FLUX_BIBCODE_R", Char},
    {"FLUX_VAR_R", Char},
    {"FLUX_MULT_R", Char},
    {"FLUX_QUAL_R", Char},
    {"FLUX_UNIT_R", Char},
    {"FILTER_NAME_I", Char},
    {"FLUX_I", Float},
    {"FLUX_ERROR_I", Float},
    {"FLUX_SYSTEM_I", Char},
    {"FLUX_BIBCODE_I", Char},
    {"FLUX_VAR_I", Char},
    {"FLUX_MULT_I", Char},
    {"FLUX_QUAL_I", Char},
    {"FLUX_UNIT_I", Char},
    {"FILTER_NAME_J", Char},
    {"FLUX_J", Float},
    {"FLUX_ERROR_J", Float},
    {"FLUX_SYSTEM_J", Char},
    {"FLUX_BIBCODE_J", Char},
    {"FLUX_VAR_J", Char},
    {"FLUX_MULT_J", Char},
    {"FLUX_QUAL_J", Char},
    {"FLUX_UNIT_J", Char},
    {"FILTER_NAME_H", Char},
    {"FLUX_H", Float},
    {"FLUX_ERROR_H", Float},
    {"FLUX_SYSTEM_H", Char},
    {"FLUX_BIBCODE_H", Char},
    {"FLUX_VAR_H", Char},
    {"FLUX_MULT_H", Char},
    {"FLUX_QUAL_H", Char},
    {"FLUX_UNIT_H", Char},
    {"FILTER_NAME_K", Char},
    {"FLUX_K", Float},
    {"FLUX_ERROR_K", Float},
    {"FLUX_SYSTEM_K", Char},
    {"FLUX_BIBCODE_K", Char},
    {"FLUX_VAR_K", Char},
    {"FLUX_MULT_K", Char},
    {"FLUX_QUAL_K", Char},
    {"FLUX_UNIT_K", Char},
    {"FILTER_NAME_u", Char},
    {"FLUX_u", Float},
    {"FLUX_ERROR_u", Float},
    {"FLUX_SYSTEM_u", Char},
    {"FLUX_BIBCODE_u", Char},
    {"FLUX_VAR_u", Char},
    {"FLUX_MULT_u", Char},
    {"FLUX_QUAL_u", Char},
    {"FLUX_UNIT_u", Char},
    {"FILTER_NAME_g", Char},
    {"FLUX_g", Float},
    {"FLUX_ERROR_g", Float},
    {"FLUX_SYSTEM_g", Char},
    {"FLUX_BIBCODE_g", Char},
    {"FLUX_VAR_g", Char},
    {"FLUX_MULT_g", Char},
    {"FLUX_QUAL_g", Char},
    {"FLUX_UNIT_g", Char},
    {"FILTER_NAME_r", Char},
    {"FLUX_r", Float},
    {"FLUX_ERROR_r", Float},
    {"FLUX_SYSTEM_r", Char},
    {"FLUX_BIBCODE_r", Char},
    {"FLUX_VAR_r", Char},
    {"FLUX_MULT_r", Char},
    {"FLUX_QUAL_r", Char},
    {"FLUX_UNIT_r", Char},
    {"FILTER_NAME_i", Char},
    {"FLUX_i", Float},
    {"FLUX_ERROR_i", Float},
    {"FLUX_SYSTEM_i", Char},
    {"FLUX_BIBCODE_i", Char},
    {"FLUX_VAR_i", Char},
    {"FLUX_MULT_i", Char},
    {"FLUX_QUAL_i", Char},
    {"FLUX_UNIT_i", Char},
    {"FILTER_NAME_z", Char},
    {"FLUX_z", Float},
    {"FLUX_ERROR_z", Float},
    {"FLUX_SYSTEM_z", Char},
    {"FLUX_BIBCODE_z", Char},
    {"FLUX_VAR_z", Char},
    {"FLUX_MULT_z", Char},
    {"FLUX_QUAL_z", Char},
    {"FLUX_UNIT_z", Char},
    {"Diameter_diameter", Double},
    {"Diameter_Q", Char},
    {"Diameter_unit", Char},
    {"Diameter_error", Double},
    {"Diameter_filter", Char},
    {"Diameter_method", Char},
    {"Diameter_bibcode", Char},
    {"Distance_distance", Double},
    {"Distance_Q", Char},
    {"Distance_unit", Char},
    {"Distance_merr", Double},
    {"Distance_perr", Double},
    {"Distance_method", Char},
    {"Distance_bibcode", Char},
    {"Fe_H_Teff", Int},
    {"Fe_H_log_g", Float},
    {"Fe_H_Fe_H", Float},
    {"Fe_H_flag", Char},
    {"Fe_H_CompStar", Char},
    {"Fe_H_CatNo", Char},
    {"Fe_H_bibcode", Char},
    {"Herschel_ObsId", Char},
    {"Herschel_alpha", Char},
    {"Herschel_delta", Char},
    {"PLX_plx", Float},
    {"PLX_me", Float},
    {"PLX_R", Char},
    {"PLX_bibcode", Char},
    {"PM_pmra", Float},
    {"PM_me_pmra", Float},
    {"PM_pmde", Float},
    {"PM_me_pmde", Float},
    {"PM_system", Char},
    {"PM_bibcode", Char},
    {"ROT_upVsini", Char},
    {"ROT_Vsini", Float},
    {"ROT_err", Float},
    {"ROT_mes", Int},
    {"ROT_qual", Char},
    {"ROT_bibcode", Char},
    {"V__vartyp", Char},
    {"V__Lomax", Char},
    {"V__max", Float},
    {"V__R_max", Char},
    {"V__band", Char},
    {"V__Upmin", Char},
    {"V__min", Float},
    {"V__R_min", Char},
    {"V__UpPeriod", Char},
    {"V__period", Double},
    {"V__R_period", Char},
    {"V__epoch", Double},
    {"V__R_epoch", Char},
    {"V__D_rt", Float},
    {"V__R_D_rt", Char},
    {"V__bibcode", Char},
    {"velocities_type", Char},
    {"velocities_Value", Double},
    {"velocities_R", Char},
    {"velocities_me", Double},
    {"velocities_Acc", Char},
    {"velocities_Nmes", Int},
    {"velocities_nat", Char},
    {"velocities_Q", Char},
    {"velocities_dom", Char},
    {"velocities_res", Float},
    {"velocities_d", Char},
    {"velocities_Date", Double},
    {"velocities_Rem", Char},
    {"velocities_Origin", Char},
    {"velocities_bibcode", Char},
    {"ISO_TDT", Char},
    {"ISO_alpha", Double},
    {"ISO_delta", Double},
    {"IUE_Homogenized_Name", Char},
    {"IUE_ComplID", Char},
    {"IUE_PROG", Char},
    {"IUE_CL", Int},
    {"IUE_D", Char},
    {"IUE_CAM", Char},
    {"IUE_IMAGE", Int},
    {"IUE_A", Char},
    {"IUE_FES", Int},
    {"IUE_MD", Char},
    {"IUE_ObsDate", Char},
    {"IUE_Time", Char},
    {"IUE_ExpTim", Int},
    {"IUE_m", Char},
    {"IUE_CEB", Char},
    {"IUE_S", Char},
    {"IUE_Comments", Char},
    {"IUE_F", Char},
    {"IUE_bibcode", Char},
    {"XMM_Obsno", Char},
    {"OID4", Char},
};

// Need to keep on the stack so it can be used across this header file
//	at compile time
FieldType get_field_type(const char *key) {
	for (size_t i=0; i<sizeof(FIELD_TYPE_MAP)/sizeof(FIELD_TYPE_MAP[0]); i++)
	{
		FieldEntry entry = FIELD_TYPE_MAP[i];
		// Match found
		if (entry.name == key) {
			return entry.type;
		}
	}

	// Can't find a match
	return FIELD_MISSING;
}

// Field definitions from SIMBAD XML files.
typedef struct Row Row;
// typedef union {
// 	char* ;
// 	int;
// 	double;
// 	float
// } FieldValue;
struct Row {
	Map fields; // char* to char*
    // Row(rapidxml::xml_node<> *tablerow);
};

char* get_raw_value_checked(Row *row, const char *key) {
	if ( (row == NULL) || (row->fields.buckets == NULL) ) {
		return NULL;
	} 

	return map_get(&(row->fields), key);
}

FieldResult get_field_value_as(Row *row, const char *key, FieldType want) {
	FieldResult res = { FIELD_MISSING };


	char *value = get_raw_value_checked(row, key);

	if (value == NULL) {
		return res; // Currently defaulted to missing
	}

	// Find actual type from variant or default to string
	FieldType actual = get_field_type(key);
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


typedef struct VOTable VOTable;
struct VOTable {
    // Success or failure writing into `tableData`
	int (*load)(VOTable *self, const char*filename);

    void (*printTable)(VOTable *self);

    vector(Row) rows; // Row

};


vector(Row) *getTableData(VOTable *table) {
    return &table->rows;
}



#endif
