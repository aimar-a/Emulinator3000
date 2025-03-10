#include "chip8_structure.h"
#include "chip8_display.h"
#include "chip8_input.h"
#include "chip8_timers.h"
#include "chip8_opcodes.h"
#include <SDL2/SDL.h>
#include <stdio.h>

int main(int argc, char *argv[])
{
  Chip8 chip8;
  memset(chip8.V, 0, sizeof(chip8.V));
  chip8.pc = 0x200;
  chip8.I = 0;
  chip8.sp = 0;
  timersInit(&chip8.delay_timer, &chip8.sound_timer);
  memset(chip8.stack, 0, sizeof(chip8.stack));
  memset(chip8.memoria, 0, sizeof(chip8.memoria));
  inputInitTeclado(&chip8.teclado[0], &chip8.esc);
  displayInitPantalla(&chip8.pantalla[0]);
  opcodesInit(&chip8);

  // Load ROM
  // FILE *rom = fopen("../resources/chip8-roms/games/Tetris [Fran Dachille, 1991].ch8", "rb");
  // FILE *rom = fopen("../resources/chip8-roms/programs/BMP Viewer - Hello (C8 example) [Hap, 2005].ch8", "rb");
  // FILE *rom = fopen("../resources/chip8-roms/programs/Chip8 emulator Logo [Garstyciuks].ch8", "rb");
  // FILE *rom = fopen("../resources/chip8-roms/games/Airplane.ch8", "rb");
  FILE *rom = fopen("../resources/chip8-roms/games/Space Invaders [David Winter].ch8", "rb");
  // FILE *rom = fopen("../resources/chip8-roms/programs/Delay Timer Test [Matthew Mikolay, 2010].ch8", "rb");
  // FILE *rom = fopen("../resources/chip8-roms/programs/Keypad Test [Hap, 2006].ch8", "rb");

  if (rom == NULL)
  {
    perror("No se pudo abrir el archivo");
    return 1;
  }
  fread(&chip8.memoria[0x200], 1, 4096 - 0x200, rom);
  fclose(rom);

  // CPU loop while not pressing ESC
  do
  {
    // Fetch opcode
    SDL_Delay(16);
    printf("PC: %u\t", chip8.pc);
    printf("Opcode: 0x%X\t", (chip8.memoria[chip8.pc] << 8) | chip8.memoria[chip8.pc + 1]);
    printf("ST: %u\tDT: %u\n", chip8.sound_timer, chip8.delay_timer);

    uint16_t opcode = (chip8.memoria[chip8.pc] << 8) | chip8.memoria[chip8.pc + 1];
    chip8.pc += 2;

    displayPrintPantalla();
    timersDecrement();
    inputCapturarTeclado();

    // Decode and execute opcode
    opcodesEvaluate(opcode);
  } while (!chip8.esc);

  SDL_CloseAudio();
  displayCerrarPantalla();
  SDL_Quit();

  return 0;
}
