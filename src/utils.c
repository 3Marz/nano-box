
#include "utils.h"

Color color_from_palette(Ram *ram, int index) {
	return (Color){
		Peek(ram, 0x3000+(index*3)),
		Peek(ram, 0x3000+(index*3)+1),
		Peek(ram, 0x3000+(index*3)+2),
		255};
}

