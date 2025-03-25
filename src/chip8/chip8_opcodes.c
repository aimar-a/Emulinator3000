/*
0NNN 	Call 		Calls machine code routine (RCA 1802 for COSMAC VIP) at address NNN. Not necessary for most ROMs.[24]
00E0 	Display 	disp_clear() 	Clears the screen.[24]
00EE 	Flow 	return; 	Returns from a subroutine.[24]
1NNN 	Flow 	goto NNN; 	Jumps to address NNN.[24]
2NNN 	Flow 	*(0xNNN)() 	Calls subroutine at NNN.[24]
3XNN 	Cond 	if (Vx == NN) 	Skips the next instruction if VX equals NN (usually the next instruction is a jump to skip a code block).[24]
4XNN 	Cond 	if (Vx != NN) 	Skips the next instruction if VX does not equal NN (usually the next instruction is a jump to skip a code block).[24]
5XY0 	Cond 	if (Vx == Vy) 	Skips the next instruction if VX equals VY (usually the next instruction is a jump to skip a code block).[24]
6XNN 	Const 	Vx = NN 	Sets VX to NN.[24]
7XNN 	Const 	Vx += NN 	Adds NN to VX (carry flag is not changed).[24]
8XY0 	Assig 	Vx = Vy 	Sets VX to the value of VY.[24]
8XY1 	BitOp 	Vx |= Vy 	Sets VX to VX or VY. (bitwise OR operation).[24]
8XY2 	BitOp 	Vx &= Vy 	Sets VX to VX and VY. (bitwise AND operation).[24]
8XY3[a] 	BitOp 	Vx ^= Vy 	Sets VX to VX xor VY.[24]
8XY4 	Math 	Vx += Vy 	Adds VY to VX. VF is set to 1 when there's an overflow, and to 0 when there is not.[24]
8XY5 	Math 	Vx -= Vy 	VY is subtracted from VX. VF is set to 0 when there's an underflow, and 1 when there is not. (i.e. VF set to 1 if VX >= VY and 0 if not).[24]
8XY6[a] 	BitOp 	Vx >>= 1 	Shifts VX to the right by 1, then stores the least significant bit of VX prior to the shift into VF.[b][24]
8XY7[a] 	Math 	Vx = Vy - Vx 	Sets VX to VY minus VX. VF is set to 0 when there's an underflow, and 1 when there is not. (i.e. VF set to 1 if VY >= VX).[24]
8XYE[a] 	BitOp 	Vx <<= 1 	Shifts VX to the left by 1, then sets VF to 1 if the most significant bit of VX prior to that shift was set, or to 0 if it was unset.[b][24]
9XY0 	Cond 	if (Vx != Vy) 	Skips the next instruction if VX does not equal VY. (Usually the next instruction is a jump to skip a code block).[24]
ANNN 	MEM 	I = NNN 	Sets I to the address NNN.[24]
BNNN 	Flow 	PC = V0 + NNN 	Jumps to the address NNN plus V0.[24]
CXNN 	Rand 	Vx = rand() & NN 	Sets VX to the result of a bitwise and operation on a random number (Typically: 0 to 255) and NN.[24]
DXYN 	Display 	draw(Vx, Vy, N) 	Draws a sprite at coordinate (VX, VY) that has a width of 8 pixels and a height of N pixels. Each row of 8 pixels is read as bit-coded starting from memory location I; I value does not change after the execution of this instruction. As described above, VF is set to 1 if any screen pixels are flipped from set to unset when the sprite is drawn, and to 0 if that does not happen.[24]
EX9E 	KeyOp 	if (key() == Vx) 	Skips the next instruction if the key stored in VX(only consider the lowest nibble) is pressed (usually the next instruction is a jump to skip a code block).[24]
EXA1 	KeyOp 	if (key() != Vx) 	Skips the next instruction if the key stored in VX(only consider the lowest nibble) is not pressed (usually the next instruction is a jump to skip a code block).[24]
FX07 	Timer 	Vx = get_delay() 	Sets VX to the value of the delay timer.[24]
FX0A 	KeyOp 	Vx = get_key() 	A key press is awaited, and then stored in VX (blocking operation, all instruction halted until next key event, delay and sound timers should continue processing).[24]
FX15 	Timer 	delay_timer(Vx) 	Sets the delay timer to VX.[24]
FX18 	Sound 	sound_timer(Vx) 	Sets the sound timer to VX.[24]
FX1E 	MEM 	I += Vx 	Adds VX to I. VF is not affected.[c][24]
FX29 	MEM 	I = sprite_addr[Vx] 	Sets I to the location of the sprite for the character in VX(only consider the lowest nibble). Characters 0-F (in hexadecimal) are represented by a 4x5 font.[24]
FX33 	BCD

set_BCD(Vx)
*(I+0) = BCD(3);
*(I+1) = BCD(2);
*(I+2) = BCD(1);

  Stores the binary-coded decimal representation of VX, with the hundreds digit in memory at location in I, the tens digit at location I+1, and the ones digit at location I+2.[24]
FX55 	MEM 	reg_dump(Vx, &I) 	Stores from V0 to VX (including VX) in memory, starting at address I. The offset from I is increased by 1 for each value written, but I itself is left unmodified.[d][24]
FX65 	MEM 	reg_load(Vx, &I) 	Fills from V0 to VX (including VX) with values from memory, starting at address I. The offset from I is increased by 1 for each value read, but I itself is left unmodified.[d][24]
*/
#include "chip8_opcodes.h"

