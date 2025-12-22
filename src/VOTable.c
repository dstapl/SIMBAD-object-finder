#include "VOTable.h"


const FieldEntry FIELD_TYPE_MAP[NUM_FIELD_HEADERS] = {
#define FIELD(name, type) { #name, type },
#include "VOTable_fields.def"
#undef FIELD
};

const char *FIELD_HEADERS[NUM_FIELD_HEADERS] = {
#define FIELD(name, type) #name,
#include "VOTable_fields.def"
#undef FIELD
};


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

char* get_raw_value_checked(Row *row, const char *key) {
	if ( (row == NULL) || (row->fields.buckets == NULL) ) {
		return NULL;
	} 

	return map_get(&(row->fields), key);
}


FieldResult get_field_value_as(Row *row, const char *key, FieldType want) {
	FieldResult res = { FIELD_MISSING, {0} };

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

vector(Row) *getTableData(VOTable *table) {
    return &table->rows;
}


// TODO: XML operations
