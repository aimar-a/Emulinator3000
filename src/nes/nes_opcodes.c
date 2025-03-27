#include "nes_opcodes.h"

uint8_t nes_evaluate_opcode(NES *nes)
{
  uint8_t opcode = nes_read(nes, nes->PC);
  nes->PC++;
  nes_log("INFO: PC: 0x%04X, Opcode: 0x%02X (0x%02X 0x%02X)\n", nes->PC - 1, opcode, nes_read(nes, nes->PC), nes_read(nes, nes->PC + 1));
  printf("INFO: PC: 0x%04X, Opcode: 0x%02X (0x%02X 0x%02X)\n", nes->PC - 1, opcode, nes_read(nes, nes->PC), nes_read(nes, nes->PC + 1));
  switch (opcode)
  {
  case 0x00:
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
    nes_jsr(nes);
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
    nes_jmp(nes);
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
    nes_jmp_indirect(nes);
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
    nes_log("ERROR: Unknown opcode 0x%02X\n", opcode);
    exit(1);
    break;
  }
  nes_log("ERROR: Opcode 0x%02X not implemented\n", opcode);
  exit(1);
}