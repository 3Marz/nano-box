
#include "ram.h"
#include "globals.h"
#include <stdint.h>
#include <stdio.h>

const int fontMem[] = {
    0x0,  0x0,  0x0,  0x0,  0x0,  0x0, // " "
    0x4,  0x4,  0x4,  0x0,  0x4,  0x0, // "!"
    0xa,  0xa,  0x0,  0x0,  0x0,  0x0, // """
    0xa,  0x1e, 0xa,  0x1e, 0xa,  0x0, // "#"
    0xe,  0xc,  0x6,  0xe,  0x4,  0x0, // "$"
    0x12, 0x6,  0xc,  0x18, 0x12, 0x0, // "%"
    0xc,  0x14, 0xc,  0x12, 0x1c, 0x0, // "&"
    0x4,  0x8,  0x0,  0x0,  0x0,  0x0, // "'"
    0x6,  0xc,  0x8,  0xc,  0x6,  0x0, // "("
    0xc,  0x6,  0x2,  0x6,  0xc,  0x0, // ")"
    0xa,  0x4,  0xe,  0x4,  0xa,  0x0, // "*"
    0x0,  0x4,  0xe,  0x4,  0x0,  0x0, // "+"
    0x0,  0x0,  0x0,  0x4,  0x8,  0x0, // ","
    0x0,  0x0,  0xe,  0x0,  0x0,  0x0, // "-"
    0x0,  0x0,  0x0,  0x0,  0x4,  0x0, // "."
    0x2,  0x6,  0xc,  0x18, 0x10, 0x0, // "/"
    0xc,  0x16, 0x1a, 0x12, 0xc,  0x0, // "0"
    0xc,  0x1c, 0xc,  0xc,  0x1e, 0x0, // "1"
    0x1e, 0x2,  0x1e, 0x18, 0x1e, 0x0, // "2"
    0x1e, 0x2,  0xe,  0x6,  0x1e, 0x0, // "3"
    0x12, 0x1a, 0x1e, 0x2,  0x2,  0x0, // "4"
    0x1e, 0x10, 0x1e, 0x6,  0x1e, 0x0, // "5"
    0x1c, 0x10, 0x1e, 0x1a, 0x1e, 0x0, // "6"
    0x1e, 0x2,  0x6,  0xc,  0xc,  0x0, // "7"
    0xc,  0x1a, 0xc,  0x1a, 0xc,  0x0, // "8"
    0x1e, 0x16, 0x1e, 0x6,  0x1e, 0x0, // "9"
    0x0,  0x4,  0x0,  0x4,  0x0,  0x0, // ":"
    0x0,  0x4,  0x0,  0x4,  0x8,  0x0, // ";"
    0x2,  0x4,  0x8,  0x4,  0x2,  0x0, // "<"
    0x0,  0xe,  0x0,  0xe,  0x0,  0x0, // "="
    0x8,  0x4,  0x2,  0x4,  0x8,  0x0, // ">"
    0xe,  0x2,  0x6,  0x0,  0x4,  0x0, // "?"
    0x1e, 0x12, 0x16, 0x10, 0xe,  0x0, // "@"
    0xc,  0x1a, 0x1a, 0x1e, 0x12, 0x0, // "A"
    0x1c, 0x1a, 0x1c, 0x1a, 0x1c, 0x0, // "B"
    0xc,  0x1a, 0x18, 0x1a, 0xc,  0x0, // "C"
    0x1c, 0x1a, 0x1a, 0x1a, 0x1c, 0x0, // "D"
    0x1e, 0x18, 0x1c, 0x18, 0x1e, 0x0, // "E"
    0x1e, 0x18, 0x1c, 0x18, 0x18, 0x0, // "F"
    0xe,  0x18, 0x1a, 0x1a, 0xe,  0x0, // "G"
    0x1a, 0x1a, 0x1e, 0x1a, 0x1a, 0x0, // "H"
    0x1e, 0xc,  0xc,  0xc,  0x1e, 0x0, // "I"
    0xe,  0x6,  0x6,  0x16, 0x1e, 0x0, // "J"
    0x12, 0x16, 0x1c, 0x1a, 0x1a, 0x0, // "K"
    0x18, 0x18, 0x18, 0x18, 0x1e, 0x0, // "L"
    0x1e, 0x1e, 0x1a, 0x12, 0x12, 0x0, // "M"
    0x12, 0x1a, 0x1e, 0x16, 0x12, 0x0, // "N"
    0xc,  0x1a, 0x1a, 0x1a, 0xc,  0x0, // "O"
    0x1e, 0x1a, 0x1a, 0x1e, 0x18, 0x0, // "P"
    0xc,  0x1a, 0x12, 0x14, 0xa,  0x0, // "Q"
    0x1e, 0x1a, 0x1a, 0x1c, 0x1a, 0x0, // "R"
    0xe,  0x18, 0x1e, 0x6,  0x1c, 0x0, // "S"
    0x1e, 0x1e, 0xc,  0xc,  0xc,  0x0, // "T"
    0x1a, 0x1a, 0x1a, 0x1a, 0xc,  0x0, // "U"
    0x16, 0x16, 0x16, 0x1c, 0x8,  0x0, // "V"
    0x12, 0x12, 0x16, 0x1e, 0x1e, 0x0, // "W"
    0x1a, 0x1a, 0xc,  0x1a, 0x1a, 0x0, // "X"
    0x16, 0x16, 0x1e, 0xc,  0xc,  0x0, // "Y"
    0x1e, 0x6,  0xc,  0x18, 0x1e, 0x0, // "Z"
    0xe,  0xc,  0xc,  0xc,  0xe,  0x0, // "["
    0x10, 0x18, 0xc,  0x6,  0x2,  0x0, // "\"
    0xe,  0x6,  0x6,  0x6,  0xe,  0x0, // "]"
    0x4,  0xe,  0xa,  0x0,  0x0,  0x0, // "^"
    0x0,  0x0,  0x0,  0x0,  0xe,  0x0, // "_"
    0x8,  0x4,  0x0,  0x0,  0x0,  0x0, // "`"
    0x0,  0xe,  0x1a, 0x1a, 0xe,  0x0, // "a"
    0x10, 0x1c, 0x1a, 0x1a, 0x1c, 0x0, // "b"
    0x0,  0xe,  0x18, 0x18, 0xe,  0x0, // "c"
    0x2,  0xe,  0x16, 0x16, 0xe,  0x0, // "d"
    0x0,  0xc,  0x1e, 0x10, 0xc,  0x0, // "e"
    0x6,  0x8,  0x1e, 0x8,  0x8,  0x0, // "f"
    0x0,  0x1c, 0x1c, 0x4,  0x1c, 0x0, // "g"
    0x10, 0x10, 0x1c, 0x1a, 0x1a, 0x0, // "h"
    0xc,  0x0,  0xc,  0xc,  0xc,  0x0, // "i"
    0x6,  0x0,  0x6,  0x6,  0xe,  0x0, // "j"
    0x0,  0x12, 0x14, 0x18, 0x16, 0x0, // "k"
    0x0,  0x18, 0x18, 0x18, 0xe,  0x0, // "l"
    0x0,  0x1e, 0x1e, 0x12, 0x12, 0x0, // "m"
    0x0,  0x1c, 0x1a, 0x1a, 0x1a, 0x0, // "n"
    0x0,  0xc,  0x1a, 0x1a, 0xc,  0x0, // "o"
    0x0,  0x1e, 0x1a, 0x1e, 0x18, 0x0, // "p"
    0x0,  0x1e, 0x16, 0x1e, 0x6,  0x0, // "q"
    0x0,  0x16, 0x18, 0x18, 0x18, 0x0, // "r"
    0x0,  0xe,  0x18, 0xe,  0x1c, 0x0, // "s"
    0x0,  0xc,  0x1e, 0xc,  0xe,  0x0, // "t"
    0x0,  0x1a, 0x1a, 0x1a, 0xc,  0x0, // "u"
    0x0,  0x12, 0x16, 0x1c, 0x8,  0x0, // "v"
    0x0,  0x12, 0x12, 0x1e, 0x1e, 0x0, // "w"
    0x0,  0x1a, 0xc,  0x1a, 0x1a, 0x0, // "x"
    0x0,  0x16, 0x1e, 0x6,  0x1e, 0x0, // "y"
    0x0,  0x1e, 0x6,  0x18, 0x1e, 0x0, // "z"
    0x6,  0x4,  0xc,  0x4,  0x6,  0x0, // "{"
    0x4,  0x4,  0x4,  0x4,  0x4,  0x0, // "|"
    0xc,  0x4,  0x6,  0x4,  0xc,  0x0, // "}"
    0x0,  0xa,  0x14, 0x0,  0x0,  0x0, // "~"
};

