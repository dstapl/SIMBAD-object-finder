#include <assert.h>
#include <stdio.h>
#include <string.h>

#include "VOTABLE_SQLITE.h"
#include "dev-utils.h"
#include "sqlite3.h"
#include "VOTABLE_XML.h"

static char *strndup(const char *s, size_t n) {
    size_t len = strnlen(s, n);
    char *p = malloc(len + 1);
    if (p) {
        memcpy(p, s, len);
        p[len] = '\0';
    }
    return p;
}

const char* IGNORE_TAGS[] = {
	"VOTABLE",
	"DEFINITIONS",
	"COOSYS",
	"RESOURCE",
	"DESCRIPTION", // TODO: Use this to generate descriptions to show the user
	"LINK",
	"DATA", // Adjacent to <TABLEDATA> so just check one or the other
};

// Boolean result: 1 is success
// TODO: Assumes both strings have the same capitalisation
// TODO(API): Take in list of IGNORE_TAGS instead of global?
static int tag_is_ignored(const char *tag_name) {
	for (size_t i = 0; i < sizeof(IGNORE_TAGS)/sizeof(IGNORE_TAGS[0]); i++) {
		const char* ignored_tag = IGNORE_TAGS[i];
		if (strcmp(tag_name, ignored_tag) == 0) {
			// Success
			return 1;
		}
	}

	// Failure
	return 0;
}

// Boolean result: 1 indicates error (Unsupported XML element)
static int handle_unknown_tag(const char *tag_name, XML_TAG_POSITION tag_open) {
	// Check if we've encountered an unsupported tag
	if (!tag_is_ignored(tag_name)) {
		// Not supported
		switch (tag_open) {
			case XML_TAG_OPEN:
				perror("Encountered unsupported XML element (OPEN TAG): ");
				break;
			case XML_TAG_CLOSE:
				perror("Encountered unsupported XML element (CLOSE TAG): ");
				break;
			default:
				UNIMPLEMENTED("Unsupported XML element type");
		}
		perror(tag_name);
		return 1;
	}
	// Otherwise just do nothing
	return 0;
}

// Boolean result: 1 indicates error (See handle_unknown_tag)
static int handle_unknown_tag_s(XML_ParserState *state, const char *tag_name, XML_TAG_POSITION tag_open) {
	// TODO: Maybe change name to reflect error return value
	if (handle_unknown_tag(tag_name, tag_open)) {
		// Unknown
		print_state(state);
		abort();
		return 1;
	}

	return 0;
}

void print_state(XML_ParserState *state) {
	printf("DEBUG: PRINTING XML_ParserState\n");
	printf("\tinTD: %d\n", state->inTD);
	printf("\tinTR: %d\n", state->inTR);
	printf("\t(XML)section: %d\n", state->section);
	printf("END DEBUG\n");
	// Include SQL?
	// print_ctx(state->ctx);
}

// Only need to check for Field tags etc.
void start_element_in_definitions(XML_ParserState *state, const char *name, const char **atts) {
	if (strcmp(name, "FIELD") == 0) {
		state->inFIELD = 1;

		const char *id = NULL;
		for (int i = 0; atts[i]; i += 2) {
			if (strcmp(atts[i], "name") == 0)
				id = atts[i+1];
		}
		UNUSED_VAR(id);


				

		// printf("Field: %s\n", id ? id : "(no name)");
	} else if (strcmp(name, "TABLEDATA") == 0) {
		// Switch section immediately
		// printf("DEBUG: Switching DIRECTLY from XML_FIELDS to XML_TABLEDATA\n");
		state->section = XML_TABLEDATA;
	}
	// TODO: Handle field descriptions tag?
}


// Only need to check for Field tags etc.
void start_element_in_tabledata(XML_ParserState *state, const char *name, const char **atts) {
	UNUSED_VAR(atts);

	if (strcmp(name, "TD") == 0) {
        state->inTD = 1;
    } else if (strcmp(name, "TR") == 0) {
		state->inTR = 1;
	}
}


