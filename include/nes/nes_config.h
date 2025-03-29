#ifndef NES_CONFIG_H
#define NES_CONFIG_H
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// Declaración de las variables globales
extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int SCREEN_SCALE;

// Declaración de la función para leer el archivo de configuración
void leer_configuracion(const char *archivoConfig, const char *prefix);

#endif // NES_CONFIG_H
