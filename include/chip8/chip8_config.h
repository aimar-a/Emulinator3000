
#ifndef CONFIG_H
#define CONFIG_H

extern int SCREEN_WIDTH;
extern int SCREEN_HEIGHT;
extern int SCREEN_SCALE;

extern int SCREEN_WIDTH_SUPERCHIP;
extern int SCREEN_HEIGHT_SUPERCHIP;
extern int SCREEN_SCALE_SUPERCHIP;

void cargarConfiguracion(const char *archivoConfig);

#endif // CONFIG_H