const int palette[] = {
    0x0a, 0x08, 0x0d, // Black
    0x69, 0x75, 0x94, // Gray
    0xdf, 0xe9, 0xf5, // White
    0xf7, 0xaa, 0xa8, // Skin ?
    0xd4, 0x68, 0x9a, // Pink
    0x78, 0x2c, 0x96, // Purple
    0xe8, 0x35, 0x62, // Red
    0xf2, 0x82, 0x5c, // Orange
    0xff, 0xc7, 0x6e, // Yellow
    0x88, 0xc4, 0x4d, // Green
    0x3f, 0x9e, 0x59, // Dark Green
    0x37, 0x34, 0x61, // Deep Blue ?
    0x48, 0x54, 0xa8, // Dark Blue
    0x71, 0x99, 0xd9, // Light Blue
    0x9e, 0x52, 0x52, // Brown
    0x4d, 0x25, 0x36, // Dark Brown
};

uint8_t Peek(Ram *ram, uint16_t address) {
	if (address > 0xFFFF || address < 0) {
		return 0;
	}

	return ram->data[address];
}
uint8_t Peek2(Ram *ram, uint16_t address) {
	if (address > 0xFFFE || address < 0) {
		return 0;
	}

	return ram->data[address]<<8 | ram->data[address+1];
}
uint8_t Peek4(Ram *ram, uint16_t address) {
	if (address > 0xFFFC || address < 0) {
		return 0;
	}

	return 
		ram->data[address]<<24 | 
		ram->data[address+1]<<16 | 
		ram->data[address+2]<<8 | 
		ram->data[address+3]; 
}

