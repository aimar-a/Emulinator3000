#ifndef NES_INSTRUCTIONS_H
#define NES_INSTRUCTIONS_H

#include "nes_structure.h"
#include "nes_memory.h"
#include "nes_registers.h"
#include "nes_interrupts.h"
#include "nes_ppu.h"
#include "nes_apu.h"
#include "nes_controller.h"
#include "nes_mapper.h"
#include "nes_rom.h"
#include "nes_display.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <stdbool.h>
#include <stdint.h>
#include "nes_logger.h"

/**
 * Pushes a value onto the NES stack.
 *
 * @param nes A pointer to the NES system.
 * @param value The value to be pushed onto the stack.
 */
void nes_push(NES *nes, uint8_t value);
/**
 * Pushes an address onto the NES stack.
 *
 * @param nes A pointer to the NES system.
 * @param address The address to be pushed onto the stack.
 */
void nes_push_address(NES *nes, uint16_t address);
/**
 * Pulls a value from the NES stack.
 *
 * @param nes A pointer to the NES system.
 * @return The value pulled from the stack.
 */
uint8_t nes_pull(NES *nes);
/**
 * Pulls an address from the NES stack.
 *
 * @param nes A pointer to the NES system.
 * @return The address pulled from the stack.
 */
uint16_t nes_pull_address(NES *nes);

// --------------------------------------
// NES CPU Instructions
// --------------------------------------

/**
 * Adds the value at the given address to the accumulator with carry.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_adc(NES *nes, uint16_t address);

/**
 * Performs a bitwise AND between the accumulator and the value at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_and(NES *nes, uint16_t address);

/**
 * Shifts the value at the given address one bit to the left.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_asl(NES *nes, uint16_t address);

/**
 * Branches if the carry flag is clear.
 * @param nes Pointer to the NES structure.
 */
void nes_bcc(NES *nes);

/**
 * Branches if the carry flag is set.
 * @param nes Pointer to the NES structure.
 */
void nes_bcs(NES *nes);

/**
 * Branches if the zero flag is set.
 * @param nes Pointer to the NES structure.
 */
void nes_beq(NES *nes);

/**
 * Performs a bit test between the accumulator and the value at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_bit(NES *nes, uint16_t address);

/**
 * Branches if the negative flag is set.
 * @param nes Pointer to the NES structure.
 */
void nes_bmi(NES *nes);

/**
 * Branches if the zero flag is clear.
 * @param nes Pointer to the NES structure.
 */
void nes_bne(NES *nes);

/**
 * Branches if the negative flag is clear.
 * @param nes Pointer to the NES structure.
 */
void nes_bpl(NES *nes);

/**
 * Forces an interrupt.
 * @param nes Pointer to the NES structure.
 */
void nes_brk(NES *nes);

/**
 * Branches if the overflow flag is clear.
 * @param nes Pointer to the NES structure.
 */
void nes_bvc(NES *nes);

/**
 * Branches if the overflow flag is set.
 * @param nes Pointer to the NES structure.
 */
void nes_bvs(NES *nes);

/**
 * Clears the carry flag.
 * @param nes Pointer to the NES structure.
 */
void nes_clc(NES *nes);

/**
 * Clears the decimal mode flag.
 * @param nes Pointer to the NES structure.
 */
void nes_cld(NES *nes);

/**
 * Clears the interrupt disable flag.
 * @param nes Pointer to the NES structure.
 */
void nes_cli(NES *nes);

/**
 * Clears the overflow flag.
 * @param nes Pointer to the NES structure.
 */
void nes_clv(NES *nes);

/**
 * Compares the accumulator with the value at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_cmp(NES *nes, uint16_t address);

/**
 * Compares the X register with the value at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_cpx(NES *nes, uint16_t address);

/**
 * Compares the Y register with the value at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_cpy(NES *nes, uint16_t address);

/**
 * Decrements the value at the given address by one.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_dec(NES *nes, uint16_t address);

/**
 * Decrements the X register by one.
 * @param nes Pointer to the NES structure.
 */
void nes_dex(NES *nes);

/**
 * Decrements the Y register by one.
 * @param nes Pointer to the NES structure.
 */
void nes_dey(NES *nes);

/**
 * Performs a bitwise exclusive OR between the accumulator and the value at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_eor(NES *nes, uint16_t address);

/**
 * Increments the value at the given address by one.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_inc(NES *nes, uint16_t address);

/**
 * Increments the X register by one.
 * @param nes Pointer to the NES structure.
 */
void nes_inx(NES *nes);

