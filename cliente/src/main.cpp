#include <iostream>
#include "menu_cmd.hpp"
#include "cliente.hpp"
#include "config_file.hpp"

int main(int argc, char *argv[])
{
  // Cargar configuraciones
  cargarConfiguracion();

  client_run();
  return 0;
}
