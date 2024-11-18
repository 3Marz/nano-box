
#include "utils.h"

TPixel color_from_palette(Ram *ram, int index) {
	return (TPixel){
		Peek(ram, 0x3000+(index*3)),
		Peek(ram, 0x3000+(index*3)+1),
		Peek(ram, 0x3000+(index*3)+2),
		255};
}

