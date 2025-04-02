#include "nes_opcodes.h"

// Mapping of opcodes to instruction names
const char *opcode_names[256] = {
    [0x00] = "BRK (Force Interrupt)", [0x01] = "ORA (Logical Inclusive OR, Indirect,X)", [0x05] = "ORA (Logical Inclusive OR, Zero Page)", [0x06] = "ASL (Arithmetic Shift Left, Zero Page)", [0x08] = "PHP (Push Processor Status)", [0x09] = "ORA (Logical Inclusive OR, Immediate)", [0x0A] = "ASL (Arithmetic Shift Left, Accumulator)", [0x0D] = "ORA (Logical Inclusive OR, Absolute)", [0x0E] = "ASL (Arithmetic Shift Left, Absolute)", [0x10] = "BPL (Branch if Positive)", [0x11] = "ORA (Logical Inclusive OR, Indirect,Y)", [0x15] = "ORA (Logical Inclusive OR, Zero Page,X)", [0x16] = "ASL (Arithmetic Shift Left, Zero Page,X)", [0x18] = "CLC (Clear Carry Flag)", [0x19] = "ORA (Logical Inclusive OR, Absolute,Y)", [0x1D] = "ORA (Logical Inclusive OR, Absolute,X)", [0x1E] = "ASL (Arithmetic Shift Left, Absolute,X)", [0x20] = "JSR (Jump to Subroutine)", [0x21] = "AND (Logical AND, Indirect,X)", [0x24] = "BIT (Test Bits, Zero Page)", [0x25] = "AND (Logical AND, Zero Page)", [0x26] = "ROL (Rotate Left, Zero Page)", [0x28] = "PLP (Pull Processor Status)", [0x29] = "AND (Logical AND, Immediate)", [0x2A] = "ROL (Rotate Left, Accumulator)", [0x2C] = "BIT (Test Bits, Absolute)", [0x2D] = "AND (Logical AND, Absolute)", [0x2E] = "ROL (Rotate Left, Absolute)", [0x30] = "BMI (Branch if Minus)", [0x31] = "AND (Logical AND, Indirect,Y)", [0x35] = "AND (Logical AND, Zero Page,X)", [0x36] = "ROL (Rotate Left, Zero Page,X)", [0x38] = "SEC (Set Carry Flag)", [0x39] = "AND (Logical AND, Absolute,Y)", [0x3D] = "AND (Logical AND, Absolute,X)", [0x3E] = "ROL (Rotate Left, Absolute,X)", [0x40] = "RTI (Return from Interrupt)", [0x41] = "EOR (Exclusive OR, Indirect,X)", [0x45] = "EOR (Exclusive OR, Zero Page)", [0x46] = "LSR (Logical Shift Right, Zero Page)", [0x48] = "PHA (Push Accumulator)", [0x49] = "EOR (Exclusive OR, Immediate)", [0x4A] = "LSR (Logical Shift Right, Accumulator)", [0x4C] = "JMP (Jump, Absolute)", [0x4D] = "EOR (Exclusive OR, Absolute)", [0x4E] = "LSR (Logical Shift Right, Absolute)", [0x50] = "BVC (Branch if Overflow Clear)", [0x51] = "EOR (Exclusive OR, Indirect,Y)", [0x55] = "EOR (Exclusive OR, Zero Page,X)", [0x56] = "LSR (Logical Shift Right, Zero Page,X)", [0x58] = "CLI (Clear Interrupt Disable)", [0x59] = "EOR (Exclusive OR, Absolute,Y)", [0x5D] = "EOR (Exclusive OR, Absolute,X)", [0x5E] = "LSR (Logical Shift Right, Absolute,X)", [0x60] = "RTS (Return from Subroutine)", [0x61] = "ADC (Add with Carry, Indirect,X)", [0x65] = "ADC (Add with Carry, Zero Page)", [0x66] = "ROR (Rotate Right, Zero Page)", [0x68] = "PLA (Pull Accumulator)", [0x69] = "ADC (Add with Carry, Immediate)", [0x6A] = "ROR (Rotate Right, Accumulator)", [0x6C] = "JMP (Jump, Indirect)", [0x6D] = "ADC (Add with Carry, Absolute)", [0x6E] = "ROR (Rotate Right, Absolute)", [0x70] = "BVS (Branch if Overflow Set)", [0x71] = "ADC (Add with Carry, Indirect,Y)", [0x75] = "ADC (Add with Carry, Zero Page,X)", [0x76] = "ROR (Rotate Right, Zero Page,X)", [0x78] = "SEI (Set Interrupt Disable)", [0x79] = "ADC (Add with Carry, Absolute,Y)", [0x7D] = "ADC (Add with Carry, Absolute,X)", [0x7E] = "ROR (Rotate Right, Absolute,X)", [0x81] = "STA (Store Accumulator, Indirect,X)", [0x84] = "STY (Store Y Register, Zero Page)", [0x85] = "STA (Store Accumulator, Zero Page)", [0x86] = "STX (Store X Register, Zero Page)", [0x88] = "DEY (Decrement Y Register)", [0x8A] = "TXA (Transfer X to Accumulator)", [0x8C] = "STY (Store Y Register, Absolute)", [0x8D] = "STA (Store Accumulator, Absolute)", [0x8E] = "STX (Store X Register, Absolute)", [0x90] = "BCC (Branch if Carry Clear)", [0x91] = "STA (Store Accumulator, Indirect,Y)", [0x94] = "STY (Store Y Register, Zero Page,X)", [0x95] = "STA (Store Accumulator, Zero Page,X)", [0x96] = "STX (Store X Register, Zero Page,Y)", [0x98] = "TYA (Transfer Y to Accumulator)", [0x99] = "STA (Store Accumulator, Absolute,Y)", [0x9A] = "TXS (Transfer X to Stack Pointer)", [0x9D] = "STA (Store Accumulator, Absolute,X)", [0xA0] = "LDY (Load Y Register, Immediate)", [0xA1] = "LDA (Load Accumulator, Indirect,X)", [0xA2] = "LDX (Load X Register, Immediate)", [0xA4] = "LDY (Load Y Register, Zero Page)", [0xA5] = "LDA (Load Accumulator, Zero Page)", [0xA6] = "LDX (Load X Register, Zero Page)", [0xA8] = "TAY (Transfer Accumulator to Y)", [0xA9] = "LDA (Load Accumulator, Immediate)", [0xAA] = "TAX (Transfer Accumulator to X)", [0xAC] = "LDY (Load Y Register, Absolute)", [0xAD] = "LDA (Load Accumulator, Absolute)", [0xAE] = "LDX (Load X Register, Absolute)", [0xB0] = "BCS (Branch if Carry Set)", [0xB1] = "LDA (Load Accumulator, Indirect,Y)", [0xB4] = "LDY (Load Y Register, Zero Page,X)", [0xB5] = "LDA (Load Accumulator, Zero Page,X)", [0xB6] = "LDX (Load X Register, Zero Page,Y)", [0xB8] = "CLV (Clear Overflow Flag)", [0xB9] = "LDA (Load Accumulator, Absolute,Y)", [0xBA] = "TSX (Transfer Stack Pointer to X)", [0xBC] = "LDY (Load Y Register, Absolute,X)", [0xBD] = "LDA (Load Accumulator, Absolute,X)", [0xBE] = "LDX (Load X Register, Absolute,Y)", [0xC0] = "CPY (Compare Y Register, Immediate)", [0xC1] = "CMP (Compare Accumulator, Indirect,X)", [0xC4] = "CPY (Compare Y Register, Zero Page)", [0xC5] = "CMP (Compare Accumulator, Zero Page)", [0xC6] = "DEC (Decrement Memory, Zero Page)", [0xC8] = "INY (Increment Y Register)", [0xC9] = "CMP (Compare Accumulator, Immediate)", [0xCA] = "DEX (Decrement X Register)", [0xCC] = "CPY (Compare Y Register, Absolute)", [0xCD] = "CMP (Compare Accumulator, Absolute)", [0xCE] = "DEC (Decrement Memory, Absolute)", [0xD0] = "BNE (Branch if Not Equal)", [0xD1] = "CMP (Compare Accumulator, Indirect,Y)", [0xD5] = "CMP (Compare Accumulator, Zero Page,X)", [0xD6] = "DEC (Decrement Memory, Zero Page,X)", [0xD8] = "CLD (Clear Decimal Mode)", [0xD9] = "CMP (Compare Accumulator, Absolute,Y)", [0xDD] = "CMP (Compare Accumulator, Absolute,X)", [0xDE] = "DEC (Decrement Memory, Absolute,X)", [0xE0] = "CPX (Compare X Register, Immediate)", [0xE1] = "SBC (Subtract with Carry, Indirect,X)", [0xE4] = "CPX (Compare X Register, Zero Page)", [0xE5] = "SBC (Subtract with Carry, Zero Page)", [0xE6] = "INC (Increment Memory, Zero Page)", [0xE8] = "INX (Increment X Register)", [0xE9] = "SBC (Subtract with Carry, Immediate)", [0xEA] = "NOP (No Operation)", [0xEC] = "CPX (Compare X Register, Absolute)", [0xED] = "SBC (Subtract with Carry, Absolute)", [0xEE] = "INC (Increment Memory, Absolute)", [0xF0] = "BEQ (Branch if Equal)", [0xF1] = "SBC (Subtract with Carry, Indirect,Y)", [0xF5] = "SBC (Subtract with Carry, Zero Page,X)", [0xF6] = "INC (Increment Memory, Zero Page,X)", [0xF8] = "SED (Set Decimal Mode)", [0xF9] = "SBC (Subtract with Carry, Absolute,Y)", [0xFD] = "SBC (Subtract with Carry, Absolute,X)", [0xFE] = "INC (Increment Memory, Absolute,X)"};
