#ifndef UTILS_H
#define UTILS_H

#include "ram.h"
#include "../include/tigr.h"
#include "../include/sds.h"

TPixel color_from_palette(Ram *ram, int index);
sds sdsinschar(sds str, int i, char ch);
void remove_element(sds *arr, int index, int arr_len);
void insert_element(sds *arr, int index, int arr_len);

#endif
