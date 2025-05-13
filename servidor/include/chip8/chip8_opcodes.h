#ifndef OPCODES_H
#define OPCODES_H

#include "chip8_structure.h"
#include "chip8_display.h"
#include "superchip8bool.h"
#include <stdio.h>
#include <stdint.h>
#include "chip8_logger.h"

void chip8opcodesInit(Chip8 *chip8_);
void chip8opcodesEvaluate(uint16_t opcode);

#endif // OPCODES_H