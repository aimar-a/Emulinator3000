#include "chip8_cpu.h"
#include <time.h>
#include "bd.h"
#include <stdio.h>
#include "menu_cmd.h"
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
    exit(1);
  }

  if (chip8->pantalla == NULL)
  {
    chip8_log("ERROR: No se pudo asignar memoria para pantalla\n");
    exit(1);
  }
}


void chip8cpuLaunch(char *rom_path)
{
  chip8_log_clear();
  chip8_log("INFO: Inicializando CPU\n");

  Chip8 chip8;
  chip8.esc = 0;
  memset(chip8.V, 0, sizeof(chip8.V));
  chip8.pc = 0x200;
  chip8.I = 0;
  chip8.sp = 0;
  time_t tiempoInicio, tiempoFin;


  time(&tiempoInicio); //guardamos el tiempo actual (cuando se ha empezado a jugar) en tiempoInicio
  
  chip8_log("INFO: Inicializando memoria y configuración de la pantalla\n");
  inicializarMemoria(&chip8, modosuperchip8);

  chip8_log("INFO: Inicializando temporizadores\n");
  chip8timersInit(&chip8.delay_timer, &chip8.sound_timer);
  memset(chip8.stack, 0, sizeof(chip8.stack));
  memset(chip8.memoria, 0, sizeof(chip8.memoria));
  chip8inputInitTeclado(&chip8.teclado[0], &chip8.esc);
  chip8displayInitPantalla(&chip8.pantalla[0]);
  chip8opcodesInit(&chip8);

  FILE *rom = fopen(rom_path, "rb");

  if (rom == NULL)
  {
    chip8_log("ERROR: No se pudo abrir el archivo ROM\n");
    exit(1);
  }
  chip8_log("INFO: Cargando ROM en memoria\n");
  fread(&chip8.memoria[0x200], 1, 4096 - 0x200, rom);
  fclose(rom);

  chip8_log("INFO: Iniciando ciclo de ejecución\n");


  do
  {
    SDL_Delay(selectedDelay);

    uint16_t opcode = (chip8.memoria[chip8.pc] << 8) | chip8.memoria[chip8.pc + 1];
    chip8.pc += 2;

    chip8displayPrintPantalla();
    chip8timersDecrement();
    chip8inputCapturarTeclado();
    chip8opcodesEvaluate(opcode);
    
  } while (!chip8.esc);

  //BD INSERTAR PARTIDA 
  time(&tiempoFin); //guardamos el tiempo actual (cuando se ha empezado a jugar) en tiempoInicio
  int tiempoJugado =(int) difftime(tiempoFin,tiempoInicio); 



  //llamamos a la funcion getIdJuego para conseguir la id del juego que estamos jugando apartir de su ROM
   int id = getIdJuego(rom_path);


  if(hajugado(currentUser, id)==true){

    //hacemos el update sumando el tiempo que ya habia jugado + el que acaba de jugar
    int tiempojugadoanterior = getTiempoJugado(currentUser, id);
    int tiempoTotal= tiempojugadoanterior + tiempoJugado;
    updateTiempoJugado(tiempoTotal, currentUser, id);   //SI ha jugado a este juego hacemos update



  }else{

    insertarTiempoJugado(tiempoJugado, currentUser,id);   //si el usuario NO ha jugado a este juego hacemos insert. 


  }


  //insertamos los datos de la partida
  //void insertarPartida(char* user, int idjuego, int tiempojugado, int puntmax){

  insertarPartida(currentUser, id, tiempoJugado, tiempoJugado*0.5); // la puntuacion maxima vamos a dejarla asi

  //updatear Juego puntuacion y usuario record si se ha hecho un nuevo record   
  int puntuacionPartida = tiempoJugado*0.5;
  if(puntuacionPartida>getPuntuacionRecord(id)){

    updateUsuarioPuntuacionRecord(currentUser, puntuacionPartida, id);


  }
  
  



  chip8_log("INFO: Finalizando ejecución y liberando recursos\n");
  chip8displayDestroyPantalla();
  // chip8inputDestroyTeclado(&chip8.teclado[0]);
  // chip8timersDestroy(&chip8.delay_timer, &chip8.sound_timer);
}