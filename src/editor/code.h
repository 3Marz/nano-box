#ifndef EDITOR_CODE_H
#define EDITOR_CODE_H

#include "../../include/sds.h"

typedef struct CodeEditor {
	int yoff;
	int col;
	int row;
	int len;
	sds* data;
} CodeEditor;

CodeEditor* code_init();
void code_close(CodeEditor *e);

void insert_new_line(CodeEditor *e);

void delete_char(CodeEditor *e);
void delete_line(CodeEditor *e);

#endif
