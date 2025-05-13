#include "cliente.hpp"
#include "menu_cmd.hpp" // Incluir el archivo del menú

// Implementación de la función principal del cliente
void client_run()
{
  net::ConnectionConfig config;
  config.host = SERVER_IP; // Dirección IP del servidor
  config.port = PORT;      // Puerto del servidor

  socket_t sock = net::connect(config); // Establecer conexión
  if (sock == INVALID_SOCKET_VALUE)
  {
    std::cerr << "Error al conectar al servidor: " << WSAGetLastError() << std::endl;
    return;
  }

  printf("Conectado al servidor\n");

  menuUsuario(sock);

  return;
}