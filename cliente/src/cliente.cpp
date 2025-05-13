#include "cliente.h"

void client_run()
{
  WSADATA wsa;
  SOCKET sock;
  struct sockaddr_in server;

  // Inicializar Winsock
  if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
  {
    printf("WSAStartup failed: %d\n", WSAGetLastError());
    return;
  }

  // Crear y conectar socket (igual que antes)
  sock = socket(AF_INET, SOCK_STREAM, 0);
  server.sin_family = AF_INET;
  server.sin_addr.s_addr = inet_addr(SERVER_IP);
  server.sin_port = htons(PORT);

  if (connect(sock, (struct sockaddr *)&server, sizeof(server)) < 0)
  {
    printf("Connect failed: %d\n", WSAGetLastError());
    closesocket(sock);
    return;
  }
  printf("Conectado al servidor\n");

  menuUsuario(sock);

  closesocket(sock);
  WSACleanup();
  return;
}