Chip8 *chip8;

bool modosuperchip8 = false; // por defecto estamos en chip8

void chip8opcodesInit(Chip8 *chip8_)
{
  chip8 = chip8_;
}

void chip8opcodesEvaluate(uint16_t opcode)
{
  chip8_log("INFO: Evaluating opcode: 0x%X\n", opcode);

  uint8_t x = (opcode & 0x0F00) >> 8;
  uint8_t y = (opcode & 0x00F0) >> 4;
  uint8_t n = opcode & 0x000F;
  uint8_t nn = opcode & 0x00FF;
  uint16_t nnn = opcode & 0x0FFF;

  switch (opcode & 0xF000)
  {
  case 0x0000:
    switch (opcode)
    {
    case 0x00E0:
      // Clear the screen
      chip8displayLimpiarPantalla();
      break;
    case 0x00EE:
      // Return from a subroutine
      chip8->pc = chip8->stack[chip8->sp];
      chip8->sp--;
      break;

    case 0x00FD: // exit interpreter

      chip8_log("Opcode 0x00FD: exit interpreter");
      exit(0);
      break;

    case 0x00FE: // Activar modo chip8
      modosuperchip8 = false;
      // SCREEN_WIDTH=64;
      // SCREEN_HEIGHT=32;
      chip8displayLimpiarPantalla();
      break;

    case 0x00FF: // Activar modo super chip8
      modosuperchip8 = true;
      // SCREEN_WIDTH=128;
      // SCREEN_HEIGHT=64;
      chip8displayLimpiarPantalla();
      break;
    default:
      // Call RCA 1802 program at address NNN
      break;
    }
    break;
  case 0x1000:
    // Jump to address NNN
    chip8->pc = nnn;
    break;
  case 0x2000:
    // Call subroutine at NNN
    chip8->sp++;
    chip8->stack[chip8->sp] = chip8->pc;
    chip8->pc = nnn;
    break;
  case 0x3000:
    // Skip next instruction if Vx == NN
    if (chip8->V[x] == nn)
    {
      chip8->pc += 2;
    }
    break;
  case 0x4000:
    // Skip next instruction if Vx != NN
    if (chip8->V[x] != nn)
    {
      chip8->pc += 2;
    }
    break;
  case 0x5000:
    // Skip next instruction if Vx == Vy
    if (chip8->V[x] == chip8->V[y])
    {
      chip8->pc += 2;
    }
    break;
  case 0x6000:
    // Set Vx to NN
    chip8->V[x] = nn;
    break;
  case 0x7000:
    // Add NN to Vx
    chip8->V[x] += nn;
    break;
  case 0x8000:
    switch (opcode & 0x000F)
    {
    case 0x0000:
      // Set Vx to Vy
      chip8->V[x] = chip8->V[y];
      break;
    case 0x0001:
      // Set Vx to Vx | Vy
      chip8->V[x] |= chip8->V[y];
      break;
    case 0x0002:
      // Set Vx to Vx & Vy
      chip8->V[x] &= chip8->V[y];
      break;
    case 0x0003:
      // Set Vx to Vx ^ Vy
      chip8->V[x] ^= chip8->V[y];
      break;
    case 0x0004:
      // Add Vy to Vx
      chip8->V[x] += chip8->V[y];
      if (chip8->V[x] < chip8->V[y])
      {
        chip8->V[0xF] = 1;
      }
      else
      {
        chip8->V[0xF] = 0;
      }
      break;
    case 0x0005:
      // Subtract Vy from Vx
      if (chip8->V[x] > chip8->V[y])
      {
        chip8->V[0xF] = 1;
      }
      else
      {
        chip8->V[0xF] = 0;
      }
      chip8->V[x] -= chip8->V[y];
      break;
    case 0x0006:
      // Shift Vx right by 1
      chip8->V[0xF] = chip8->V[x] & 0x1;
      chip8->V[x] >>= 1;
      break;
    case 0x0007:
      // Set Vx to Vy - Vx
      if (chip8->V[y] > chip8->V[x])
      {
        chip8->V[0xF] = 1;
      }
      else
      {
        chip8->V[0xF] = 0;
      }
      chip8->V[x] = chip8->V[y] - chip8->V[x];
      break;
    case 0x000E:
      // Shift Vx left by 1
      chip8->V
          [0xF] = chip8->V[x] >> 7;
      chip8->V[x] <<= 1;
      break;
    default:
      chip8_log("ERROR: Unknown opcode: 0x%X\n", opcode);
      exit(1);
      break;
    }
    break;
  case 0x9000:
    // Skip next instruction if Vx != Vy
    if (chip8->V[x] != chip8->V[y])
    {
      chip8->pc += 2;
    }
    break;
  case 0xA000:
    // Set I to NNN
    chip8->I = nnn;
    break;
  case 0xB000:
    if (modosuperchip8)
    { // Jump to address XNN + VX if superchip8 mode

      chip8->pc = nnn + chip8->V[x];
    }
    else
    { // Jump to address NNN + V0 if chip 8 mode

      chip8->pc = nnn + chip8->V[0];
    }
    break;
  case 0xC000:
    // Set Vx to random number & NN
    chip8->V[x] = (rand() % 256) & nn;
    break;
  case 0xD000:
    // Draw sprite at Vx, Vy with width 8 and height N
    if (n == 0 && modosuperchip8)
    { // superChip8
      n = 16;
    }
    chip8->V[0xF] = chip8displayDrawSprite(chip8->V[x], chip8->V[y], &chip8->memoria[chip8->I], n);
    break;
  case 0xE000:
    switch (opcode & 0x00FF)
    {
    case 0x009E:
      // Skip next instruction if key in Vx is pressed
      if (chip8->teclado[chip8->V[x]] != 0)
      {
        chip8->pc += 2;
      }
      break;
    case 0x00A1:
      // Skip next instruction if key in Vx is not pressed
      if (chip8->teclado[chip8->V[x]] == 0)
      {
        chip8->pc += 2;
      }
      break;
    default:
      chip8_log("ERROR: Unknown opcode: 0x%X\n", opcode);
      exit(1);
      break;
    }
    break;
  case 0xF000:
    switch (opcode & 0x00FF)
    {
    case 0x0007:
      // Set Vx to delay timer
      chip8->V[x] = chip8->delay_timer;
      break;
    case 0x000A:
      // Wait for key press and store in Vx
      for (int i = 0; i < 16; i++)
      {
        if (chip8->teclado[i] != 0)
        {
          chip8->V[x] = i;
          break;
        }
        else
        {
          chip8->pc -= 2;
        }
      }
      break;
    case 0x0015:
      // Set delay timer to Vx
      chip8->delay_timer = chip8->V[x];
      break;
    case 0x0018:
      // Set sound timer to Vx
      chip8->sound_timer = chip8->V[x];
      break;
    case 0x001E:
      // Add Vx to I
      chip8->I += chip8->V[x];
      break;
    case 0x0029:
      // Set I to location of sprite for character in Vx
      if (modosuperchip8)
      {
        chip8->I = chip8->V[x] * 10; // superchip8
      }
      else
      {
        chip8->I = chip8->V[x] * 5;
      }
      break;
    case 0x0033:
      // Store BCD representation of Vx in memory locations I, I+1, I+2
      chip8->memoria[chip8->I] = chip8->V[x] / 100;
      chip8->memoria[chip8->I + 1] = (chip8->V[x] / 10) % 10;
      chip8->memoria[chip8->I + 2] = chip8->V[x] % 10;
      break;
    case 0x0055:
      // Store V0 to Vx in memory starting at address I
      for (int i = 0; i <= x; i++)
      {
        chip8->memoria[chip8->I + i] = chip8->V[i];
      }
      break;
    case 0x0065:
      // Load V0 to Vx from memory starting at address I
      for (int i = 0; i <= x; i++)
      {
        chip8->V[i] = chip8->memoria[chip8->I + i];
      }
      break;
    case 0x0075:
      // Store V0 to Vx in memory starting at address I
      if (modosuperchip8)
      {
        for (int i = 0; i <= x; i++)
        {
          chip8->memoria[chip8->I + i] = chip8->V[i];
        }
      }
      break;
    case 0x0085:
      // Load V0 to Vx from memory starting at address I
      if (modosuperchip8)
      {
        for (int i = 0; i <= x; i++)
        {
          chip8->V[i] = chip8->memoria[chip8->I + i];
        }
      }
      break;
    default:
      chip8_log("ERROR: Unknown opcode: 0x%X\n", opcode);
      exit(1);
      break;
    }
    break;
  default:
    chip8_log("ERROR: Unknown opcode: 0x%X\n", opcode);
    exit(1);
    break;
  }
}
