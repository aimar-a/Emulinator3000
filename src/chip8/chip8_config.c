// config.c
#include "chip8_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int SCREEN_WIDTH = 64; // valor predeterminado
int SCREEN_HEIGHT = 32; // valor predeterminado
int SCREEN_SCALE = 26; // valor predeterminado

int SCREEN_WIDTH_SUPERCHIP = 128;
int SCREEN_HEIGHT_SUPERCHIP = 64;
int SCREEN_SCALE_SUPERCHIP = 13;

void cargarConfiguracion(const char *archivoConfig)
{
    printf("Intentando abrir el archivo: %s\n", archivoConfig);

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
            sscanf(linea, "SCREEN_WIDTH_CHIP=%d", &SCREEN_WIDTH);
        }
        else if (strstr(linea, "SCREEN_HEIGHT_CHIP") != NULL)
        {
            sscanf(linea, "SCREEN_HEIGHT_CHIP=%d", &SCREEN_HEIGHT);
        }
        else if (strstr(linea, "SCREEN_SCALE_CHIP") != NULL)
        {
            sscanf(linea, "SCREEN_SCALE_CHIP=%d", &SCREEN_SCALE);
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
    }

    fclose(archivo);
}
