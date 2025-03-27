#include "nes_instructions.h"

#define CARRY 0x01
#define ZERO 0x02
#define INTERRUPT 0x04
#define DECIMAL 0x08
#define BREAK 0x10
#define OVERFLOW 0x40
#define NEGATIVE 0x80

// esto deberia de ir en otro sitio pero por ahora se queda aqui
void nes_push(NES *nes, uint8_t value)
{
  nes_write(nes, 0x100 | nes->SP, value);
  nes->SP--;
}

uint8_t nes_pull(NES *nes)
{
  nes->SP++;
  return nes_read(nes, 0x100 | nes->SP);
}
// hasta aqui

/*
Official 6502 Instructions
ADC 	AND 	ASL 	BCC 	BCS 	BEQ 	BIT 	BMI 	BNE 	BPL 	BRK 	BVC 	BVS 	CLC
CLD 	CLI 	CLV 	CMP 	CPX 	CPY 	DEC 	DEX 	DEY 	EOR 	INC 	INX 	INY 	JMP
JSR 	LDA 	LDX 	LDY 	LSR 	NOP 	ORA 	PHA 	PHP 	PLA 	PLP 	ROL 	ROR 	RTI
RTS 	SBC 	SEC 	SED 	SEI 	STA 	STX 	STY 	TAX 	TAY 	TSX 	TXA 	TXS 	TYA
*/