void Poke(Ram *ram, uint16_t address, uint8_t value) {
	if (address > 0xFFFF || address < 0) {
		return;
	}

	ram->data[address] = value;
}
void Poke2(Ram *ram, uint16_t address, uint16_t value) {
	if (address > 0xFFFE || address < 0) {
		return;
	}

	ram->data[address] = value >> 8;
	ram->data[address+1] = value & 0xFF;
}
/*void poke4(Ram *ram, uint16_t address, uint32_t value) {*/
/*	if (address > 0xFFFC || address < 0) {*/
/*		return;*/
/*	}*/
/**/
/*	ram->data[address] = value >> 24;*/
/*	ram->data[address+1] = (value >> 16) & 0xFF;*/
/*	ram->data[address+2] = (value >> 8) & 0xFF;*/
/*	ram->data[address+3] = value & 0xFF;*/
/*}*/

void ram_reset(Ram *ram) {
	for(int i = 0; i < 0xFFFF; i++) {
		ram->data[i] = 0;
	}
}

void ram_init(Ram *ram) {
	ram_reset(ram);

	//Test fill screen buffer
	/*for(int i = 0; i < (SCREEN_WIDTH*SCREEN_HEIGHT)/2; i++) {*/
	/*	if(i > (SCREEN_WIDTH*SCREEN_HEIGHT)/4) {*/
	/*		ram->data[i] = 0x1e;*/
	/*	} else {*/
	/*		ram->data[i] = 0x30;*/
	/*	}*/
	/*}*/

	//Load Palette
	for(int i = 0x3000; i < 0x3030; i++) {
		ram->data[i] = palette[i - 0x3000];
	}
	
	//Load Font
	for(int i = 0x3030; i < 0x326A; i++) {
		ram->data[i] = fontMem[i - 0x3030];
	}
}


