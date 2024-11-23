
#include "utils.h"

TPixel color_from_palette(Ram *ram, int index) {
	return (TPixel){
		Peek(ram, 0x3000+(index*3)),
		Peek(ram, 0x3000+(index*3)+1),
		Peek(ram, 0x3000+(index*3)+2),
		255};
}

sds sdsinschar(sds str, int i, char ch) {
	sds temp = sdsdup(str);
	if (i != -1) {
		sdsrange(str, 0, i);
	} else {
		str = sdsnew("");
	}

	char s[2] = "\0";
	s[0] = ch;
	str = sdscat(str, s);

	sdsrange(temp, i+1, -1);
	str = sdscatsds(str, temp);
	sdsfree(temp);

	return str;
}

void remove_element(sds *arr, int index, int arr_len) {
	int i;
	for(i = index; i < arr_len - 1; i++) arr[i] = arr[i + 1];
}
void insert_element(sds *arr, int index, int arr_len) {
	int i;
	for (i = arr_len-1; i > index; i--) arr[i] = arr[i-1];
}