void nes_adc(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  uint16_t result = nes->A + value + (nes->P & CARRY);
  nes->P &= ~(CARRY | OVERFLOW | NEGATIVE | ZERO);

  if (result > 0xFF)
  {
    nes->P |= CARRY;
  }

  if ((nes->A ^ result) & (value ^ result) & 0x80)
  {
    nes->P |= OVERFLOW;
  }

  nes->A = result & 0xFF;

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_and(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->A &= value;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_asl(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->P &= ~(CARRY | NEGATIVE | ZERO);

  if (value & 0x80)
  {
    nes->P |= CARRY;
  }

  value <<= 1;

  if (value == 0)
  {
    nes->P |= ZERO;
  }

  if (value & 0x80)
  {
    nes->P |= NEGATIVE;
  }

  nes_write(nes, address, value);
}

void nes_bcc(NES *nes)
{
  if (!(nes->P & CARRY))
  {
    nes->PC += (int8_t)nes_read(nes, nes->PC);
  }
  else
  {
    nes->PC++;
  }
}

void nes_bcs(NES *nes)
{
  if (nes->P & CARRY)
  {
    nes->PC += (int8_t)nes_read(nes, nes->PC);
  }
  else
  {
    nes->PC++;
  }
}

void nes_beq(NES *nes)
{
  if (nes->P & ZERO)
  {
    nes->PC += (int8_t)nes_read(nes, nes->PC);
  }
  else
  {
    nes->PC++;
  }
}

void nes_bit(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->P &= ~(NEGATIVE | OVERFLOW | ZERO);

  if (nes->A & value)
  {
    nes->P |= ZERO;
  }

  if (value & 0x80)
  {
    nes->P |= NEGATIVE;
  }

  if (value & 0x40)
  {
    nes->P |= OVERFLOW;
  }
}

void nes_bmi(NES *nes)
{
  if (nes->P & NEGATIVE)
  {
    nes->PC += (int8_t)nes_read(nes, nes->PC);
  }
  else
  {
    nes->PC++;
  }
}

void nes_bne(NES *nes)
{
  if (!(nes->P & ZERO))
  {
    nes->PC += (int8_t)nes_read(nes, nes->PC);
  }
  else
  {
    nes->PC++;
  }
}

void nes_bpl(NES *nes)
{
  if (!(nes->P & NEGATIVE))
  {
    nes->PC += (int8_t)nes_read(nes, nes->PC);
  }
  else
  {
    nes->PC++;
  }
}

void nes_brk(NES *nes)
{
  nes->PC++;
  nes_push(nes, nes->PC >> 8);
  nes_push(nes, nes->PC & 0xFF);
  nes_push(nes, nes->P | BREAK);
  nes->P |= INTERRUPT;
  nes->PC = nes_read(nes, 0xFFFE) | (nes_read(nes, 0xFFFF) << 8);
}

void nes_bvc(NES *nes)
{
  if (!(nes->P & OVERFLOW))
  {
    nes->PC += (int8_t)nes_read(nes, nes->PC);
  }
  else
  {
    nes->PC++;
  }
}

void nes_bvs(NES *nes)
{
  if (nes->P & OVERFLOW)
  {
    nes->PC += (int8_t)nes_read(nes, nes->PC);
  }
  else
  {
    nes->PC++;
  }
}

void nes_clc(NES *nes)
{
  nes->P &= ~CARRY;
}

void nes_cld(NES *nes)
{
  nes->P &= ~DECIMAL;
}

void nes_cli(NES *nes)
{
  nes->P &= ~INTERRUPT;
}

void nes_clv(NES *nes)
{
  nes->P &= ~OVERFLOW;
}

void nes_cmp(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->P &= ~(CARRY | NEGATIVE | ZERO);

  if (nes->A >= value)
  {
    nes->P |= CARRY;
  }

  if (nes->A == value)
  {
    nes->P |= ZERO;
  }

  if ((nes->A - value) & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_cpx(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->P &= ~(CARRY | NEGATIVE | ZERO);

  if (nes->X >= value)
  {
    nes->P |= CARRY;
  }

  if (nes->X == value)
  {
    nes->P |= ZERO;
  }

  if ((nes->X - value) & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_cpy(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->P &= ~(CARRY | NEGATIVE | ZERO);

  if (nes->Y >= value)
  {
    nes->P |= CARRY;
  }

  if (nes->Y == value)
  {
    nes->P |= ZERO;
  }

  if ((nes->Y - value) & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_dec(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->P &= ~(NEGATIVE | ZERO);

  value--;

  if (value == 0)
  {
    nes->P |= ZERO;
  }

  if (value & 0x80)
  {
    nes->P |= NEGATIVE;
  }

  nes_write(nes, address, value);
}

void nes_dex(NES *nes)
{
  nes->X--;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->X == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->X & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_dey(NES *nes)
{
  nes->Y--;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->Y == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->Y & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_eor(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->A ^= value;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_inc(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->P &= ~(NEGATIVE | ZERO);

  value++;

  if (value == 0)
  {
    nes->P |= ZERO;
  }

  if (value & 0x80)
  {
    nes->P |= NEGATIVE;
  }

  nes_write(nes, address, value);
}

void nes_inx(NES *nes)
{
  nes->X++;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->X == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->X & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_iny(NES *nes)
{
  nes->Y++;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->Y == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->Y & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_jmp(NES *nes)
{
  nes->PC = nes_read(nes, nes->PC) | (nes_read(nes, nes->PC + 1) << 8);
}

void nes_jsr(NES *nes)
{
  nes->PC--;
  nes_push(nes, nes->PC >> 8);
  nes_push(nes, nes->PC & 0xFF);
  nes->PC = nes_read(nes, nes->PC + 1) | (nes_read(nes, nes->PC + 2) << 8);
}

void nes_lda(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->A = value;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_ldx(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->X = value;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->X == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->X & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_ldy(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->Y = value;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->Y == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->Y & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_lsr(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->P &= ~(CARRY | NEGATIVE | ZERO);

  if (value & 0x01)
  {
    nes->P |= CARRY;
  }

  value >>= 1;

  if (value == 0)
  {
    nes->P |= ZERO;
  }

  if (value & 0x80)
  {
    nes->P |= NEGATIVE;
  }

  nes_write(nes, address, value);
}

void nes_nop(NES *nes)
{
}

void nes_ora(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  nes->A |= value;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_pha(NES *nes)
{
  nes_push(nes, nes->A);
}

void nes_php(NES *nes)
{
  nes_push(nes, nes->P | BREAK);
}

void nes_pla(NES *nes)
{
  nes->A = nes_pull(nes);
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_plp(NES *nes)
{
  nes->P = nes_pull(nes) | BREAK;
}

void nes_rol(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  uint8_t carry = nes->P & CARRY;
  nes->P &= ~(CARRY | NEGATIVE | ZERO);

  if (value & 0x80)
  {
    nes->P |= CARRY;
  }

  value <<= 1;
  value |= carry;

  if (value == 0)
  {
    nes->P |= ZERO;
  }

  if (value & 0x80)
  {
    nes->P |= NEGATIVE;
  }

  nes_write(nes, address, value);
}

void nes_ror(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  uint8_t carry = nes->P & CARRY;
  nes->P &= ~(CARRY | NEGATIVE | ZERO);

  if (value & 0x01)
  {
    nes->P |= CARRY;
  }

  value >>= 1;
  value |= carry << 7;

  if (value == 0)
  {
    nes->P |= ZERO;
  }

  if (value & 0x80)
  {
    nes->P |= NEGATIVE;
  }

  nes_write(nes, address, value);
}

void nes_rti(NES *nes)
{
  nes->P = nes_pull(nes) | BREAK;
  nes->PC = nes_pull(nes);
  nes->PC |= nes_pull(nes) << 8;
}

void nes_rts(NES *nes)
{
  nes->PC = nes_pull(nes);
  nes->PC |= nes_pull(nes) << 8;
  nes->PC++;
}

void nes_sbc(NES *nes, uint16_t address)
{
  uint8_t value = nes_read(nes, address);
  uint16_t result = nes->A - value - (1 - (nes->P & CARRY));
  nes->P &= ~(CARRY | OVERFLOW | NEGATIVE | ZERO);

  if (result < 0x100)
  {
    nes->P |= CARRY;
  }

  if ((nes->A ^ result) & (nes->A ^ value) & 0x80)
  {
    nes->P |= OVERFLOW;
  }

  nes->A = result & 0xFF;

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_sec(NES *nes)
{
  nes->P |= CARRY;
}

void nes_sed(NES *nes)
{
  nes->P |= DECIMAL;
}

void nes_sei(NES *nes)
{
  nes->P |= INTERRUPT;
}

void nes_sta(NES *nes, uint16_t address)
{
  nes_write(nes, address, nes->A);
}

void nes_stx(NES *nes, uint16_t address)
{
  nes_write(nes, address, nes->X);
}

void nes_sty(NES *nes, uint16_t address)
{
  nes_write(nes, address, nes->Y);
}

void nes_tax(NES *nes)
{
  nes->X = nes->A;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->X == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->X & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_tay(NES *nes)
{
  nes->Y = nes->A;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->Y == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->Y & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_tsx(NES *nes)
{
  nes->X = nes->SP;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->X == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->X & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_txa(NES *nes)
{
  nes->A = nes->X;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_txs(NES *nes)
{
  nes->SP = nes->X;
}

void nes_tya(NES *nes)
{
  nes->A = nes->Y;
  nes->P &= ~(NEGATIVE | ZERO);

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

// Los raritos de la casa

void nes_asl_a(NES *nes)
{
  nes->P &= ~(CARRY | NEGATIVE | ZERO);

  if (nes->A & 0x80)
  {
    nes->P |= CARRY;
  }

  nes->A <<= 1;

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_rol_a(NES *nes)
{
  uint8_t carry = nes->P & CARRY;
  nes->P &= ~(CARRY | NEGATIVE | ZERO);

  if (nes->A & 0x80)
  {
    nes->P |= CARRY;
  }

  nes->A <<= 1;
  nes->A |= carry;

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_ror_a(NES *nes)
{
  uint8_t carry = nes->P & CARRY;
  nes->P &= ~(CARRY | NEGATIVE | ZERO);

  if (nes->A & 0x01)
  {
    nes->P |= CARRY;
  }

  nes->A >>= 1;
  nes->A |= carry << 7;

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}

void nes_jmp_indirect(NES *nes)
{
  uint16_t address = nes_read(nes, nes->PC) | (nes_read(nes, nes->PC + 1) << 8);
  nes->PC = nes_read(nes, address) | (nes_read(nes, address + 1) << 8);
}

void nes_lsr_a(NES *nes)
{
  nes->P &= ~(CARRY | NEGATIVE | ZERO);

  if (nes->A & 0x01)
  {
    nes->P |= CARRY;
  }

  nes->A >>= 1;

  if (nes->A == 0)
  {
    nes->P |= ZERO;
  }

  if (nes->A & 0x80)
  {
    nes->P |= NEGATIVE;
  }
}