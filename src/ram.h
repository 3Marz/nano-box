#ifndef RAM_H
#define RAM_H

#include <stdint.h>

#define RAM_SIZE 0xFFFF

#define RAM_PALETTE_START  0x3000
#define RAM_FONT_START     0x3030
#define RAM_SPRITES_START  0x326A
#define RAM_TILES_START    0x48EA
#define RAM_MOUSE_START    0x5F6A
#define RAM_KEYBOARD_START 0x5F6D
#define RAM_BUTTONS_START  0x5F6E

typedef struct Ram {
	uint8_t data[RAM_SIZE]; 
} Ram;

void ram_init(Ram *ram);
void ram_reset(Ram *ram);

// Ram Operations
uint8_t Peek (Ram *ram, uint16_t address);
uint8_t Peek2(Ram *ram, uint16_t address);
uint8_t Peek4(Ram *ram, uint16_t address);

void Poke (Ram *ram, uint16_t address, uint8_t value);
void Poke2(Ram *ram, uint16_t address, uint16_t value);
/*void poke4(Ram *ram, uint16_t address, uint32_t value);*/

#endif
