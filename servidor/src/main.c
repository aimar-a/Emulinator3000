#include "bd.h"
#include "config_file.h"
#include "server.h"

int main(int argc, char *argv[])
{
  // Cargar la configuración desde el archivo
  cargarConfiguracion();

  // Inicializar la base de datos
  iniciarBaseDeDatos();

  // Iniciar el servidor
  server_run();

  return 0;
}