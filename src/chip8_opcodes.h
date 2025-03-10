#ifndef OPCODES_H
#define OPCODES_H

#include "chip8_structure.h"

void opcodesInit(Chip8 *chip8_);
void opcodesEvaluate(uint16_t opcode);

#endif // OPCODES_H