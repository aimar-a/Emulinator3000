#ifndef NES_INSTRUCTIONS_H
#define NES_INSTRUCTIONS_H

#include "nes_structure.h"

void nes_adc(NES *nes, uint8_t value);
void nes_and(NES *nes, uint8_t value);
void nes_asl(NES *nes, uint16_t address);
void nes_bcc(NES *nes);
void nes_bcs(NES *nes);
void nes_beq(NES *nes);
void nes_bit(NES *nes, uint16_t address);
void nes_bmi(NES *nes);
void nes_bne(NES *nes);
void nes_bpl(NES *nes);
void nes_brk(NES *nes);
void nes_bvc(NES *nes);
void nes_bvs(NES *nes);
void nes_clc(NES *nes);
void nes_cld(NES *nes);
void nes_cli(NES *nes);
void nes_clv(NES *nes);
void nes_cmp(NES *nes, uint8_t value);
void nes_cpx(NES *nes, uint8_t value);
void nes_cpy(NES *nes, uint8_t value);
void nes_dec(NES *nes, uint16_t address);
void nes_dex(NES *nes);
void nes_dey(NES *nes);
void nes_eor(NES *nes, uint8_t value);
void nes_inc(NES *nes, uint16_t address);
void nes_inx(NES *nes);
void nes_iny(NES *nes);
void nes_jmp(NES *nes);
void nes_jsr(NES *nes);
void nes_lda(NES *nes, uint8_t value);
void nes_ldx(NES *nes, uint8_t value);
void nes_ldy(NES *nes, uint8_t value);
void nes_lsr(NES *nes, uint16_t address);
void nes_nop(NES *nes);
void nes_ora(NES *nes, uint8_t value);
void nes_pha(NES *nes);
void nes_php(NES *nes);
void nes_pla(NES *nes);
void nes_plp(NES *nes);
void nes_rol(NES *nes, uint16_t address);
void nes_ror(NES *nes, uint16_t address);
void nes_rti(NES *nes);
void nes_rts(NES *nes);
void nes_sbc(NES *nes, uint8_t value);
void nes_sec(NES *nes);
void nes_sed(NES *nes);
void nes_sei(NES *nes);
void nes_sta(NES *nes, uint16_t address);
void nes_stx(NES *nes, uint16_t address);
void nes_sty(NES *nes, uint16_t address);
void nes_tax(NES *nes);
void nes_tay(NES *nes);
void nes_tsx(NES *nes);
void nes_txa(NES *nes);
void nes_txs(NES *nes);
void nes_tya(NES *nes);
// Los raritos de la casa
void nes_asl_a(NES *nes);
void nes_ror_a(NES *nes);
void nes_jmp_indirect(NES *nes);
void nes_rol_a(NES *nes);
void nes_lsr_a(NES *nes);

#endif