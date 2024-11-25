
#include "code.h"
#include "../utils.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>


const char keywords[][10] = {
	"and", "break", "do", "else", "elseif", "end", "false", "for", "function", "if", "in", "local", "nil", "not", "or", "repeat", "return", "then", "true", "until", "while", "goto",

	"time", "getkeys", "mouse", "btn", "btnp",
	"peek", "peek2", "peek4", "poke", "poke2",
	"cls", "pxset", "pxget", "line", "text", "rect", "rectf", "circ", "circf", "spr"
};

CodeEditor* code_init() {
	CodeEditor *e = malloc(sizeof(CodeEditor));
	e->yoff = 9;
	e->col = 0;
	e->row = 0;
	e->scrollx = 0;
	e->scrolly = 0;
	e->len = 1;
	e->data = (sds *)malloc(e->len*sizeof(sds));
	if (e->data == NULL) {
		fprintf(stderr, "Editor: Could not create code editor\n");
		return NULL;
	}
	e->data[0] = sdsnew("");

	e->syntax = (int **)malloc(e->len*sizeof(int*));
	if (e->syntax == NULL) {
		fprintf(stderr, "Editor: Could not create syntax array\n");
		return NULL;
	}

	return e;
}

int is_word(sds *data, char word[], int len, int x, int y) {
	for (int i = 0; i < len; i++) {
		if (data[y][x+i] != word[i]) {
			return 0;
		}
	}
	char leftChar = data[y][x-1];
	char rightChar = data[y][x+len];
	if (isalnum(leftChar) || isalnum(rightChar)) {
		return 0;	
	}
	return 1;
}
void color_word(int **syntax, int len, int x, int y, int color) {
	for (int i = 0; i < len; i++) {
		syntax[y][x+i] = color;
	}
}

// Worst thing i've wrote so far
void code_update_syntax(CodeEditor *e) {
	// Resize Syntax Array
	e->syntax = (int **)realloc(e->syntax, e->len*sizeof(int*));
	if (e->syntax == NULL) {
		fprintf(stderr, "Editor: Could not resize syntax array\n");
		return;
	}
	for (int i = 0; i < e->len; i++) {
		e->syntax[i] = (int *)malloc(sdslen(e->data[i])*sizeof(int));
		if (e->syntax[i] == NULL) {
			fprintf(stderr, "Editor: Could not resize syntax line %i\n", i);
			return;
		}
	}

	// Refill
	for (int i = 0; i < e->len; i++) {
		for (int j = 0; j < sdslen(e->data[i]); j++) {
			e->syntax[i][j] = 2;
		}
	}

	bool in_comment = false;
	bool multi_comment = false;
	char comment_char[3] = "\0";

	bool in_string = false;
	char string_char[2] = "\0";

	bool in_multi_string = false;

	for (int i = 0; i < e->len; i++) {
		for (int j = 0; j < sdslen(e->data[i]); j++) {

			// Comments
			if (e->data[i][j] == '-' && e->data[i][j+1] == '-' && !in_string) {
				in_comment = true;
				if (e->data[i][j+2] == '[' && e->data[i][j+3] == '[') {
					multi_comment = true;
				}
			}

			if (in_comment) {
				if (multi_comment) {
					if (e->data[i][j] == ']' && e->data[i][j+1] == ']') {
						e->syntax[i][j] = 1;
						e->syntax[i][j+1] = 1;
						in_comment = false;
						multi_comment = false;
						j+=2;
					}	
				} else {
					if (j == sdslen(e->data[i])-1) {
						e->syntax[i][j] = 1;
						in_comment = false;
						j++;
					}
				}
			}

			if (in_comment) {
				e->syntax[i][j] = 1;
				continue;
			}

			// Simple Strings
			if ((e->data[i][j] == '\"' || e->data[i][j] == '\'') && !in_string) {
				string_char[0] = e->data[i][j];
				in_string = true;
				e->syntax[i][j] = 9;
				continue;
			}
			if (in_string && string_char[0] == e->data[i][j]) {
				e->syntax[i][j] = 9;
				in_string = false;
				continue;
			}
			if (in_string) {
				e->syntax[i][j] = 9;
				continue;
			}
			
			// Multiline Strings
			if (e->data[i][j] == '[' && e->data[i][j+1] == '[') {
				in_multi_string = true;
			}
			if (e->data[i][j] == ']' && e->data[i][j+1] == ']') {
				e->syntax[i][j] = 9;
				e->syntax[i][j+1] = 9;
				in_multi_string = false;
				j+=2;
			}
			if (in_multi_string) {
				e->syntax[i][j] = 9;
				continue;
			}

			// Symbols
			if (ispunct(e->data[i][j]) && e->data[i][j] != '_') {
				e->syntax[i][j] = 3;
				continue;
			}

			// Numbers
			if (isdigit(e->data[i][j]) && !isalpha(e->data[i][j-1])) {
				if (!isdigit(e->data[i][j-1]) && e->syntax[i][j-1] != 4) {
					e->syntax[i][j] = 4;
					continue;
				} else if (e->syntax[i][j-1] == 4) {
					e->syntax[i][j] = 4;
					continue;
				}
			}

			// keywords
			for (int k = 0; k < 41; k++) {
				if (is_word(e->data, keywords[k], strlen(keywords[k]), j, i) && !in_string) {
					color_word(e->syntax, strlen(keywords[k]), j, i, k>22 ? 6 : 5);
					j+=strlen(keywords[k])-1;
				}
			}
		}
	}
}

void insert_new_line(CodeEditor *e) {
	e->len++;
	e->data = (sds *)realloc(e->data, e->len*sizeof(sds));
	if (e->data == NULL) {
		fprintf(stderr, "Could not realloc arr\n");
		exit(1);
	}
	e->row++;
	if (e->col < sdslen(e->data[e->row-1])) {
		sds tmp = sdsdup(e->data[e->row-1]);

		if (e->col != 0) {
			sdsrange(e->data[e->row-1], 0, e->col-1);
		} else {
			e->data[e->row-1] = sdsnew("");
		}
		sdsrange(tmp, e->col, -1);
		insert_element(e->data, e->row, e->len);
		e->data[e->row] = tmp;
	} else {
		insert_element(e->data, e->row, e->len);
		e->data[e->row] = sdsdup("");
	}
	e->col = 0;
}

void delete_char(CodeEditor *e) {
	sds tmp = sdsdup(e->data[e->row]);
	if (e->col == 1) {
		e->data[e->row] = sdsnew("");
	} else {
		sdsrange(e->data[e->row], 0, e->col-2);
	}
	sdsrange(tmp, e->col, -1);
	e->data[e->row] = sdscatsds(e->data[e->row], tmp);
	sdsfree(tmp);
	e->col--;
}

void delete_line(CodeEditor *e) {
	sds tmp = sdsdup(e->data[e->row]);
	e->row--;
	e->col = sdslen(e->data[e->row]);
	e->data[e->row] = sdscatsds(e->data[e->row], tmp);
	sdsfree(tmp);
	sdsfree(e->data[e->row+1]);
	remove_element(e->data, e->row+1, e->len);
	e->data = (sds *)realloc(e->data, (e->len-1) * sizeof(sds));
	if (e->data == NULL) {
		fprintf(stderr, "Could not remove element\n");
		exit(0);
	}
	e->len--;
}

void code_close(CodeEditor *e) {
	for (int i = 0; i < e->len; i++) {
		sdsfree(e->data[i]);
		free(e->syntax[i]);
	}
	free(e->data);
	free(e->syntax);
	free(e);
}
