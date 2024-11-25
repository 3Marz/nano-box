#ifndef EDITOR_CODE_H
#define EDITOR_CODE_H

#include "../../include/sds.h"

extern const char keywords[][10];

typedef struct CodeEditor {
	int yoff;
	int col;
	int row;
	int scrolly;
	int scrollx;

	int len;
	sds*  data; // the code string array
	int** syntax; // same length as data
} CodeEditor;

CodeEditor* code_init();
void code_close(CodeEditor *e);

void insert_new_line(CodeEditor *e);
void code_update_syntax(CodeEditor *e);

void delete_char(CodeEditor *e);
void delete_line(CodeEditor *e);

#endif
