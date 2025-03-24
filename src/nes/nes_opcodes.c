#include "nes_opcodes.h"

uint8_t nes_evaluate_opcode(NES *nes)
{
  uint8_t opcode = nes_read(nes, nes->PC);
  nes->PC++;
  printf("PC: %04X, Opcode: %02X (%02X %02X)\n", nes->PC - 1, opcode, nes_read(nes, nes->PC), nes_read(nes, nes->PC + 1));
  switch (opcode)
  {
  case 0x00:
    printf("ERROR: Opcode 0x00 not implemented\n");
    exit(1);
    nes_brk(nes);
    return 7;
  case 0x01:
    nes_ora(nes, nes_indirect_x(nes));
    return 1;
  case 0x05:
    nes_ora(nes, nes_zero_page(nes));
    return 2;
  case 0x06:
    nes_asl(nes, nes_zero_page(nes));
    return 5;
  case 0x08:
    nes_php(nes);
    break;
  case 0x09:
    nes_ora(nes, nes_immediate(nes));
    break;
  case 0x0A:
    nes_asl_a(nes);
    break;
  case 0x0D:
    nes_ora(nes, nes_absolute(nes));
    break;
  case 0x0E:
    nes_asl(nes, nes_absolute(nes));
    break;
  case 0x10:
    nes_bpl(nes);
    break;
  case 0x11:
    nes_ora(nes, nes_indirect_y(nes));
    break;
  case 0x15:
    nes_ora(nes, nes_zero_page_x(nes));
    break;
  case 0x16:
    nes_asl(nes, nes_zero_page_x(nes));
    break;
  case 0x18:
    nes_clc(nes);
    break;
  case 0x19:
    nes_ora(nes, nes_absolute_y(nes));
    break;
  case 0x1D:
    nes_ora(nes, nes_absolute_x(nes));
    break;
  case 0x1E:
    nes_asl(nes, nes_absolute_x(nes));
    break;
  case 0x20:
    nes_jsr(nes);
    break;
  case 0x21:
    nes_and(nes, nes_indirect_x(nes));
    break;
  case 0x24:
    nes_bit(nes, nes_zero_page(nes));
    break;
  case 0x25:
    nes_and(nes, nes_zero_page(nes));
    break;
  case 0x26:
    nes_rol(nes, nes_zero_page(nes));
    break;
  case 0x28:
    nes_plp(nes);
    break;
  case 0x29:
    nes_and(nes, nes_immediate(nes));
    break;
  case 0x2A:
    nes_rol_a(nes);
    break;
  case 0x2C:
    nes_bit(nes, nes_absolute(nes));
    break;
  case 0x2D:
    nes_and(nes, nes_absolute(nes));
    break;
  case 0x2E:
    nes_rol(nes, nes_absolute(nes));
    break;
  case 0x30:
    nes_bmi(nes);
    break;
  case 0x31:
    nes_and(nes, nes_indirect_y(nes));
    break;
  case 0x35:
    nes_and(nes, nes_zero_page_x(nes));
    break;
  case 0x36:
    nes_rol(nes, nes_zero_page_x(nes));
    break;
  case 0x38:
    nes_sec(nes);
    break;
  case 0x39:
    nes_and(nes, nes_absolute_y(nes));
    break;
  case 0x3D:
    nes_and(nes, nes_absolute_x(nes));
    break;
  case 0x3E:
    nes_rol(nes, nes_absolute_x(nes));
    break;
  case 0x40:
    nes_rti(nes);
    break;
  case 0x41:
    nes_eor(nes, nes_indirect_x(nes));
    break;
  case 0x45:
    nes_eor(nes, nes_zero_page(nes));
    break;
  case 0x46:
    nes_lsr(nes, nes_zero_page(nes));
    break;
  case 0x48:
    nes_pha(nes);
    break;
  case 0x49:
    nes_eor(nes, nes_immediate(nes));
    break;
  case 0x4A:
    nes_lsr_a(nes);
    break;
  case 0x4C:
    nes_jmp(nes);
    break;
  case 0x4D:
    nes_eor(nes, nes_absolute(nes));
    break;
  case 0x4E:
    nes_lsr(nes, nes_absolute(nes));
    break;
  case 0x50:
    nes_bvc(nes);
    break;
  case 0x51:
    nes_eor(nes, nes_indirect_y(nes));
    break;
  case 0x55:
    nes_eor(nes, nes_zero_page_x(nes));
    break;
  case 0x56:
    nes_lsr(nes, nes_zero_page_x(nes));
    break;
  case 0x58:
    nes_cli(nes);
    break;
  case 0x59:
    nes_eor(nes, nes_absolute_y(nes));
    break;
  case 0x5D:
    nes_eor(nes, nes_absolute_x(nes));
    break;
  case 0x5E:
    nes_lsr(nes, nes_absolute_x(nes));
    break;
  case 0x60:
    nes_rts(nes);
    break;
  case 0x61:
    nes_adc(nes, nes_indirect_x(nes));
    break;
  case 0x65:
    nes_adc(nes, nes_zero_page(nes));
    break;
  case 0x66:
    nes_ror(nes, nes_zero_page(nes));
    break;
  case 0x68:
    nes_pla(nes);
    break;
  case 0x69:
    nes_adc(nes, nes_immediate(nes));
    break;
  case 0x6A:
    nes_ror_a(nes);
    break;
  case 0x6C:
    nes_jmp_indirect(nes);
    break;
  case 0x6D:
    nes_adc(nes, nes_absolute(nes));
    break;
  case 0x6E:
    nes_ror(nes, nes_absolute(nes));
    break;
  case 0x70:
    nes_bvs(nes);
    break;
  case 0x71:
    nes_adc(nes, nes_indirect_y(nes));
    break;
  case 0x75:
    nes_adc(nes, nes_zero_page_x(nes));
    break;
  case 0x76:
    nes_ror(nes, nes_zero_page_x(nes));
    break;
  case 0x78:
    nes_sei(nes);
    break;
  case 0x79:
    nes_adc(nes, nes_absolute_y(nes));
    break;
  case 0x7D:
    nes_adc(nes, nes_absolute_x(nes));
    break;
  case 0x7E:
    nes_ror(nes, nes_absolute_x(nes));
    break;
  case 0x81:
    nes_sta(nes, nes_indirect_x(nes));
    break;
  case 0x84:
    nes_sty(nes, nes_zero_page(nes));
    break;
  case 0x85:
    nes_sta(nes, nes_zero_page(nes));
    break;
  case 0x86:
    nes_stx(nes, nes_zero_page(nes));
    break;
  case 0x88:
    nes_dey(nes);
    break;
  case 0x8A:
    nes_txa(nes);
    break;
  case 0x8C:
    nes_sty(nes, nes_absolute(nes));
    break;
  case 0x8D:
    nes_sta(nes, nes_absolute(nes));
    break;
  case 0x8E:
    nes_stx(nes, nes_absolute(nes));
    break;
  case 0x90:
    nes_bcc(nes);
    break;
  case 0x91:
    nes_sta(nes, nes_indirect_y(nes));
    break;
  case 0x94:
    nes_sty(nes, nes_zero_page_x(nes));
    break;
  case 0x95:
    nes_sta(nes, nes_zero_page_x(nes));
    break;
  case 0x96:
    nes_stx(nes, nes_zero_page_y(nes));
    break;
  case 0x98:
    nes_tya(nes);
    break;
  case 0x99:
    nes_sta(nes, nes_absolute_y(nes));
    break;
  case 0x9A:
    nes_txs(nes);
    break;
  case 0x9D:
    nes_sta(nes, nes_absolute_x(nes));
    break;
  case 0xA0:
    nes_ldy(nes, nes_immediate(nes));
    break;
  case 0xA1:
    nes_lda(nes, nes_indirect_x(nes));
    break;
  case 0xA2:
    nes_ldx(nes, nes_immediate(nes));
    break;
  case 0xA4:
    nes_ldy(nes, nes_zero_page(nes));
    break;
  case 0xA5:
    nes_lda(nes, nes_zero_page(nes));
    break;
  case 0xA6:
    nes_ldx(nes, nes_zero_page(nes));
    break;
  case 0xA8:
    nes_tay(nes);
    break;
  case 0xA9:
    nes_lda(nes, nes_immediate(nes));
    break;
  case 0xAA:
    nes_tax(nes);
    break;
  case 0xAC:
    nes_ldy(nes, nes_absolute(nes));
    break;
  case 0xAD:
    nes_lda(nes, nes_absolute(nes));
    break;
  case 0xAE:
    nes_ldx(nes, nes_absolute(nes));
    break;
  case 0xB0:
    nes_bcs(nes);
    break;
  case 0xB1:
    nes_lda(nes, nes_indirect_y(nes));
    break;
  case 0xB4:
    nes_ldy(nes, nes_zero_page_x(nes));
    break;
  case 0xB5:
    nes_lda(nes, nes_zero_page_x(nes));
    break;
  case 0xB6:
    nes_ldx(nes, nes_zero_page_y(nes));
    break;
  case 0xB8:
    nes_clv(nes);
    break;
  case 0xB9:
    nes_lda(nes, nes_absolute_y(nes));
    break;
  case 0xBA:
    nes_tsx(nes);
    break;
  case 0xBC:
    nes_ldy(nes, nes_absolute_x(nes));
    break;
  case 0xBD:
    nes_lda(nes, nes_absolute_x(nes));
    break;
  case 0xBE:
    nes_ldx(nes, nes_absolute_y(nes));
    break;
  case 0xC0:
    nes_cpy(nes, nes_immediate(nes));
    break;
  case 0xC1:
    nes_cmp(nes, nes_indirect_x(nes));
    break;
  case 0xC4:
    nes_cpy(nes, nes_zero_page(nes));
    break;
  case 0xC5:
    nes_cmp(nes, nes_zero_page(nes));
    break;
  case 0xC6:
    nes_dec(nes, nes_zero_page(nes));
    break;
  case 0xC8:
    nes_iny(nes);
    break;
  case 0xC9:
    nes_cmp(nes, nes_immediate(nes));
    break;
  case 0xCA:
    nes_dex(nes);
    break;
  case 0xCC:
    nes_cpy(nes, nes_absolute(nes));
    break;
  case 0xCD:
    nes_cmp(nes, nes_absolute(nes));
    break;
  case 0xCE:
    nes_dec(nes, nes_absolute(nes));
    break;
  case 0xD0:
    nes_bne(nes);
    break;
  case 0xD1:
    nes_cmp(nes, nes_indirect_y(nes));
    break;
  case 0xD5:
    nes_cmp(nes, nes_zero_page_x(nes));
    break;
  case 0xD6:
    nes_dec(nes, nes_zero_page_x(nes));
    break;
  case 0xD8:
    nes_cld(nes);
    break;
  case 0xD9:
    nes_cmp(nes, nes_absolute_y(nes));
    break;
  case 0xDD:
    nes_cmp(nes, nes_absolute_x(nes));
    break;
  case 0xDE:
    nes_dec(nes, nes_absolute_x(nes));
    break;
  case 0xE0:
    nes_cpx(nes, nes_immediate(nes));
    break;
  case 0xE1:
    nes_sbc(nes, nes_indirect_x(nes));
    break;
  case 0xE4:
    nes_cpx(nes, nes_zero_page(nes));
    break;
  case 0xE5:
    nes_sbc(nes, nes_zero_page(nes));
    break;
  case 0xE6:
    nes_inc(nes, nes_zero_page(nes));
    break;
  case 0xE8:
    nes_inx(nes);
    break;
  case 0xE9:
    nes_sbc(nes, nes_immediate(nes));
    break;
  case 0xEA:
    nes_nop(nes);
    break;
  case 0xEC:
    nes_cpx(nes, nes_absolute(nes));
    break;
  case 0xED:
    nes_sbc(nes, nes_absolute(nes));
    break;
  case 0xEE:
    nes_inc(nes, nes_absolute(nes));
    break;
  case 0xF0:
    nes_beq(nes);
    break;
  case 0xF1:
    nes_sbc(nes, nes_indirect_y(nes));
    break;
  case 0xF5:
    nes_sbc(nes, nes_zero_page_x(nes));
    break;
  case 0xF6:
    nes_inc(nes, nes_zero_page_x(nes));
    break;
  case 0xF8:
    nes_sed(nes);
    break;
  case 0xF9:
    nes_sbc(nes, nes_absolute_y(nes));
    break;
  case 0xFD:
    nes_sbc(nes, nes_absolute_x(nes));
    break;
  case 0xFE:
    nes_inc(nes, nes_absolute_x(nes));
    break;
  default:
    printf("Unknown opcode %02X\n", opcode);
    exit(1);
    break;
  }

  return 4; // TODO: Return the correct number of cycles 4 each opcode
}