uint8_t nes_evaluate_opcode(NES *nes)
{
  uint8_t opcode = nes_read(nes, nes->PC);
  nes->PC++;

  nes_log_traceback("INFO: Registers: A: 0x%02X, X: 0x%02X, Y: 0x%02X, P: 0x%02X, SP: 0x%02X\n",
                    nes->A, nes->X, nes->Y, nes->P, nes->SP);
  // printf("Registers: A: 0x%02X, X: 0x%02X, Y: 0x%02X, P: 0x%02X, SP: 0x%02X\n",
  //        nes->A, nes->X, nes->Y, nes->P, nes->SP);

  const char *instruction_name = opcode_names[opcode] ? opcode_names[opcode] : "ERROR, UNKNOWN INSTRUCTION";
  nes_log_traceback("INFO: PC: 0x%04X, Opcode: 0x%02X (0x%02X 0x%02X), Instruction: %s\n",
                    nes->PC - 1, opcode, nes_read(nes, nes->PC), nes_read(nes, nes->PC + 1), instruction_name);
  // printf("INFO: PC: 0x%04X, Opcode: 0x%02X (0x%02X 0x%02X), Instruction: %s\n",
  //        nes->PC - 1, opcode, nes_read(nes, nes->PC), nes_read(nes, nes->PC + 1), instruction_name);

  switch (opcode)
  {
  case 0x00:
    nes_log_error("ERROR: BRK instruction not implemented\n");
    exit(1);
    nes_brk(nes);
    return 7;
  case 0x01:
    nes_ora(nes, nes_indirect_x(nes));
    return 6;
  case 0x05:
    nes_ora(nes, nes_zero_page(nes));
    return 3;
  case 0x06:
    nes_asl(nes, nes_zero_page(nes));
    return 5;
  case 0x08:
    nes_php(nes);
    return 3;
  case 0x09:
    nes_ora(nes, nes_immediate(nes));
    return 2;
  case 0x0A:
    nes_asl_a(nes);
    return 2;
  case 0x0D:
    nes_ora(nes, nes_absolute(nes));
    return 4;
  case 0x0E:
    nes_asl(nes, nes_absolute(nes));
    return 6;
  case 0x10:
    nes_bpl(nes);
    return 2; // +1 if branch succeeds, +2 if it crosses a page boundary
  case 0x11:
    nes_ora(nes, nes_indirect_y(nes));
    return 5; // +1 if it crosses a page boundary
  case 0x15:
    nes_ora(nes, nes_zero_page_x(nes));
    return 4;
  case 0x16:
    nes_asl(nes, nes_zero_page_x(nes));
    return 6;
  case 0x18:
    nes_clc(nes);
    return 2;
  case 0x19:
    nes_ora(nes, nes_absolute_y(nes));
    return 4; // +1 if it crosses a page boundary
  case 0x1D:
    nes_ora(nes, nes_absolute_x(nes));
    return 4; // +1 if it crosses a page boundary
  case 0x1E:
    nes_asl(nes, nes_absolute_x(nes));
    return 7;
  case 0x20:
    nes_jsr(nes, nes_absolute(nes));
    return 6;
  case 0x21:
    nes_and(nes, nes_indirect_x(nes));
    return 6;
  case 0x24:
    nes_bit(nes, nes_zero_page(nes));
    return 3;
  case 0x25:
    nes_and(nes, nes_zero_page(nes));
    return 3;
  case 0x26:
    nes_rol(nes, nes_zero_page(nes));
    return 5;
  case 0x28:
    nes_plp(nes);
    return 4;
  case 0x29:
    nes_and(nes, nes_immediate(nes));
    return 2;
  case 0x2A:
    nes_rol_a(nes);
    return 2;
  case 0x2C:
    nes_bit(nes, nes_absolute(nes));
    return 4;
  case 0x2D:
    nes_and(nes, nes_absolute(nes));
    return 4;
  case 0x2E:
    nes_rol(nes, nes_absolute(nes));
    return 6;
  case 0x30:
    nes_bmi(nes);
    return 2; // +1 if branch succeeds, +2 if it crosses a page boundary
  case 0x31:
    nes_and(nes, nes_indirect_y(nes));
    return 5; // +1 if it crosses a page boundary
  case 0x35:
    nes_and(nes, nes_zero_page_x(nes));
    return 4;
  case 0x36:
    nes_rol(nes, nes_zero_page_x(nes));
    return 6;
  case 0x38:
    nes_sec(nes);
    return 2;
  case 0x39:
    nes_and(nes, nes_absolute_y(nes));
    return 4; // +1 if it crosses a page boundary
  case 0x3D:
    nes_and(nes, nes_absolute_x(nes));
    return 4; // +1 if it crosses a page boundary
  case 0x3E:
    nes_rol(nes, nes_absolute_x(nes));
    return 7;
  case 0x40:
    nes_rti(nes);
    return 6;
  case 0x41:
    nes_eor(nes, nes_indirect_x(nes));
    return 6;
  case 0x45:
    nes_eor(nes, nes_zero_page(nes));
    return 3;
  case 0x46:
    nes_lsr(nes, nes_zero_page(nes));
    return 5;
  case 0x48:
    nes_pha(nes);
    return 3;
  case 0x49:
    nes_eor(nes, nes_immediate(nes));
    return 2;
  case 0x4A:
    nes_lsr_a(nes);
    return 2;
  case 0x4C:
    nes_jmp(nes, nes_absolute(nes));
    return 3;
  case 0x4D:
    nes_eor(nes, nes_absolute(nes));
    return 4;
  case 0x4E:
    nes_lsr(nes, nes_absolute(nes));
    return 6;
  case 0x50:
    nes_bvc(nes);
    return 2; // +1 if branch succeeds, +2 if it crosses a page boundary
  case 0x51:
    nes_eor(nes, nes_indirect_y(nes));
    return 5; // +1 if it crosses a page boundary
  case 0x55:
    nes_eor(nes, nes_zero_page_x(nes));
    return 4;
  case 0x56:
    nes_lsr(nes, nes_zero_page_x(nes));
    return 6;
  case 0x58:
    nes_cli(nes);
    return 2;
  case 0x59:
    nes_eor(nes, nes_absolute_y(nes));
    return 4; // +1 if it crosses a page boundary
  case 0x5D:
    nes_eor(nes, nes_absolute_x(nes));
    return 4; // +1 if it crosses a page boundary
  case 0x5E:
    nes_lsr(nes, nes_absolute_x(nes));
    return 7;
  case 0x60:
    nes_rts(nes);
    return 6;
  case 0x61:
    nes_adc(nes, nes_indirect_x(nes));
    return 6;
  case 0x65:
    nes_adc(nes, nes_zero_page(nes));
    return 3;
  case 0x66:
    nes_ror(nes, nes_zero_page(nes));
    return 5;
  case 0x68:
    nes_pla(nes);
    return 4;
  case 0x69:
    nes_adc(nes, nes_immediate(nes));
    return 2;
  case 0x6A:
    nes_ror_a(nes);
    return 2;
  case 0x6C:
    nes_jmp(nes, nes_indirect(nes));
    return 5;
  case 0x6D:
    nes_adc(nes, nes_absolute(nes));
    return 4;
  case 0x6E:
    nes_ror(nes, nes_absolute(nes));
    return 6;
  case 0x70:
    nes_bvs(nes);
    return 2; // +1 if branch succeeds, +2 if it crosses a page boundary
  case 0x71:
    nes_adc(nes, nes_indirect_y(nes));
    return 5; // +1 if it crosses a page boundary
  case 0x75:
    nes_adc(nes, nes_zero_page_x(nes));
    return 4;
  case 0x76:
    nes_ror(nes, nes_zero_page_x(nes));
    return 6;
  case 0x78:
    nes_sei(nes);
    return 2;
  case 0x79:
    nes_adc(nes, nes_absolute_y(nes));
    return 4; // +1 if it crosses a page boundary
  case 0x7D:
    nes_adc(nes, nes_absolute_x(nes));
    return 4; // +1 if it crosses a page boundary
  case 0x7E:
    nes_ror(nes, nes_absolute_x(nes));
    return 7;
  case 0x81:
    nes_sta(nes, nes_indirect_x(nes));
    return 6;
  case 0x84:
    nes_sty(nes, nes_zero_page(nes));
    return 3;
  case 0x85:
    nes_sta(nes, nes_zero_page(nes));
    return 3;
  case 0x86:
    nes_stx(nes, nes_zero_page(nes));
    return 3;
  case 0x88:
    nes_dey(nes);
    return 2;
  case 0x8A:
    nes_txa(nes);
    return 2;
  case 0x8C:
    nes_sty(nes, nes_absolute(nes));
    return 4;
  case 0x8D:
    nes_sta(nes, nes_absolute(nes));
    return 4;
  case 0x8E:
    nes_stx(nes, nes_absolute(nes));
    return 4;
  case 0x90:
    nes_bcc(nes);
    return 2; // +1 if branch succeeds, +2 if it crosses a page boundary
  case 0x91:
    nes_sta(nes, nes_indirect_y(nes));
    return 6;
  case 0x94:
    nes_sty(nes, nes_zero_page_x(nes));
    return 4;
  case 0x95:
    nes_sta(nes, nes_zero_page_x(nes));
    return 4;
  case 0x96:
    nes_stx(nes, nes_zero_page_y(nes));
    return 4;
  case 0x98:
    nes_tya(nes);
    return 2;
  case 0x99:
    nes_sta(nes, nes_absolute_y(nes));
    return 5;
  case 0x9A:
    nes_txs(nes);
    return 2;
  case 0x9D:
    nes_sta(nes, nes_absolute_x(nes));
    return 5;
  case 0xA0:
    nes_ldy(nes, nes_immediate(nes));
    return 2;
  case 0xA1:
    nes_lda(nes, nes_indirect_x(nes));
    return 6;
  case 0xA2:
    nes_ldx(nes, nes_immediate(nes));
    return 2;
  case 0xA4:
    nes_ldy(nes, nes_zero_page(nes));
    return 3;
  case 0xA5:
    nes_lda(nes, nes_zero_page(nes));
    return 3;
  case 0xA6:
    nes_ldx(nes, nes_zero_page(nes));
    return 3;
  case 0xA8:
    nes_tay(nes);
    return 2;
  case 0xA9:
    nes_lda(nes, nes_immediate(nes));
    return 2;
  case 0xAA:
    nes_tax(nes);
    return 2;
  case 0xAC:
    nes_ldy(nes, nes_absolute(nes));
    return 4;
  case 0xAD:
    nes_lda(nes, nes_absolute(nes));
    return 4;
  case 0xAE:
    nes_ldx(nes, nes_absolute(nes));
    return 4;
  case 0xB0:
    nes_bcs(nes);
    return 2; // +1 if branch succeeds, +2 if it crosses a page boundary
  case 0xB1:
    nes_lda(nes, nes_indirect_y(nes));
    return 5; // +1 if it crosses a page boundary
  case 0xB4:
    nes_ldy(nes, nes_zero_page_x(nes));
    return 4;
  case 0xB5:
    nes_lda(nes, nes_zero_page_x(nes));
    return 4;
  case 0xB6:
    nes_ldx(nes, nes_zero_page_y(nes));
    return 4;
  case 0xB8:
    nes_clv(nes);
    return 2;
  case 0xB9:
    nes_lda(nes, nes_absolute_y(nes));
    return 4; // +1 if it crosses a page boundary
  case 0xBA:
    nes_tsx(nes);
    return 2;
  case 0xBC:
    nes_ldy(nes, nes_absolute_x(nes));
    return 4; // +1 if it crosses a page boundary
  case 0xBD:
    nes_lda(nes, nes_absolute_x(nes));
    return 4; // +1 if it crosses a page boundary
  case 0xBE:
    nes_ldx(nes, nes_absolute_y(nes));
    return 4; // +1 if it crosses a page boundary
  case 0xC0:
    nes_cpy(nes, nes_immediate(nes));
    return 2;
  case 0xC1:
    nes_cmp(nes, nes_indirect_x(nes));
    return 6;
  case 0xC4:
    nes_cpy(nes, nes_zero_page(nes));
    return 3;
  case 0xC5:
    nes_cmp(nes, nes_zero_page(nes));
    return 3;
  case 0xC6:
    nes_dec(nes, nes_zero_page(nes));
    return 5;
  case 0xC8:
    nes_iny(nes);
    return 2;
  case 0xC9:
    nes_cmp(nes, nes_immediate(nes));
    return 2;
  case 0xCA:
    nes_dex(nes);
    return 2;
  case 0xCC:
    nes_cpy(nes, nes_absolute(nes));
    return 4;
  case 0xCD:
    nes_cmp(nes, nes_absolute(nes));
    return 4;
  case 0xCE:
    nes_dec(nes, nes_absolute(nes));
    return 6;
  case 0xD0:
    nes_bne(nes);
    return 2; // +1 if branch succeeds, +2 if it crosses a page boundary
  case 0xD1:
    nes_cmp(nes, nes_indirect_y(nes));
    return 5; // +1 if it crosses a page boundary
  case 0xD5:
    nes_cmp(nes, nes_zero_page_x(nes));
    return 4;
  case 0xD6:
    nes_dec(nes, nes_zero_page_x(nes));
    return 6;
  case 0xD8:
    nes_cld(nes);
    return 2;
  case 0xD9:
    nes_cmp(nes, nes_absolute_y(nes));
    return 4; // +1 if it crosses a page boundary
  case 0xDD:
    nes_cmp(nes, nes_absolute_x(nes));
    return 4; // +1 if it crosses a page boundary
  case 0xDE:
    nes_dec(nes, nes_absolute_x(nes));
    return 7;
  case 0xE0:
    nes_cpx(nes, nes_immediate(nes));
    return 2;
  case 0xE1:
    nes_sbc(nes, nes_indirect_x(nes));
    return 6;
  case 0xE4:
    nes_cpx(nes, nes_zero_page(nes));
    return 3;
  case 0xE5:
    nes_sbc(nes, nes_zero_page(nes));
    return 3;
  case 0xE6:
    nes_inc(nes, nes_zero_page(nes));
    return 5;
  case 0xE8:
    nes_inx(nes);
    return 2;
  case 0xE9:
    nes_sbc(nes, nes_immediate(nes));
    return 2;
  case 0xEA:
    nes_nop(nes);
    return 2;
  case 0xEC:
    nes_cpx(nes, nes_absolute(nes));
    return 4;
  case 0xED:
    nes_sbc(nes, nes_absolute(nes));
    return 4;
  case 0xEE:
    nes_inc(nes, nes_absolute(nes));
    return 6;
  case 0xF0:
    nes_beq(nes);
    return 2; // +1 if branch succeeds, +2 if it crosses a page boundary
  case 0xF1:
    nes_sbc(nes, nes_indirect_y(nes));
    return 5; // +1 if it crosses a page boundary
  case 0xF5:
    nes_sbc(nes, nes_zero_page_x(nes));
    return 4;
  case 0xF6:
    nes_inc(nes, nes_zero_page_x(nes));
    return 6;
  case 0xF8:
    nes_sed(nes);
    return 2;
  case 0xF9:
    nes_sbc(nes, nes_absolute_y(nes));
    return 4; // +1 if it crosses a page boundary
  case 0xFD:
    nes_sbc(nes, nes_absolute_x(nes));
    return 4; // +1 if it crosses a page boundary
  case 0xFE:
    nes_inc(nes, nes_absolute_x(nes));
    return 7;
  default:
    nes_log_error("ERROR: Unknown opcode 0x%02X\n", opcode);
    exit(1);
    return 0; // No operation
  }
}