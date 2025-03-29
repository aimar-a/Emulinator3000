
#include "nes_config.h"  // Incluir el archivo de configuración

#define MAX_LINE_LENGTH 256


void leer_configuracion(const char *archivoConfig, const char *prefix) {
    FILE *archivo = fopen(archivoConfig, "r");
    if (!archivo) {
        printf("No se pudo abrir el archivo de configuración: %s\n", archivoConfig);
        return;
    }

    char linea[MAX_LINE_LENGTH];
    
    while (fgets(linea, MAX_LINE_LENGTH, archivo)) {
        // Eliminar salto de línea
        linea[strcspn(linea, "\n")] = '\0';

        // Verificar si la línea comienza con el prefijo adecuado
        if (strncmp(linea, prefix, strlen(prefix)) == 0) {
            // Obtener la parte de la variable (después del "prefix_")
            if (strncmp(linea, "SCREEN_WIDTH", 12) == 0) {
                SCREEN_WIDTH = atoi(linea + strlen(prefix) + 1); // +1 para saltar el "="
            } else if (strncmp(linea, "SCREEN_HEIGHT", 13) == 0) {
                SCREEN_HEIGHT = atoi(linea + strlen(prefix) + 1);
            } else if (strncmp(linea, "SCREEN_SCALE", 12) == 0) {
                SCREEN_SCALE = atoi(linea + strlen(prefix) + 1);
            }
        }
    }

    fclose(archivo);
}

