
#include "utils.h"

Color color_from_palette(Ram *ram, int index) {
	return (Color){
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

int pos_in_rect(int rx, int ry, int rw, int rh, int x, int y) {
	return x >= rx && x <= rx+rw && y >= ry && y <= ry+rh;
}
int pos_in_rectex(Rectangle rect, int x, int y) {
	return x >= rect.x && x <= rect.x+rect.width && y >= rect.y && y <= rect.y+rect.height;
}

int min(int a, int b) {
	return a < b ? a : b;
}

int IsKeyPressedNRepeat(int key) {
	return (IsKeyPressed(key) || IsKeyPressedRepeat(key));
}

