#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>

#include "chip8_cpu.h"

#include "menu_cmd.h"

#define ROMS_PATH "resources/chip8-roms"

void menuInicial()
{
  int option = -1;
  while (option != 0)
  {
    system("cls");
    printf("Bienvenido al emulador EMULINATOR 3000\n");
    printf("Seleccione la consola que desea emular:\n");
    printf("1. CHIP-8\n");
    printf("2. Game Boy\n");
    printf("3. NES\n");
    printf("0. Salir\n");
    printf("Opción: ");

    scanf("%d", &option);

    switch (option)
    {
    case 1:
      menuChip8();
      break;
    case 2:
      printf("Emulando Game Boy...\n");
      break;
    case 3:
      printf("Emulando NES...\n");
      break;
    case 0:
      printf("Saliendo...\n");
      break;
    default:
      printf("Opción inválida\n");
      break;
    }
  }
}

void menuChip8()
{
  int option = -1;
  while (option != 0)
  {
    system("cls");
    printf("CHIP-8\n");
    printf("Seleccione una opción:\n");
    printf("1. Cargar ROM\n");
    printf("2. Configuración\n");
    printf("0. Volver\n");
    printf("Opción: ");

    scanf("%d", &option);

    switch (option)
    {
    case 1:
      menuListaROMs();
      break;
    case 2:
      printf("Configurando...\n");
      break;
    case 0:
      printf("Volviendo...\n");
      break;
    default:
      printf("Opción inválida\n");
      break;
    }
  }
}

void listarROMsRecursivo(const char *directory, char roms[][256], int *count)
{
  struct dirent *entry;
  DIR *dp = opendir(directory);

  if (dp == NULL)
  {
    perror("Error al abrir el directorio");
    return;
  }
  while ((entry = readdir(dp)) != NULL)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

    struct stat path_stat;
    if (stat(path, &path_stat) == 0)
    {
      if (S_ISREG(path_stat.st_mode))
      { // Archivo regular
        const char *ext = strrchr(entry->d_name, '.');
        if (ext != NULL && strcmp(ext, ".ch8") == 0)
        {
          snprintf(roms[*count], 256, "%s", path);
          (*count)++;
        }
      }
      else if (S_ISDIR(path_stat.st_mode))
      { // Directorio
        listarROMsRecursivo(path, roms, count);
      }
    }
  }
  closedir(dp);
}

void menuListaROMs()
{
  char roms[200][256];
  int romCount = 0;

  printf("Listado de ROMs disponibles:\n");
  listarROMsRecursivo(ROMS_PATH, roms, &romCount);

  if (romCount == 0)
  {
    printf("No se encontraron ROMs.\n");
    return;
  }

  int pagina = 0;
  int totalPaginas = (romCount + 19) / 20;
  int opcion;

  do
  {
    system("cls");
    printf("Listado de ROMs (Página %d de %d):\n", pagina + 1, totalPaginas);

    for (int i = pagina * 20; i < (pagina + 1) * 20 && i < romCount; i++)
    {
      printf("%d. %s\n", i + 1, roms[i]);
    }

    printf("\nOpciones:\n");
    printf("-1. Página anterior\n");
    printf("-2. Página siguiente\n");
    printf("0. Salir\n");
    printf("Seleccione un ROM para ejecutar (1-%d) o navegar (-1, -2): ", romCount);
    scanf("%d", &opcion);

    if (opcion == -1 && pagina > 0)
    {
      pagina--;
    }
    else if (opcion == -2 && pagina < totalPaginas - 1)
    {
      pagina++;
    }
    else if (opcion > 0 && opcion <= romCount)
    {
      chip8cpuLaunch(roms[opcion - 1]);
      return;
    }
  } while (opcion != 0);
}
