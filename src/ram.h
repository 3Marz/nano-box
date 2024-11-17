#ifndef RAM_H
#define RAM_H

#include <stdint.h>

typedef struct Ram {
	uint8_t data[0xFFFF]; 
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