// Boolean return: 1 is changed section
static int change_section_from_tag(XML_ParserState *state, const char* tag_name, XML_TAG_POSITION tag_open) {
	int changed_section = 1; // Assume changed (True)

	if (strcmp(tag_name, "TABLE") == 0) {
		if (tag_open) {
			// printf("== START TABLE ==\n");
			state->section = XML_FIELDS;
		} else {
			// printf("== END TABLE ==\n");
			// printf("DEBUG: Exiting XML_FIELDS");
			state->section = XML_OTHER_SECTION;
		}
	} else if (strcmp(tag_name, "TABLEDATA") == 0) {
		if (tag_open) {
			state->section = XML_TABLEDATA;
		} else {
			// printf("DEBUG: Exiting XML_TABLEDATA");
			state->section = XML_OTHER_SECTION;
		}
	}
	else {
		// No matches so section hasn't changed
		changed_section = 0;
	}

	return changed_section;
}

void XMLCALL
startElement(void *userData, const char *name, const char **atts) {
    XML_ParserState *state = (XML_ParserState*)userData;

	switch (state->section) {
		case XML_FIELDS:
			// If <TABLEDATA> tag is encountered
			// switch to XML_TABLEDATA
			start_element_in_definitions(state, name, atts);
			break;
		case XML_TABLEDATA:
			start_element_in_tabledata(state, name, atts);
			break;
		case XML_OTHER_SECTION:
			if (!change_section_from_tag(state, name, XML_TAG_OPEN)){
				handle_unknown_tag_s(state, name, XML_TAG_OPEN);
			}
			break;
	}
}



// Only need to check for Field tags etc.
void end_element_in_definitions(XML_ParserState *state, const char *name) {
	if (strcmp(name, "FIELD") == 0) {
		UNUSED_VAR(name);

		state->inFIELD = 0;
		// Write out field to SQL buffer
	}
}


// Only need to check for Field tags etc.
void end_element_in_tabledata(XML_ParserState *state, const char *name) {
	if (strcmp(name, "TD") == 0) {
        state->inTD = 0;
		state->cell_field_idx++;
    } else if (strcmp(name, "TR") == 0) {
		state->inTR = 0;

		// Bind and insert object SQL stmt to buffer
		SQL_DB_STATE *ctx = state->ctx;
		sqlite3_stmt *stmt = ctx->stmt;

		// Build insertion stmt for the current row
		// Iterating over all stored TD values
		for (int i = 0; i < state->cell_field_idx; i++) {
			sqlite3_bind_text(stmt, i + 1, state->row_cells[i] ? state->row_cells[i] : "", -1, SQLITE_TRANSIENT);

			// Free cells as we go along
			free(state->row_cells[i]);
			state->row_cells[i] = NULL;
		}

		// 
		int rc = sqlite3_step(stmt);
		if (rc != SQLITE_DONE) {
			fprintf(stderr, "Failed to execute insert: %s\n", sqlite3_errmsg(ctx->db));
			// handle error or abort
		}
		sqlite3_reset(stmt);

		ctx->batch_count++;

		// If bigger than batch size, commit the transaction
		if (ctx->batch_count >= SQL_INSERT_BATCH_SIZE) {
			sqlite3_exec(ctx->db, "COMMIT;", NULL, NULL, NULL);
			sqlite3_exec(ctx->db, "BEGIN TRANSACTION;", NULL, NULL, NULL);

			// Reset batch count
			ctx->batch_count = 0;
		}

		// Reset column
		state->cell_field_idx = 0;
	}
}



void XMLCALL
endElement(void *userData, const char *name) {
    XML_ParserState *state = (XML_ParserState*)userData;


	switch (state->section) {
		case XML_FIELDS:
			end_element_in_definitions(state, name);
			break;
		case XML_TABLEDATA:
			end_element_in_tabledata(state, name);
			break;
		case XML_OTHER_SECTION:
			if (!change_section_from_tag(state, name, XML_TAG_CLOSE)){
				handle_unknown_tag_s(state, name, XML_TAG_CLOSE);
			}
			break;
	}

}

