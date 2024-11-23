
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
