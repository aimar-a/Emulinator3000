#include "chip8_config.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

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
bool deletebbdd = false;

// Variable para almacenar la ruta de la base de datos

// Función para cargar la configuración desde el archivo .txt
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
    }

    fclose(archivo);
}