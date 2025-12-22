/// TODO: All the function naming for `get_field_type` etc.
#ifndef _FILEREADING_
#define _FILEREADING_
#pragma once


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


// X-Macro magic
enum {
	NUM_FIELD_HEADERS =
#define FIELD(name, type) +1
#include "VOTable_fields.def"
#undef FIELD
};

extern const char *FIELD_HEADERS[NUM_FIELD_HEADERS];
extern const FieldEntry FIELD_TYPE_MAP[NUM_FIELD_HEADERS];


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

typedef struct VOTable VOTable;
struct VOTable {
    // Success or failure writing into `tableData`
	int (*load)(VOTable *self, const char*filename);

    void (*printTable)(VOTable *self);

    vector(Row) rows; // Row
};


// Need to keep on the stack so it can be used across this header file
//	at compile time
FieldType get_field_type(const char *key);

char* get_raw_value_checked(Row *row, const char *key);
FieldResult get_field_value_as(Row *row, const char *key, FieldType want);

vector(Row) *getTableData(VOTable *table);


#endif
