#include "logros.h"
#include <time.h>

char fecha[20];

void updatetimenow()
{
  time_t now = time(NULL);
  struct tm *tm_now = localtime(&now);
  strftime(fecha, 20, "%d/%m/%Y %H:%M:%S", tm_now);
}

void logroBienvenido(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 1, fecha);
  printf("Logro de bienvenida insertado para el usuario %s\n", user);
}

void logroChip8tester(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 2, fecha);
  printf("Logro de Chip8 tester insertado para el usuario %s\n", user);
}

void logroPingPong(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 3, fecha);
  printf("Logro de Ping Pong insertado para el usuario %s\n", user);
}

void logroJuntos(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 4, fecha);
  printf("Logro de Juntos insertado para el usuario %s\n", user);
}

void logroBuscandoLaSalida(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 5, fecha);
  printf("Logro de Buscando la salida insertado para el usuario %s\n", user);
}

void logroTetris(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 6, fecha);
  printf("Logro de Tetris insertado para el usuario %s\n", user);
}

void logroNesTester(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 7, fecha);
  printf("Logro de Nes tester insertado para el usuario %s\n", user);
}

void logroChip8Fan(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 8, fecha);
  printf("Logro de Chip8 Fan insertado para el usuario %s\n", user);
}

void logroChip8Pro(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 9, fecha);
  printf("Logro de Chip8 Pro insertado para el usuario %s\n", user);
}

void logroChip8Master(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 10, fecha);
  printf("Logro de Chip8 Master insertado para el usuario %s\n", user);
}

void logroNesFan(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 11, fecha);
  printf("Logro de Nes Fan insertado para el usuario %s\n", user);
}

void logroNesPro(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 12, fecha);
  printf("Logro de Nes Pro insertado para el usuario %s\n", user);
}

void logroNesMaster(char *user)
{
  updatetimenow();
  insertarLogrosUsuarios(user, 13, fecha);
  printf("Logro de Nes Master insertado para el usuario %s\n", user);
}