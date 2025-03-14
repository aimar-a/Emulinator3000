#include "nes_cpu.h"
#include "nes_opcodes.h"
#include "nes_structure.h"
#include "nes_display.h"
#include "nes_controller.h"
#include "nes_memory.h"
#include "nes_rom.h"
#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>

void nes_launch()
{
  NES nes;
  nes_reset(&nes);
  nes.rom = nes_load_rom("resources/nes-roms/Tetris.nes");
  nes_run(&nes);
}

void nes_reset(NES *nes)
{
  nes->A = 0;
  nes->X = 0;
  nes->Y = 0;
  nes->PC = 0x8000;
  nes->SP = 0xFD;
  nes->P = 0x24;
  nes->controller_strobe = false;
  for (int i = 0; i < 2; i++)
  {
    nes->controller_shift[i] = 0;
    nes->controller_state[i] = 0;
  }
  for (int i = 0; i < 0x8000; i++)
  {
    nes->memory[i] = 0;
  }
}

void nes_run(NES *nes)
{
  while (true)
  {
    SDL_Delay(16);
    nes_controller_update(nes);
    nes_evaluate_opcode(nes);
    nes_display_draw(nes->screen);
  }
}