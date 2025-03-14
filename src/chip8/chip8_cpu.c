#include "chip8_cpu.h"
#include "chip8_structure.h"
#include "chip8_display.h"
#include "chip8_input.h"
#include "chip8_timers.h"
#include "chip8_opcodes.h"
#include <SDL2/SDL.h>
#include <stdio.h>


void inicializarMemoria(Chip8 *chip8, bool modosuperchip8) {
  if (modosuperchip8) {
      // Si está en SuperChip8, se reserva memoria para 128KB
      chip8->memoria = (uint8_t *)malloc(131072); // 128KB
  } else {
      // Si está en Chip8, se reserva memoria para 4KB
      chip8->memoria = (uint8_t *)malloc(4096);  // 4KB
  }

  if (chip8->memoria == NULL) { //para combrobar si ha habido algun fallo al reservar la memoria
      printf("Error: No se pudo asignar memoria.\n");
      exit(1);
  }
}
void chip8cpuLaunch(char *rom_path)
{
  Chip8 chip8;
  chip8.esc = 0;
  memset(chip8.V, 0, sizeof(chip8.V));
  chip8.pc = 0x200;
  chip8.I = 0;
  chip8.sp = 0;
  chip8timersInit(&chip8.delay_timer, &chip8.sound_timer);
  memset(chip8.stack, 0, sizeof(chip8.stack));
  memset(chip8.memoria, 0, sizeof(chip8.memoria));
  chip8inputInitTeclado(&chip8.teclado[0], &chip8.esc);
  chip8displayInitPantalla(&chip8.pantalla[0]);
  chip8opcodesInit(&chip8);

  FILE *rom = fopen(rom_path, "rb");

  if (rom == NULL)
  {
    perror("No se pudo abrir el archivo");
    exit(1);
  }
  fread(&chip8.memoria[0x200], 1, 4096 - 0x200, rom);
  fclose(rom);

  do
  {
    SDL_Delay(selectedDelay); // Usa el valor configurado en settings

    uint16_t opcode = (chip8.memoria[chip8.pc] << 8) | chip8.memoria[chip8.pc + 1];
    chip8.pc += 2;

    chip8displayPrintPantalla();
    chip8timersDecrement();
    chip8inputCapturarTeclado();
    chip8opcodesEvaluate(opcode);
  } while (!chip8.esc);

  chip8displayDestroyPantalla();
  // chip8inputDestroyTeclado(&chip8.teclado[0]);
  // chip8timersDestroy(&chip8.delay_timer, &chip8.sound_timer);
}