#include "config_file.h"

int SCREEN_WIDTH_CHIP8 = 64;
int SCREEN_HEIGHT_CHIP8 = 32;

int SCREEN_WIDTH_SUPERCHIP = 128;
int SCREEN_HEIGHT_SUPERCHIP = 64;

int SCREEN_WIDTH_NES = 256;
int SCREEN_HEIGHT_NES = 240;

bool cleanbbdd = false;
bool createbbdd = false;
bool deletebbdd = false;
bool loadSampleData = false; // Variable para cargar datos de muestra
bool loadGames = false;      // Variable para cargar juegos

bool log_chip8_enabled = true; // Variable para habilitar/deshabilitar el log de Chip8
bool log_nes_enabled = true;   // Variable para habilitar/deshabilitar el log de NES

char *CONFIG_FILE = "resources/config/server.conf";

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

        // Leer configuraciones para SuperChip8
        else if (strstr(linea, "SCREEN_WIDTH_SUPERCHIP") != NULL)
        {
            sscanf(linea, "SCREEN_WIDTH_SUPERCHIP=%d", &SCREEN_WIDTH_SUPERCHIP);
        }
        else if (strstr(linea, "SCREEN_HEIGHT_SUPERCHIP") != NULL)
        {
            sscanf(linea, "SCREEN_HEIGHT_SUPERCHIP=%d", &SCREEN_HEIGHT_SUPERCHIP);
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

        // Leer las configuraciones booleanas
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
        else if (strstr(linea, "loadSampleData") != NULL)
        {
            if (strstr(linea, "true") != NULL)
            {
                loadSampleData = true;
            }
            else
            {
                loadSampleData = false;
            }
        }
        else if (strstr(linea, "loadGames") != NULL)
        {
            if (strstr(linea, "true") != NULL)
            {
                loadGames = true;
            }
            else
            {
                loadGames = false;
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