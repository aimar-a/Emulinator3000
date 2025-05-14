#include "config_file.hpp"

int SCREEN_WIDTH_CHIP8 = 64;
int SCREEN_HEIGHT_CHIP8 = 32;
int SCREEN_SCALE_CHIP8 = 26;

int SCREEN_WIDTH_SUPERCHIP = 128;
int SCREEN_HEIGHT_SUPERCHIP = 64;
int SCREEN_SCALE_SUPERCHIP = 13;

int SCREEN_WIDTH_NES = 256;
int SCREEN_HEIGHT_NES = 240;
int SCREEN_SCALE_NES = 4;

bool load = false;
bool cleanbbdd = false;
bool createbbdd = false;
bool deletebbdd = true;

bool log_chip8_enabled = true; // Variable para habilitar/deshabilitar el log de Chip8
bool log_nes_enabled = true;   // Variable para habilitar/deshabilitar el log de NES

// Variable para almacenar la ruta de la base de datos

// Función para cargar la configuración desde el archivo .txt
void cargarConfiguracion(const char *archivoConfig)
{
  // printf("Intentando abrir el archivo: %s\n", archivoConfig);

  FILE *archivo = fopen(archivoConfig, "r");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración: %s\n", archivoConfig);
    return;
  }

  char linea[256];
  while (fgets(linea, sizeof(linea), archivo))
  {
    // Comentar líneas con #
    if (linea[0] == '#')
      continue;

    // Eliminar salto de línea al final de la línea
    linea[strcspn(linea, "\n")] = 0;

    // Leer configuraciones para Chip8
    if (strstr(linea, "SCREEN_WIDTH_CHIP") != NULL)
    {
      sscanf(linea, "SCREEN_WIDTH_CHIP=%d", &SCREEN_WIDTH_CHIP8);
    }
    else if (strstr(linea, "SCREEN_HEIGHT_CHIP") != NULL)
    {
      sscanf(linea, "SCREEN_HEIGHT_CHIP=%d", &SCREEN_HEIGHT_CHIP8);
    }
    else if (strstr(linea, "SCREEN_SCALE_CHIP") != NULL)
    {
      sscanf(linea, "SCREEN_SCALE_CHIP=%d", &SCREEN_SCALE_CHIP8);
    }

    // Leer configuraciones para SuperChip8
    else if (strstr(linea, "SCREEN_WIDTH_SUPERCHIP") != NULL)
    {
      sscanf(linea, "SCREEN_WIDTH_SUPERCHIP=%d", &SCREEN_WIDTH_SUPERCHIP);
    }
    else if (strstr(linea, "SCREEN_HEIGHT_SUPERCHIP") != NULL)
    {
      sscanf(linea, "SCREEN_HEIGHT_SUPERCHIP=%d", &SCREEN_HEIGHT_SUPERCHIP);
    }
    else if (strstr(linea, "SCREEN_SCALE_SUPERCHIP") != NULL)
    {
      sscanf(linea, "SCREEN_SCALE_SUPERCHIP=%d", &SCREEN_SCALE_SUPERCHIP);
    }

    // Leer configuraciones para NES
    else if (strstr(linea, "SCREEN_WIDTH_NES") != NULL)
    {
      sscanf(linea, "SCREEN_WIDTH_NES=%d", &SCREEN_WIDTH_NES);
    }
    else if (strstr(linea, "SCREEN_HEIGHT_NES") != NULL)
    {
      sscanf(linea, "SCREEN_HEIGHT_NES=%d", &SCREEN_HEIGHT_NES);
    }
    else if (strstr(linea, "SCREEN_SCALE_NES") != NULL)
    {
      sscanf(linea, "SCREEN_SCALE_NES=%d", &SCREEN_SCALE_NES);
    }

    // Leer las configuraciones booleanas
    else if (strstr(linea, "load") != NULL)
    {
      if (strstr(linea, "true") != NULL)
      {
        load = true;
      }
      else
      {
        load = false;
      }
    }
    else if (strstr(linea, "cleanbbdd") != NULL)
    {
      if (strstr(linea, "true") != NULL)
      {
        cleanbbdd = true;
      }
      else
      {
        cleanbbdd = false;
      }
    }
    else if (strstr(linea, "createbbdd") != NULL)
    {
      if (strstr(linea, "true") != NULL)
      {
        createbbdd = true;
      }
      else
      {
        createbbdd = false;
      }
    }
    else if (strstr(linea, "deletebbdd") != NULL)
    {
      if (strstr(linea, "true") != NULL)
      {
        deletebbdd = true;
      }
      else
      {
        deletebbdd = false;
      }
    }
    else if (strstr(linea, "log_chip8") != NULL)
    {
      if (strstr(linea, "true") != NULL)
      {
        log_chip8_enabled = true;
      }
      else
      {
        log_chip8_enabled = false;
      }
    }
    else if (strstr(linea, "log_nes") != NULL)
    {
      if (strstr(linea, "true") != NULL)
      {
        log_nes_enabled = true;
      }
      else
      {
        log_nes_enabled = false;
      }
    }
  }

  fclose(archivo);
}

