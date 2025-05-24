#include "chip8_cpu.h"
#include <time.h>
#include "bd.h"
#include <stdio.h>
#include <stdbool.h>

void inicializarMemoria(Chip8 *chip8, bool modosuperchip8)
{
  if (modosuperchip8)
  {
    chip8_log("INFO: Inicializando memoria para SuperChip8\n");
    chip8->memoria = (uint8_t *)malloc(131072); // 128KB
    chip8->pantalla = (uint8_t *)malloc(128 * 64 * sizeof(uint8_t));
  }
  else
  {
    chip8_log("INFO: Inicializando memoria para Chip8\n");
    chip8->memoria = (uint8_t *)malloc(4096); // 4KB
    chip8->pantalla = (uint8_t *)malloc(64 * 32 * sizeof(uint8_t));
  }

  if (chip8->memoria == NULL)
  {
    chip8_log("ERROR: No se pudo asignar memoria para memoria\n");
  }

  if (chip8->pantalla == NULL)
  {

    chip8_log("ERROR: No se pudo asignar memoria para pantalla\n");
  }
}

void chip8step(Chip8 *chip8)
{
  uint16_t opcode = (chip8->memoria[chip8->pc] << 8) | chip8->memoria[chip8->pc + 1];
  chip8->pc += 2;

  if (chip8->pantalla == NULL)
  {
    printf("NO PANTALLA");
  }
  // chip8displayPrintPantalla();
  chip8timersDecrement();
  chip8opcodesEvaluate(opcode);
}

void chip8terminate(Chip8 *chip8, char *currentUser)
{
  // BD INSERTAR PARTIDA
  time(&chip8->tiempoFin); // guardamos el tiempo actual (cuando se ha empezado a jugar) en tiempoInicio
  int tiempoJugado = (int)difftime(chip8->tiempoFin, chip8->tiempoInicio);

  // llamamos a la funcion getIdJuego para conseguir la id del juego que estamos jugando apartir de su ROM
  int id = getIdJuego(chip8->rom_path);

  if (hajugado(currentUser, id) == true)
  {

    // hacemos el update sumando el tiempo que ya habia jugado + el que acaba de jugar
    int tiempojugadoanterior = getTiempoJugado(currentUser, id);
    int tiempoTotal = tiempojugadoanterior + tiempoJugado;
    updateTiempoJugado(tiempoTotal, currentUser, id); // SI ha jugado a este juego hacemos update
  }
  else
  {

    insertarTiempoJugado(tiempoJugado, currentUser, id); // si el usuario NO ha jugado a este juego hacemos insert.
  }

  // insertamos los datos de la partida
  // void insertarPartida(char *user, int idjuego, int puntmax, char* fecha,char*fecha)

  if (chip8->rom_path != NULL)
    free(chip8->rom_path);
  if (chip8->memoria != NULL)
    free(chip8->memoria);
  if (chip8->pantalla != NULL)
    free(chip8->pantalla);
  struct tm *currentTimeInicio;
  struct tm *currentTimeFin;

  currentTimeInicio = localtime(&chip8->tiempoInicio);

  char fechaInicio[30];
  strftime(fechaInicio, sizeof(fechaInicio), "%d/%m/%Y %H:%M:%S", currentTimeInicio);

  currentTimeFin = localtime(&chip8->tiempoFin);

  char fechaFin[30];
  strftime(fechaFin, sizeof(fechaFin), "%d/%m/%Y %H:%M:%S", currentTimeFin);

  insertarPartida(currentUser, id, tiempoJugado * 0.5, fechaInicio, fechaFin); // la puntuacion maxima vamos a dejarla asi
  printf("Partida insertada");
  // updatear Juego puntuacion y usuario record si se ha hecho un nuevo record
  int puntuacionPartida = tiempoJugado * 0.5;
  if (puntuacionPartida > getPuntuacionRecord(id))
  {

    updateUsuarioPuntuacionRecord(currentUser, puntuacionPartida, id);
  }

  chip8_log("INFO: Finalizando ejecución y liberando recursos\n");
  // chip8displayDestroyPantalla();
  //  chip8inputDestroyTeclado(&chip8.teclado[0]);
  //  chip8timersDestroy(&chip8.delay_timer, &chip8.sound_timer);
}

Chip8 *chip8init(char *rom_path)
{
  chip8_log_clear();
  chip8_log("INFO: Inicializando CPU\n");

  // Asignar memoria dinámica para la estructura Chip8
  Chip8 *chip8 = (Chip8 *)malloc(sizeof(Chip8));

  if (chip8 == NULL)
  {
    chip8_log("ERROR: No se pudo asignar memoria para la estructura Chip8\n");
    return NULL;
  }

  chip8->esc = 0;
  memset(chip8->V, 0, sizeof(chip8->V));
  chip8->pc = 0x200;
  chip8->I = 0;
  chip8->sp = 0;

  chip8->rom_path = strdup(rom_path);

  time(&chip8->tiempoInicio); // Guardar el tiempo actual (inicio del juego)

  chip8_log("INFO: Inicializando memoria y configuración de la pantalla\n");
  inicializarMemoria(chip8, modosuperchip8);

  chip8displaySetPantalla(chip8->pantalla);

  chip8_log("INFO: Inicializando temporizadores\n");
  chip8timersInit(&chip8->delay_timer, &chip8->sound_timer);
  memset(chip8->stack, 0, sizeof(chip8->stack));
  memset(chip8->memoria, 0, sizeof(chip8->memoria));
  chip8inputInitTeclado(&chip8->teclado[0], &chip8->esc);
  chip8opcodesInit(chip8);

  chip8_log(rom_path);

  FILE *rom = fopen(rom_path, "rb");
  if (rom == NULL)
  {
    chip8_log("ERROR: No se pudo abrir el archivo ROM\n");
    free(chip8->memoria);
    free(chip8->pantalla);
    free(chip8->rom_path);
    free(chip8);
    return NULL;
  }

  chip8_log("INFO: Cargando ROM en memoria\n");
  fseek(rom, 0, SEEK_END);
  long rom_size = ftell(rom);
  rewind(rom);

  if (rom_size > (4096 - 0x200))
  {
    chip8_log("ERROR: La ROM es demasiado grande para la memoria Chip8 estándar\n");
    fclose(rom);
    free(chip8->memoria);
    free(chip8->pantalla);
    free(chip8);
    return NULL;
  }
  fread(&chip8->memoria[0x200], 1, 4096 - 0x200, rom);
  fclose(rom);

  chip8_log("INFO: Iniciando ciclo de ejecución\n");

  return chip8;
}