void XMLCALL
charData(void *userData, const XML_Char *s, int len) {
	UNUSED_VAR(s);
	UNUSED_VAR(len);

    XML_ParserState *state = (XML_ParserState*)userData;
    // if (state->inTD) {
    //     // printf("Cell: %.*s\t", len, s);  // Data inside <TD>
    // }



    if (state->inTD) {
        // Free old buffer of current cell, if any
        if (state->row_cells[state->cell_field_idx] != NULL) {
            free(state->row_cells[state->cell_field_idx]);
            state->row_cells[state->cell_field_idx] = NULL;
        }

		// Remove this comment? Also how is it called multiple times
        // Allocate and copy current data chunk (may be called multiple times per cell)
        state->row_cells[state->cell_field_idx] = strndup(s, len);
    }
}



SQL_Result sql_close(SQL_DB_STATE *ctx) {
	// Destroy prepared statements
	sqlite3_finalize(ctx->stmt);
	sqlite3_finalize(ctx->insert_stmt_template);

	// Close db connection
	sqlite3_close(ctx->db);

	return SQL_OK;
};




// static void create_insert_template(SQL_DB_STATE *ctx) {
// 	// Prepare insert SQL once; example SQL for 3 columns:
// 	const char *insert_sql = "INSERT INTO your_table (col1, col2, col3) VALUES (?, ?, ?);";
// 	int rc = sqlite3_prepare_v2(ctx->db, insert_sql, -1, &ctx->insert_stmt_template, NULL);
// 	if (rc != SQLITE_OK) {
// 		fprintf(stderr, "Failed to prepare insert statement: %s\n", sqlite3_errmsg(ctx->db));
//
// 		// Clean up & close database
// 		SQL_Result db_close_res = sql_close(ctx);
// 		if (!db_close_res.ok) {
// 			fprintf(stderr, "Failed to close database: %d\n", db_close_res.error);
// 		}
// 	}
// };
//
int VOTABLE_XML_PARSE(const char* filename, SQL_DB_STATE *ctx) {
	FILE *file;
	FILE_OPEN(file, filename, "r");
	

    XML_Parser parser = XML_ParserCreate(NULL);
    XML_ParserState state = {0};
	state.ctx = ctx; // Bind SQL database to state

    XML_SetUserData(parser, &state); // Bind XML Parser to state

    XML_SetElementHandler(parser, startElement, endElement);
    XML_SetCharacterDataHandler(parser, charData);

    char buf[SQL_BUF_SIZE];
    size_t len;
    int done;

    do {
        len = fread(buf, 1, sizeof(buf), file);
        done = len < sizeof(buf); // Buffer not saturated

        if (XML_Parse(parser, buf, (int)len, done) == XML_STATUS_ERROR) {
			// Error
            fprintf(stderr, "Parse error: %s at line %lu\n",
                    XML_ErrorString(XML_GetErrorCode(parser)),
                    XML_GetCurrentLineNumber(parser));

            XML_ParserFree(parser);
			FILE_CLOSE(file, filename);

            return 1;
        }
    } while (!done);


	printf("End of operations\n");
	print_state(&state);

	sqlite3_exec(ctx->db, "COMMIT;", NULL, NULL, NULL);
	assert(ctx->insert_stmt_template && "Insert template was never set");
	if (ctx->insert_stmt_template) {
		sqlite3_finalize(ctx->insert_stmt_template);
	} else {
		perror("WARNING: insert_stmt_template was never set.\
				There may be uncommitted data");
	}



    XML_ParserFree(parser);
	FILE_CLOSE(file, filename);

    return 0;
}



// UNUSED FUNCTIONS. IGNORE

// int votable_print_table(VOTABLE *votable) {
// 	UNUSED_VAR(votable);
//
// 	UNIMPLEMENTED("votable_print_table");
// }
//
//
// int votable_load(VOTABLE *votable, const char *filename) {
// 	UNUSED_VAR(votable);
//
// 	UNIMPLEMENTED("UNFINISHED: votable_load");
//
// 	// Read in file
// 	FILE *file;
// 	FILE_OPEN(file, filename, "r");
//
// 	// Read in XML
//
// 	printf("End of operations");
// 	FILE_CLOSE(file, filename);
//
// 	return 0;
// }
//
//
// int votable_parse_xml() {
// 	UNIMPLEMENTED("votable_parse_xml");
// }


