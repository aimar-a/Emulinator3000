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

char *CONFIG_FILE = "resources/config/client.conf";

// Función para cargar la configuración desde el archivo .txt
void cargarConfiguracion()
{
  // printf("Intentando abrir el archivo: %s\n", CONFIG_FILE);

  FILE *archivo = fopen(CONFIG_FILE, "r");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración: %s\n", CONFIG_FILE);
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
  }

  fclose(archivo);
}

int cambiarEscalaChip8(int nuevaEscala)
{
  FILE *archivo = fopen(CONFIG_FILE, "r");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para leer: %s\n", CONFIG_FILE);
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
  archivo = fopen(CONFIG_FILE, "w");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para escribir: %s\n", CONFIG_FILE);
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
  FILE *archivo = fopen(CONFIG_FILE, "r");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para leer: %s\n", CONFIG_FILE);
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
  archivo = fopen(CONFIG_FILE, "w");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para escribir: %s\n", CONFIG_FILE);
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
  FILE *archivo = fopen(CONFIG_FILE, "r");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para leer: %s\n", CONFIG_FILE);
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
  archivo = fopen(CONFIG_FILE, "w");
  if (archivo == NULL)
  {
    printf("ERROR: No se pudo abrir el archivo de configuración para escribir: %s\n", CONFIG_FILE);
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