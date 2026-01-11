#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "VOTABLE_SQLITE.h"

static const char* votable_type_to_sql(const char *t) {
	if (!strcmp(t,"int") || !strcmp(t,"long") || !strcmp(t,"short")) return "INTEGER";
	if (!strcmp(t,"float") || !strcmp(t,"double")) return "REAL";
	if (!strcmp(t,"boolean")) return "INTEGER";         // store booleans as 0/1
	return "TEXT";
}


// Optimise for sparse <TABLEDATA> storage
// 246 columns in <TABLEDATA> but most <TD> will be empty
static const char *PRAGMA_LIST[] = {
	// For large XML docs
	"journal_mode=WAL",
	"synchronous=OFF",
	"temp_store=MEMORY",
	// "mmap_size=30000000000",  // if 64-bit system
	// "cache_size = -8000000", // 8GB cache (negative = KB)
};


SQL_Result sql_open(SQL_DB_STATE *ctx, const char *filename) {
	// memset(ctx, 0, sizeof(*ctx)); // Reset ctx
	if (sqlite3_open(filename, &ctx->db) != SQLITE_OK) {
		fprintf(stderr, "[sqlite] open error: %s\n", sqlite3_errmsg(ctx->db));
		return SQL_ERROR(SQL_ERR_OPEN);
	}

	for (size_t i = 0; i < sizeof(PRAGMA_LIST)/sizeof(PRAGMA_LIST[0]); i++) {
		// WARNING: Does not check for buffer overflow assignment
		char PRAGMA_BUF[SQL_COL_STR_BUF_SIZE];		

		strcat(PRAGMA_BUF, "PRAGMA ");
		strcat(PRAGMA_BUF, PRAGMA_LIST[i]);
		strcat(PRAGMA_BUF, ";");

		sqlite3_exec(ctx->db, PRAGMA_BUF, NULL, NULL, NULL);

		// Clear buffer
		memset(PRAGMA_BUF, 0, SQL_COL_STR_BUF_SIZE);
	}

	sqlite3_exec(ctx->db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

	return SQL_OK;
}

SQL_Result sql_close(SQL_DB_STATE *ctx) {
	if (ctx->stmt) sqlite3_finalize(ctx->stmt);
	if (ctx->db) {
		sqlite3_exec(ctx->db, "COMMIT;", NULL, NULL, NULL);
		sqlite3_close(ctx->db);
	}
	free(ctx->fields);
	ctx->fields = NULL;
	ctx->added_field_count = 0;

	return SQL_OK;
}

// Add one field from the XML
SQL_Result sql_add_field(SQL_DB_STATE *ctx, SQL_FIELD field) {
	ctx->fields = realloc(ctx->fields, sizeof(SQL_FIELD) * (ctx->added_field_count + 1));
	if (!ctx->fields) return SQL_ERROR(SQL_ERR_STATE);
	ctx->fields[ctx->added_field_count++] = field;
	return SQL_OK;
}

// Create table & metadata table
SQL_Result sql_build_schema(SQL_DB_STATE *ctx, const char *table_name) {
	if (table_name) {
		strncpy(ctx->tablename, table_name, MAX_TABLENAME_LENGTH - 1);
	};

	// Construct TABLE creation query
	char sql[SQL_BUF_SIZE] = {0};
	snprintf(sql, sizeof(sql), "CREATE TABLE IF NOT EXISTS \"%s\" (\n", ctx->tablename);

	for (int i = 0; i < ctx->added_field_count; i++) {
		char col[SQL_COL_STR_BUF_SIZE];

		snprintf(col, sizeof(col), "\"%s\" %s%s\n",
				ctx->fields[i].name,
				votable_type_to_sql(ctx->fields[i].datatype),
				(i < ctx->added_field_count - 1 ? "," : "")
		);

		strcat(sql, col);
	}
	strcat(sql, ");");

	if (sqlite3_exec(ctx->db, sql, NULL, NULL, NULL) != SQLITE_OK) {
		fprintf(stderr, "[sqlite] table create error: %s\n", sqlite3_errmsg(ctx->db));
		return SQL_ERROR(SQL_ERR_CREATE);
	}

	return SQL_OK;
}


// Build insert_smt_template: INSERT INTO table VALUES (?, ?, ?, ...)
SQL_Result sql_prepare_insert_template(SQL_DB_STATE *ctx, sqlite3_stmt *stmt) {
	// Create statement string
	char sql[SQL_BUF_SIZE] = {0};
	snprintf(sql, sizeof(sql), "INSERT INTO \"%s\" VALUES(", ctx->tablename);

	for (int i = 0; i < ctx->added_field_count; i++) {
		strcat(sql, (i == 0 ? "?" : ",?"));
	}
	strcat(sql, ");");

	// Prepare statement with the db ctx
	// Bind to ctx->insert_stmt_template
	if (sqlite3_prepare_v2(ctx->db, sql, -1, &stmt, NULL) != SQLITE_OK) {
		fprintf(stderr, "[sqlite] prepare error: %s\nSQL was: %s\n", sqlite3_errmsg(ctx->db), sql);
		return SQL_ERROR(SQL_ERR_SQL);
	}
	return SQL_OK;
}

// Insert single row: cell_values[i] is raw TD string
SQL_Result sql_insert_row(SQL_DB_STATE *ctx, char **cell_values) {
	for (int i = 0; i < ctx->added_field_count; i++) {
		const char *raw = cell_values[i];
		const char *dtype = ctx->fields[i].datatype;

		if (!raw || strlen(raw) == 0) {
			sqlite3_bind_null(ctx->stmt, i+1);
		}
		else if (!strcmp(dtype,"int") || !strcmp(dtype,"long") || !strcmp(dtype,"short") || !strcmp(dtype,"boolean")) {
			sqlite3_bind_int(ctx->stmt, i+1, atoi(raw));
		}
		else if (!strcmp(dtype,"float") || !strcmp(dtype,"double")) {
			sqlite3_bind_double(ctx->stmt, i+1, atof(raw));
		}
		else {
			sqlite3_bind_text(ctx->stmt, i+1, raw, -1, SQLITE_TRANSIENT);
		}
	}

	if (sqlite3_step(ctx->stmt) != SQLITE_DONE) {
		fprintf(stderr, "[sqlite] insert error: %s\n", sqlite3_errmsg(ctx->db));
		sqlite3_reset(ctx->stmt);
		return SQL_ERROR(SQL_ERR_INSERT);
	}

	if (!sqlite3_reset(ctx->stmt)) {
		return SQL_ERROR(SQL_ERR_STATE);
	}

	return SQL_OK;
}

// Destory prepared statement and reset pointer in ctx
// Will always set the value of stmt to null
SQL_Result sql_finish_insert(sqlite3_stmt *stmt) {
	SQL_Result res = SQL_OK;

	if (stmt && (sqlite3_finalize(stmt) != SQLITE_OK)) {
		res = SQL_ERROR(SQL_ERR_STATE);
	}
	stmt = NULL;

	return res;
}

