#include <stdio.h>

#include "dev-utils.h"

#include "VOTABLE_XML.h"

// SAFETY: Only use when map is populated with char* type for Key AND Value;
//	Otherwise UB
void print_str(const void *key, void *value, void *extra) {
	UNUSED_VAR(extra);
	printf("%s, %s\n", (char*)key, (char*)value);
}


const char *filename = "./Data/simbad_gt1_december.xml";
int main() {
	VOTABLE votable = {};
	SQL_DB_STATE ctx = {0};
	// if (votable_load(&votable, filename) == 1) {
	// 	return 1;
	// };
	if (VOTABLE_XML_PARSE(filename, &ctx) == 1) {
		return 1;
	}


	TODO("Waiting");

	// //votable.printTable();
	// vector(VOTABLE_Row*) *tableData = votable_get_table_data(&votable);
	//
	// const char *testkey = "MAIN_ID";
	//
	// VOTABLE_Row *row = *tableData[0];
	// VOTABLE_FieldResult value = votable_get_field_value_as(row, testkey, Char);
	// if (value.type != FIELD_MISSING) {
	// 	char value_str[20];
	// 	votable_field_value_to_string(value, value_str, sizeof(value_str));
	// 	printf("hello from %s\n", value_str);
	// } 
	// else {
	// 	perror("Value has a missing field type.");
	//
	// 	char* rawVal = votable_get_raw_value_checked(row, testkey);
	//
	// 	perror("Raw value is: ");
	// 	if (rawVal) {
	// 		perror(rawVal);
	// 	} else {
	// 		perror("NULL");
	// 	}
	//
	// 	perror("Key or type not found in row: ");
	// 	// for (size_t i = 0; i < sizeof(row.fields); i++) {
	// 	// 	// printf("%s", map_get_safe(row->fields, const char *str);
	// 	// 	printf("%s, %s\n", key_str, value_str);
	// 	// }
	// 	map_iter(&row->fields, &print_str, NULL);
	// 	perror("\n");
	// 	return 1;
	// };
	//
	//
	// Free at the end
	if (votable_deinit(&votable) != 0) {
		perror("Failed to deinit VOTABLE. Some data may still be allocated");
	}

	return 0;
}
