#ifndef __VOTABLE_XML__
#define __VOTABLE_XML__
#pragma once

#include "VOTable.h"

// TODO: void type instead of int?
//
// Print out table contents by row to STDOUT
int votable_print_table(VOTABLE *self);

// Boolean success
//
// Parse TABLEDATA_t (XML-type) into a vector of VOTABLE_Row* (user type)
int votable_load(VOTABLE *votable, const char *filename);


#include <expat.h>
#include "VOTABLE_SQLITE.h"


typedef enum {
	XML_OTHER_SECTION, // Rename
	XML_FIELDS, // Following <TABLE> tag but before <(TABLE)DATA> tag
	XML_TABLEDATA,
} XML_VOTABLE_SECTION;

typedef enum {
	XML_TAG_CLOSE = 0,
	XML_TAG_OPEN = 1,
} XML_TAG_POSITION;


typedef struct {
	int inFIELD; // Track description tag if/when it appears before closing tag

    int inTD; // Cells
	int inTR; // Row for each astro object

	// NOTE: ASSUMPTION that the VOTable XML is well formed
	// int inDEFINITIONS; // Only handle FIELD tag etc.
	// int inTABLEDATA; // Only handle TR/TD
	XML_VOTABLE_SECTION section;

	// SQL
	SQL_DB_STATE *ctx; // Storing SQL data during parse
	char *row_cells[NUM_FIELD_HEADERS];
	int cell_field_idx;
} XML_ParserState;


// Tags to ignore while parsing
// No errors will be produced for unsupported type
extern const char *IGNORE_TAGS[];

void XMLCALL
startElement(void *userData, const char *name, const char **atts);
void XMLCALL
endElement(void *userData, const char *name);
void XMLCALL
charData(void *userData, const XML_Char *s, int len);


void print_state(XML_ParserState *state);
int VOTABLE_XML_PARSE(const char* filename, SQL_DB_STATE *ctx);


#endif
