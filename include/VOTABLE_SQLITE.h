#ifndef __VOTABLE_SQLITE__
#define __VOTABLE_SQLITE__

#include <sqlite3.h>

// TODO: Find best values. Mainly for batch size
#define SQL_INSERT_BATCH_SIZE 1000
#define SQL_BUF_SIZE 8192
#define SQL_COL_STR_BUF_SIZE 256
#define SQLITE_BUSY_VALUE NULL

// From VOTable XML schema <FIELD name="" datatype="">
typedef struct {
    char id[64];
    char name[64];
    char datatype[32];
    char unit[32];
    char ucd[128];
    char description[256];
} SQL_FIELD;

typedef struct {
	int ok; // Truth value; 0 is okay
	enum error { // Error value
		SQL_ERR_STATE,
		SQL_ERR_OPEN,
		SQL_ERR_CREATE,
		SQL_ERR_CLOSE,
		SQL_ERR_MEMORY,
		SQL_ERR_INSERT,
		SQL_ERR_SQL,
	} error;
} SQL_Result;
#define SQL_OK (SQL_Result){.ok = 0};
#define SQL_ERROR(ERROR) (SQL_Result){.ok = 1, .error = ERROR};

// Store state
#define MAX_TABLENAME_LENGTH 128
typedef struct {
    sqlite3 *db;
    sqlite3_stmt *stmt;
	sqlite3_stmt *insert_stmt_template;

	// Replace with vector?
    SQL_FIELD *fields;
    int added_field_count;
    char tablename[MAX_TABLENAME_LENGTH];

	// When XML parsing
	int batch_count;
} SQL_DB_STATE;

extern sqlite3_stmt *sql_stmt_buffer;

SQL_Result sql_open(SQL_DB_STATE *ctx, const char *filename);
SQL_Result sql_close(SQL_DB_STATE *ctx);

SQL_Result sql_add_field(SQL_DB_STATE *ctx, SQL_FIELD field);
SQL_Result sql_build_schema(SQL_DB_STATE *ctx, const char *table_name);

SQL_Result sql_prepare_insert(SQL_DB_STATE *ctx);
SQL_Result sql_insert_row(SQL_DB_STATE *ctx, char **cell_values);
SQL_Result sql_finish_insert(sqlite3_stmt *stmt);


#endif
