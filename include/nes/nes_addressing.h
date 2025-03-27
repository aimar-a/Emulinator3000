#ifndef NES_ADDRESSING_H
#define NES_ADDRESSING_H

#include <stdint.h>
#include "nes_structure.h"
#include "nes_memory.h"
#include "nes_logger.h"

// Comentarios patrocinados por copilot

/**
 * Reads an immediate address from the program counter (PC) and increments the PC.
 *
 * @param nes Pointer to the NES structure.
 * @return The immediate address read.
 */
uint16_t nes_immediate(NES *nes);

/**
 * Reads an absolute address from the program counter (PC) and increments the PC by 2.
 *
 * @param nes Pointer to the NES structure.
 * @return The absolute address read.
 */
uint16_t nes_absolute(NES *nes);

/**
 * Reads an absolute address from the program counter (PC), adds the X register to it, and increments the PC by 2.
 *
 * @param nes Pointer to the NES structure.
 * @return The absolute address plus X register.
 */
uint16_t nes_absolute_x(NES *nes);

/**
 * Reads an absolute address from the program counter (PC), adds the Y register to it, and increments the PC by 2.
 *
 * @param nes Pointer to the NES structure.
 * @return The absolute address plus Y register.
 */
uint16_t nes_absolute_y(NES *nes);

/**
 * Reads a zero page address from the program counter (PC) and increments the PC.
 *
 * @param nes Pointer to the NES structure.
 * @return The zero page address read.
 */
uint8_t nes_zero_page(NES *nes);

/**
 * Reads a zero page address from the program counter (PC), adds the X register to it, and increments the PC.
 *
 * @param nes Pointer to the NES structure.
 * @return The zero page address plus X register, wrapped around to fit in zero page.
 */
uint8_t nes_zero_page_x(NES *nes);

/**
 * Reads a zero page address from the program counter (PC), adds the Y register to it, and increments the PC.
 *
 * @param nes Pointer to the NES structure.
 * @return The zero page address plus Y register, wrapped around to fit in zero page.
 */
uint8_t nes_zero_page_y(NES *nes);

/**
 * Reads a zero page address from the program counter (PC), uses the result as a pointer to read a 16-bit address, and increments the PC.
 *
 * @param nes Pointer to the NES structure.
 * @return The 16-bit address read from the zero page address.
 */
uint16_t nes_indirect(NES *nes);

/**
 * Reads a zero page address from the program counter (PC), adds the X register to it, and uses the result as a pointer to read a 16-bit address.
 * Increments the PC.
 *
 * @param nes Pointer to the NES structure.
 * @return The 16-bit address read from the zero page address plus X register.
 */
uint16_t nes_indirect_x(NES *nes);

/**
 * Reads a zero page address from the program counter (PC), uses the result as a pointer to read a 16-bit address, and adds the Y register to it.
 * Increments the PC.
 *
 * @param nes Pointer to the NES structure.
 * @return The 16-bit address read from the zero page address plus Y register.
 */
uint16_t nes_indirect_y(NES *nes);
#endif