/**
 * Increments the Y register by one.
 * @param nes Pointer to the NES structure.
 */
void nes_iny(NES *nes);

/**
 * Jumps to the address specified by the operand.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to jump to.
 */
void nes_jmp(NES *nes, uint16_t address);

/**
 * Jumps to the subroutine at the address specified by the operand.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to jump to.
 */
void nes_jsr(NES *nes, uint16_t address);

/**
 * Loads the accumulator with the value at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_lda(NES *nes, uint16_t address);

/**
 * Loads the X register with the value at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_ldx(NES *nes, uint16_t address);

/**
 * Loads the Y register with the value at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_ldy(NES *nes, uint16_t address);

/**
 * Shifts the value at the given address one bit to the right.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_lsr(NES *nes, uint16_t address);

/**
 * No operation.
 * @param nes Pointer to the NES structure.
 */
void nes_nop(NES *nes);

/**
 * Performs a bitwise OR between the accumulator and the value at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_ora(NES *nes, uint16_t address);

/**
 * Pushes the accumulator onto the stack.
 * @param nes Pointer to the NES structure.
 */
void nes_pha(NES *nes);

/**
 * Pushes the processor status onto the stack.
 * @param nes Pointer to the NES structure.
 */
void nes_php(NES *nes);

/**
 * Pulls the accumulator from the stack.
 * @param nes Pointer to the NES structure.
 */
void nes_pla(NES *nes);

/**
 * Pulls the processor status from the stack.
 * @param nes Pointer to the NES structure.
 */
void nes_plp(NES *nes);

/**
 * Rotates the value at the given address one bit to the left.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_rol(NES *nes, uint16_t address);

/**
 * Rotates the value at the given address one bit to the right.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_ror(NES *nes, uint16_t address);

/**
 * Returns from an interrupt.
 * @param nes Pointer to the NES structure.
 */
void nes_rti(NES *nes);

/**
 * Returns from a subroutine.
 * @param nes Pointer to the NES structure.
 */
void nes_rts(NES *nes);

/**
 * Subtracts the value at the given address from the accumulator with borrow.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to read the value from.
 */
void nes_sbc(NES *nes, uint16_t address);

/**
 * Sets the carry flag.
 * @param nes Pointer to the NES structure.
 */
void nes_sec(NES *nes);

/**
 * Sets the decimal mode flag.
 * @param nes Pointer to the NES structure.
 */
void nes_sed(NES *nes);

/**
 * Sets the interrupt disable flag.
 * @param nes Pointer to the NES structure.
 */
void nes_sei(NES *nes);

/**
 * Stores the accumulator at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to store the value.
 */
void nes_sta(NES *nes, uint16_t address);

/**
 * Stores the X register at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to store the value.
 */
void nes_stx(NES *nes, uint16_t address);

/**
 * Stores the Y register at the given address.
 * @param nes Pointer to the NES structure.
 * @param address Memory address to store the value.
 */
void nes_sty(NES *nes, uint16_t address);

/**
 * Transfers the accumulator to the X register.
 * @param nes Pointer to the NES structure.
 */
void nes_tax(NES *nes);

/**
 * Transfers the accumulator to the Y register.
 * @param nes Pointer to the NES structure.
 */
void nes_tay(NES *nes);

/**
 * Transfers the stack pointer to the X register.
 * @param nes Pointer to the NES structure.
 */
void nes_tsx(NES *nes);

/**
 * Transfers the X register to the accumulator.
 * @param nes Pointer to the NES structure.
 */
void nes_txa(NES *nes);

/**
 * Transfers the X register to the stack pointer.
 * @param nes Pointer to the NES structure.
 */
void nes_txs(NES *nes);

/**
 * Transfers the Y register to the accumulator.
 * @param nes Pointer to the NES structure.
 */
void nes_tya(NES *nes);

/**
 * Shifts the accumulator one bit to the left.
 * @param nes Pointer to the NES structure.
 */
void nes_asl_a(NES *nes);

/**
 * Rotates the accumulator one bit to the left.
 * @param nes Pointer to the NES structure.
 */
void nes_rol_a(NES *nes);

/**
 * Rotates the accumulator one bit to the right.
 * @param nes Pointer to the NES structure.
 */
void nes_ror_a(NES *nes);

/**
 * Jumps to the address specified by the indirect operand.
 * @param nes Pointer to the NES structure.
 */
void nes_jmp_indirect(NES *nes);

/**
 * Shifts the accumulator one bit to the right.
 * @param nes Pointer to the NES structure.
 */
void nes_lsr_a(NES *nes);

#endif
