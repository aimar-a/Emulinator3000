#ifndef NES_OPCODES_H
#define NES_OPCODES_H

#include "nes_structure.h"
#include "nes_instructions.h"
#include "nes_addressing.h"
#include "nes_memory.h"
#include <stdio.h>
#include "nes_logger.h"

// Evaluates the current opcode and returns the cycles used
uint8_t nes_evaluate_opcode(NES *nes);

#endif