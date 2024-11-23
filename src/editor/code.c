
#include "code.h"
#include "../utils.h"
#include <stdio.h>
#include <stdlib.h>

CodeEditor* code_init() {
	CodeEditor *e = malloc(sizeof(CodeEditor));
	e->yoff = 9;
	e->col = 0;
	e->row = 0;
	e->len = 1;
	e->data = (sds *)malloc(e->len*sizeof(sds));
	if (e->data == NULL) {
		fprintf(stderr, "Editor: Could not create code editor\n");
		return NULL;
	}
	e->data[0] = sdsnew("");

	return e;
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
	}
	free(e->data);
	free(e);
}