#include <stdio.h>
#include <string.h>

int cambiarEscalaChip8(int nuevaEscala)
{
  const char *rutaConfig = "resources/config/config";

  FILE *archivo = fopen(rutaConfig, "r");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para leer: %s\n", rutaConfig);
    return 1;
  }

  // Leer todo el archivo en memoria
  char lineas[1024][256]; // Suponemos un máximo de 1024 líneas
  int numLineas = 0;
  int encontrado = 0;

  while (fgets(lineas[numLineas], sizeof(lineas[numLineas]), archivo))
  {
    if (strstr(lineas[numLineas], "SCREEN_SCALE_CHIP") != NULL)
    {
      sprintf(lineas[numLineas], "SCREEN_SCALE_CHIP=%d\n", nuevaEscala);
      encontrado = 1;
    }
    numLineas++;
  }
  fclose(archivo);

  if (!encontrado)
  {
    printf("ERROR: No se encontró la línea SCREEN_SCALE_CHIP en el archivo de configuración.\n");
    return 1;
  }

  // Reescribir el archivo con las modificaciones
  archivo = fopen(rutaConfig, "w");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para escribir: %s\n", rutaConfig);
    return 1;
  }

  for (int i = 0; i < numLineas; i++)
  {
    fputs(lineas[i], archivo);
  }

  fclose(archivo);
  SCREEN_SCALE_CHIP8 = nuevaEscala; // Actualizar la variable global
  return 0;
}

int cambiarEscalaSuperChip(int nuevaEscala)
{
  const char *rutaConfig = "resources/config/config";

  FILE *archivo = fopen(rutaConfig, "r");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para leer: %s\n", rutaConfig);
    return 1;
  }

  // Leer todo el archivo en memoria
  char lineas[1024][256]; // Suponemos un máximo de 1024 líneas
  int numLineas = 0;
  int encontrado = 0;

  while (fgets(lineas[numLineas], sizeof(lineas[numLineas]), archivo))
  {
    if (strstr(lineas[numLineas], "SCREEN_SCALE_SUPERCHIP") != NULL)
    {
      sprintf(lineas[numLineas], "SCREEN_SCALE_SUPERCHIP=%d\n", nuevaEscala);
      encontrado = 1;
    }
    numLineas++;
  }
  fclose(archivo);

  if (!encontrado)
  {
    printf("ERROR: No se encontró la línea SCREEN_SCALE_SUPERCHIP en el archivo de configuración.\n");
    return 1;
  }

  // Reescribir el archivo con las modificaciones
  archivo = fopen(rutaConfig, "w");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para escribir: %s\n", rutaConfig);
    return 1;
  }
  for (int i = 0; i < numLineas; i++)
  {
    fputs(lineas[i], archivo);
  }
  fclose(archivo);
  SCREEN_SCALE_SUPERCHIP = nuevaEscala; // Actualizar la variable global
  return 0;
}

int cambiarEscalaNes(int nuevaEscala)
{
  const char *rutaConfig = "resources/config/config";

  FILE *archivo = fopen(rutaConfig, "r");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para leer: %s\n", rutaConfig);
    return 1;
  }

  // Leer todo el archivo en memoria
  char lineas[1024][256]; // Suponemos un máximo de 1024 líneas
  int numLineas = 0;
  int encontrado = 0;

  while (fgets(lineas[numLineas], sizeof(lineas[numLineas]), archivo))
  {
    if (strstr(lineas[numLineas], "SCREEN_SCALE_NES") != NULL)
    {
      sprintf(lineas[numLineas], "SCREEN_SCALE_NES=%d\n", nuevaEscala);
      encontrado = 1;
    }
    numLineas++;
  }
  fclose(archivo);

  if (!encontrado)
  {
    printf("ERROR: No se encontró la línea SCREEN_SCALE_NES en el archivo de configuración.\n");
    return 1;
  }

  // Reescribir el archivo con las modificaciones
  archivo = fopen(rutaConfig, "w");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para escribir: %s\n", rutaConfig);
    return 1;
  }
  for (int i = 0; i < numLineas; i++)
  {
    fputs(lineas[i], archivo);
  }
  fclose(archivo);
  SCREEN_SCALE_NES = nuevaEscala; // Actualizar la variable global
  return 0;
}