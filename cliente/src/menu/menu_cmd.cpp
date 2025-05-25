#include "menu_cmd.hpp"
#include <string.h>

void clearScreen()
{
  printf("----------------------------------------\n");
#ifdef _WIN32
  system("cls");
#else
  system("clear");
#endif
}

char getOption()
{
  char option;
  char buffer[10]; // Buffer para almacenar la entrada del usuario

  // Leer la entrada del usuario
  if (fgets(buffer, sizeof(buffer), stdin) != NULL)
  {
    // Verificar si es un solo caracter valido (sin contar '\n')
    if (buffer[0] != '\n' && buffer[1] == '\n')
    {
      return buffer[0];
    }
  }

  return ' '; // Devolver espacio si la entrada es invalida
}

void getString(char *str, int maxLen)
{
  if (fgets(str, maxLen, stdin) != NULL)
  {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
      str[len - 1] = '\0'; // Reemplaza el salto de linea con terminador nulo
    }
  }
  else
  {
    str[0] = '\0'; // Si no se lee nada, establece la cadena como vacia
  }
}

char *currentUser = NULL;

void menuUsuario(socket_t sock)
{
  // opciones:
  //   1.Iniciar sesion --> Lleva a otro menu donde poner Usuario y contrasenya
  //   2.Regsitrar --> Lleva a otro menu donde poner NUEVOUSUARIO y NUEVACONTRASEÑA

  // 4.En MenuInicial meter opcion de cambiar Contrasenya
  // 5. el SALIR  de menuInicial tiene que ir a menuUsuario

  while (1)
  {
    clearScreen();
    printf("Seleccione una de las siguientes opciones\n");
    printf("1. INICIAR SESION: \n");
    printf("2. REGISTRARSE: \n");
    printf("0. Salir\n");
    printf("Opcion: ");

    char option = getOption();

    if (option == '1')
    {
      clearScreen();
      printf("--- Iniciar Sesion ---\n");
      printf("Introduce tu Usuario: ");
      char usuario[MAX_STRING_LENGTH];
      getString(usuario, MAX_STRING_LENGTH);

      printf("\nIntroduce tu Contrasenya: ");
      char contra[MAX_STRING_LENGTH];
      getString(contra, MAX_STRING_LENGTH);

      char option_socket = 0x01;
      net::send_data(sock, &option_socket, sizeof(option_socket));
      net::send_data(sock, usuario, strlen(usuario) + 1);
      net::send_data(sock, contra, strlen(contra) + 1);

      char response[4];
      net::receive_data(sock, response, sizeof(response)); // TODO creo q no va

      if (strcmp(response, "ACK") == 0)
      {
        currentUser = strdup(usuario);
        menuInicial(sock);
      }
      else
      {
        printf("Usuario o contrasenya incorrectos\n");
        Sleep(1000);
      }
    }
    else if (option == '2')
    {
      clearScreen();
      printf("--- Registro de Usuario ---\n");
      printf("\nUsuario: ");
      char usuario[MAX_STRING_LENGTH];
      getString(usuario, MAX_STRING_LENGTH);

      printf("\nContrasenya: ");
      char contra[MAX_STRING_LENGTH];
      getString(contra, MAX_STRING_LENGTH);

      // implementacion BD (insertar usuario)
      if (usuario[0] != '\0' && contra[0] != '\0make')
      {
        char option_socket = 0x02;
        net::send_data(sock, &option_socket, sizeof(option_socket));
        net::send_data(sock, usuario, strlen(usuario) + 1);
        net::send_data(sock, contra, strlen(contra) + 1);

        char response[4];
        net::receive_data(sock, response, sizeof(response));

        if (strcmp(response, "ACK") == 0)
        {
          currentUser = strdup(usuario); // Asignar el nombre de usuario a la variable global
          printf("Usuario registrado correctamente\n");
          Sleep(1000);
          menuInicial(sock);
        }
        else
        {
          printf("El usuario ya existe\n");
          Sleep(1000);
        }
      }
      else
      {
        printf("No se pueden guardar como usuario o contrasenya valores vacios\n");
        Sleep(500);
        printf("Volviendo al menu....\n");
        Sleep(1000);
      }
    }
    else if (option == '0')
    {
      char option_socket = 0x00;
      net::send_data(sock, &option_socket, sizeof(option_socket));
      printf("Cerrando....\n");
      Sleep(1000);
      break;
    }
    else
    {
      printf("Opcion invalida\n");
      Sleep(1000);
      printf("Volviendo al menu....\n");
      Sleep(1000);
    }
  }
}

// esto no funciona porque hay que meter el codigo en un bucle
void menuCambioContraseña(socket_t sock)
{
  clearScreen();
  printf("--- Cambio de Contraseña ---\n");
  printf("Estas con el usuario: %s\n\n", currentUser);

  printf("Introduce tu contrasenya actual: "); // Para afianzar de que es el usuario el que quiere cambiarlo

  char contraActual[MAX_STRING_LENGTH];
  getString(contraActual, MAX_STRING_LENGTH);

  uint8_t option = 0x01;
  net::send_data(sock, &option, sizeof(option));
  net::send_data(sock, contraActual, strlen(contraActual) + 1);
  char response[4];
  net::receive_data(sock, response, sizeof(response));
  printf("Respuesta del servidor: %s\n", response);
  if (strcmp(response, "ACK") == 0)
  {
    printf("Contrasenya correcta\n\n");
    printf("Introduce tu nueva contrasenya: ");
    char nuevaContra[MAX_STRING_LENGTH];
    getString(nuevaContra, MAX_STRING_LENGTH);
    net::send_data(sock, nuevaContra, strlen(nuevaContra) + 1);
    char response[4];
    net::receive_data(sock, response, sizeof(response));
    if (strcmp(response, "ACK") == 0)
    {
      printf("Contrasenya cambiada correctamente\n");
      Sleep(1000);
      return;
    }
    else
    {
      printf("Error al cambiar la contrasenya\n");
      Sleep(1000);
      return;
    }
  }
  else
  {
    printf("Contrasenya incorrecta, fuera de aqui impostor!!\n");
    Sleep(1000);
    return;
  }
}

void menuInicial(socket_t sock)
{
  char option = ' ';

  while (option != '0')
  {
    option = ' ';
    clearScreen();
    printf("--------- EMULINATOR 3000 ---------\n");
    printf("Bienvenido %s!\n", currentUser);
    printf("Seleccione la opcion deseada:\n");
    printf("\n-- Emular Consola --\n");
    printf("1. CHIP-8\n");
    printf("2. NES\n");
    printf("\np. Perfil de usuario\n");
    printf("c. Configuracion\n");
    printf("\n0. Salir\n");
    printf("Opcion: ");

    option = getOption();
    uint8_t opcion_socket = 0x00;

    switch (option)
    {
    case '1':
      opcion_socket = 0x02; // Recibir ROMs CHIP8
      net::send_data(sock, &opcion_socket, sizeof(opcion_socket));
      showInitialWindow(sock); // a veces da core dump error, si no se soluciona para la entrega usar el otro
      //  menuListaROMs();
      break;
    case '2':
      opcion_socket = 0x03; // Recibir ROMs NES
      net::send_data(sock, &opcion_socket, sizeof(opcion_socket));
      menuListaROMsNES(sock);
      // menuAdvertenciaNES();
      break;
    case 'p':
    case 'P':
      menuPerfil(sock);
      break;
    case 'c':
    case 'C':
      menuConfiguracion(sock);
      break;
    case '0':
      currentUser = NULL; // Limpiar el nombre de usuario
      opcion_socket = 0x00;
      net::send_data(sock, &opcion_socket, sizeof(opcion_socket));
      printf("Volviendo al menu anterior....\n");
      Sleep(1000);
      break;
    default:
      printf("Opcion invalida\n");
      Sleep(1000);
      printf("Volviendo al menu....\n");
      Sleep(1000);
      break;
    }
  }
}

void menuConfiguracion(socket_t sock)
{
  char option = ' ';

  while (option != '0')
  {
    option = ' ';
    clearScreen();
    printf("--- Configuracion ---\n");
    printf("Seleccione una opcion:\n");
    printf("1. Cambiar Contrasenya\n");
    printf("2. Cambiar Escala de Pantalla Chip8\n");
    printf("3. Cambiar Escala de Pantalla SuperChip8\n");
    printf("4. Cambiar Escala de Pantalla NES\n");
    printf("0. Volver\n");
    printf("Opcion: ");

    option = getOption();

    switch (option)
    {
    case '1':
      menuCambioContraseña(sock);
      break;
    case '2':
      menuEscalaChip8();
      break;
    case '3':
      menuEscalaSuperChip8();
      break;
    case '4':
      menuEscalaNes();
      break;
    case '0':
      printf("Volviendo...\n");
      break;
    default:
      printf("Opcion invalida\n");
      break;
    }
  }
}

void menuEscalaChip8()
{
  while (true)
  {
    clearScreen();
    printf("--- Cambiar Escala de Pantalla Chip8 ---\n");
    printf("Escala actual: x%d\n", SCREEN_SCALE_CHIP8);
    printf("0. Volver\n");
    printf("Nueva escala: ");

    char input[MAX_STRING_LENGTH];
    getString(input, MAX_STRING_LENGTH);
    if (strcmp(input, "0") == 0)
    {
      printf("Volviendo...\n");
      break;
    }
    if (input[0] == 'x')
    {
      input[0] = ' ';
    }
    int nuevaEscala = atoi(input);
    if (nuevaEscala > 0)
    {
      if (cambiarEscalaChip8(nuevaEscala) == 1)
      {
        printf("ERROR: No se pudo cambiar la escala de pantalla Chip8\n");
        Sleep(1000);
        return;
      }
      printf("Escala de pantalla Chip8 cambiada a x%d\n", nuevaEscala);
      Sleep(1000);
      return;
    }
    else
    {
      printf("Escala invalida. Intentalo de nuevo.\n");
      Sleep(1000);
    }
  }
}

void menuEscalaSuperChip8()
{
  while (true)
  {
    clearScreen();
    printf("--- Cambiar Escala de Pantalla SuperChip8 ---\n");
    printf("Escala actual: x%d\n", SCREEN_SCALE_SUPERCHIP);
    printf("0. Volver\n");
    printf("Nueva escala: ");

    char input[MAX_STRING_LENGTH];
    getString(input, MAX_STRING_LENGTH);
    if (strcmp(input, "0") == 0)
    {
      printf("Volviendo...\n");
      break;
    }
    if (input[0] == 'x')
    {
      input[0] = ' ';
    }
    int nuevaEscala = atoi(input);
    if (nuevaEscala > 0)
    {
      if (cambiarEscalaSuperChip(nuevaEscala) == 1)
      {
        printf("ERROR: No se pudo cambiar la escala de pantalla SuperChip8\n");
        Sleep(1000);
        return;
      }
      printf("Escala de pantalla SuperChip8 cambiada a x%d\n", nuevaEscala);
      Sleep(1000);
      return;
    }
    else
    {
      printf("Escala invalida. Intentalo de nuevo.\n");
      Sleep(1000);
    }
  }
}

void menuEscalaNes()
{
  while (true)
  {
    clearScreen();
    printf("--- Cambiar Escala de Pantalla NES ---\n");
    printf("Escala actual: x%d\n", SCREEN_SCALE_NES);
    printf("0. Volver\n");
    printf("Nueva escala: ");

    char input[MAX_STRING_LENGTH];
    getString(input, MAX_STRING_LENGTH);
    if (strcmp(input, "0") == 0)
    {
      printf("Volviendo...\n");
      break;
    }
    if (input[0] == 'x')
    {
      input[0] = ' ';
    }
    int nuevaEscala = atoi(input);
    if (nuevaEscala > 0)
    {
      if (cambiarEscalaNes(nuevaEscala) == 1)
      {
        printf("ERROR: No se pudo cambiar la escala de pantalla NES\n");
        Sleep(1000);
        return;
      }
      printf("Escala de pantalla NES cambiada a x%d\n", nuevaEscala);
      Sleep(1000);
      return;
    }
    else
    {
      printf("Escala invalida. Intentalo de nuevo.\n");
      Sleep(1000);
    }
  }
}

void menuAdvertenciaNES()
{
  char option = ' ';

  while (option != 'n')
  {
    option = ' ';
    clearScreen();
    printf("ADVERTENCIA: EMULADOR NES AUN EN DESARROLLO\n");
    printf("Contiene errores y no es funcional al 100%%\n");
    printf("Quieres continuar de todas formas?\n");
    printf("s. Si\n");
    printf("n. No\n");
    printf("Opcion: ");

    option = getOption();

    switch (option)
    {
    case 's':
    case 'S':
      printf("Cargando emulador NES...\n");
      Sleep(1000);
      // nes_launch();
    case 'n':
    case 'N':
      printf("Volviendo al menu principal...\n");
      Sleep(1000);
      option = 'n';
      break;
    default:
      printf("Opcion invalida\n");
      Sleep(1000);
      break;
    }
  }
}

// Ya no se usa
void menuChip8(socket_t sock)
{
  char option = ' ';
  while (option != '0')
  {
    option = ' ';
    clearScreen();
    printf("CHIP-8\n");
    printf("Seleccione una opcion:\n");
    printf("1. Cargar ROM\n");
    printf("2. Configuracion\n");
    printf("0. Volver\n");
    printf("Opcion: ");

    option = getOption();

    switch (option)
    {
    case '1':
      menuListaROMs(sock);
      break;
    case '2':
      printf("Configurando...\n");
      break;
    case '0':
      printf("Volviendo...\n");
      break;
    default:
      printf("Opcion invalida\n");
      break;
    }
  }
}

void listarROMsRecursivo(const char *directory, char roms[][256], int *count)
{
  struct dirent *entry;
  DIR *dp = opendir(directory);

  if (dp == NULL)
  {
    perror("Error al abrir el directorio");
    return;
  }
  while ((entry = readdir(dp)) != NULL)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

    struct stat path_stat;
    if (stat(path, &path_stat) == 0)
    {
      if (S_ISREG(path_stat.st_mode))
      { // Archivo regular
        const char *ext = strrchr(entry->d_name, '.');
        if (ext != NULL && strcmp(ext, ".ch8") == 0)
        {
          snprintf(roms[*count], 256, "%s", path);
          (*count)++;
        }
      }
      else if (S_ISDIR(path_stat.st_mode))
      { // Directorio
        listarROMsRecursivo(path, roms, count);
      }
    }
  }
  closedir(dp);
}

void menuListaROMs(socket_t sock)
{
  char roms[200][256];
  int romCount = 0;

  printf("Listado de ROMs disponibles:\n");
  // listarROMsRecursivo(ROMS_PATH, roms, &romCount);

  if (romCount == 0)
  {
    printf("No se encontraron ROMs.\n");
    return;
  }

  int pagina = 0;
  int totalPaginas = (romCount + 19) / 20;

  while (1)
  {
    clearScreen();
    printf("Listado de ROMs (Pagina %d de %d):\n", pagina + 1, totalPaginas);

    for (int i = pagina * 20; i < (pagina + 1) * 20 && i < romCount; i++)
    {
      printf("%d. %s\n", i + 1, roms[i]);
    }

    printf("\nOpciones:\n");
    printf("<. Pagina anterior\n");
    printf(">. Pagina siguiente\n");
    printf("0. Salir\n");
    printf("Seleccione un ROM para ejecutar (1-%d) o navegar (<, >): ", romCount);

    char opcion[MAX_STRING_LENGTH];
    getString(opcion, MAX_STRING_LENGTH);

    if (opcion[1] == '\0')
    {
      // Si solo se ingreso un caracter
      if (opcion[0] == '<' && pagina > 0)
      {
        pagina--;
        continue;
      }
      else if (opcion[0] == '>' && pagina < totalPaginas - 1)
      {
        pagina++;
        continue;
      }
      else if (opcion[0] == '0')
      {
        printf("Saliendo...\n");
        Sleep(1000);
        return;
      }
      else if (opcion[0] >= '1' && opcion[0] <= '9')
      {
        int seleccion = opcion[0] - '0';

        // chip8cpuLaunch(roms[seleccion - 1]);
        return;
      }
    }
    else
    {
      // Si se ingreso mas de un caracter, ver si puede ser un numero de una rom
      int seleccion = atoi(opcion);
      if (seleccion >= 1 && seleccion <= romCount)
      {
        // chip8cpuLaunch(roms[seleccion - 1]);
        return;
      }
    }
    // Si no se ingreso una opcion valida
    printf("Opcion invalida\n");
    Sleep(1000);
    printf("Volviendo al menu....\n");
    Sleep(1000);
  }
}

void extraerNombreROM(const char *entrada, char *nombre)
{
  // Encontrar el primer espacio después del número
  const char *ruta = entrada;

  // Buscar la última aparición de '/'
  const char *ultimoSlash = strrchr(ruta, '/');
  if (ultimoSlash)
  {
    ruta = ultimoSlash + 1; // Tomar solo el nombre del archivo
  }

  // Copiar el nombre con extensión
  strcpy(nombre, ruta);

  // Buscar la última aparición de '.' para quitar la extensión
  char *punto = strrchr(nombre, '.');
  if (punto)
  {
    *punto = '\0'; // Eliminar la extensión
  }
}

// ns si ya existia una funcion para listas los de la nes, porsiacaso la he creado aqui:

void listarROMsRecursivoNES(const char *directory, char roms[][256], int *count)
{
  struct dirent *entry;
  DIR *dp = opendir(directory);

  if (dp == NULL)
  {
    perror("Error al abrir el directorio");
    return;
  }
  while ((entry = readdir(dp)) != NULL)
  {
    if (strcmp(entry->d_name, ".") == 0 || strcmp(entry->d_name, "..") == 0)
      continue;

    char path[512];
    snprintf(path, sizeof(path), "%s/%s", directory, entry->d_name);

    struct stat path_stat;
    if (stat(path, &path_stat) == 0)
    {
      if (S_ISREG(path_stat.st_mode))
      { // Archivo regular
        const char *ext = strrchr(entry->d_name, '.');
        if (ext != NULL && strcmp(ext, ".nes") == 0)
        {
          snprintf(roms[*count], 256, "%s", path);
          (*count)++;
        }
      }
      else if (S_ISDIR(path_stat.st_mode))
      { // Directorio
        listarROMsRecursivo(path, roms, count);
      }
    }
  }
  closedir(dp);
}

void menuPerfil(socket_t sock)
{
  char option = ' ';
  uint8_t opcion_socket = 0x00;
  while (option != '0')
  {
    option = ' ';
    clearScreen();
    printf("PERFIL DE USUARIO\n");
    printf("Seleccione una opcion:\n");
    printf("1. Ver Logros\n");
    printf("2. Ver Tiempo Jugado\n");
    printf("0. Volver\n");
    printf("Opcion: ");

    option = getOption();

    switch (option)
    {
    case '1':
      opcion_socket = 0x11; // Ver logros
      net::send_data(sock, &opcion_socket, sizeof(opcion_socket));
      menuLogros(sock);
      break;
    case '3':
      opcion_socket = 0x12; // Ver Amigos
      net::send_data(sock, &opcion_socket, sizeof(opcion_socket));
      // menuVerAmigos(sock); // Da un segmentation fault lo corrijo mañana
      printf("Ver Amigos no implementado\n");
      Sleep(1000);
      printf("Volviendo al menu....\n");
      Sleep(1000);
      break;
    case '2':
      opcion_socket = 0x13; // Ver Tiempo jugado
      net::send_data(sock, &opcion_socket, sizeof(opcion_socket));
      menuVerTiempoJugado(sock);
      break;
    case '0':
      printf("Volviendo...\n");
      break;
    default:
      printf("Opcion invalida\n");
      break;
    }
  }
}

void menuLogros(socket_t sock)
{
  char **nombreLogros = NULL;
  char **descripcionLogros = NULL;
  char **fechaObtencion = NULL;
  int cantidadLogros = 0;

  // recibimos la cantidad del servidor
  net::receive_data(sock, &cantidadLogros, sizeof(cantidadLogros));

  // recibimos la lista de nombres de logros
  nombreLogros = (char **)malloc(cantidadLogros * sizeof(char *));
  for (int i = 0; i < cantidadLogros; i++)
  {
    nombreLogros[i] = (char *)malloc(MAX_STRING_LENGTH);
    net::receive_data(sock, nombreLogros[i], MAX_STRING_LENGTH);
  }

  // recibimos la lista de descripciones de logros
  descripcionLogros = (char **)malloc(cantidadLogros * sizeof(char *));
  for (int i = 0; i < cantidadLogros; i++)
  {
    descripcionLogros[i] = (char *)malloc(MAX_STRING_LENGTH);
    net::receive_data(sock, descripcionLogros[i], MAX_STRING_LENGTH);
  }

  // recibimos la lista de fechas de obtencion
  fechaObtencion = (char **)malloc(cantidadLogros * sizeof(char *));
  for (int i = 0; i < cantidadLogros; i++)
  {
    fechaObtencion[i] = (char *)malloc(MAX_STRING_LENGTH);
    net::receive_data(sock, fechaObtencion[i], MAX_STRING_LENGTH);
  }

  while (1)
  {
    clearScreen();
    printf("--- Logros ---\n");
    printf("CantidadLogros: %i\n", cantidadLogros);

    if (cantidadLogros == -1)
    {
      printf("Error al obtener los logros\n");
      Sleep(1000);
      return;
    }
    if (cantidadLogros == 0)
    {
      printf("No hay logros\n");
      Sleep(1000);
      return;
    }
    printf("Logros conseguidos por %s:\n", currentUser);
    for (int i = 0; i < cantidadLogros; i++)
    {
      printf("%d. - %s", i + 1, nombreLogros[i]);
      int espacios = 50 - strlen(nombreLogros[i]);
      if (espacios < 0)
        espacios = 0;
      for (int j = 0; j < espacios; j++)
      {
        printf(" ");
      }
      printf("Fecha de obtencion: %s\n", fechaObtencion[i]);
    }

    printf("\nNumero del logro para ver detalles\n");

    printf("0. Volver\n");
    printf("Seleccione una opcion: ");
    char opcion[MAX_STRING_LENGTH];
    getString(opcion, MAX_STRING_LENGTH);
    if (strcmp(opcion, "0") == 0)
    {
      printf("Volviendo...\n");
      Sleep(1000);
      break; // Salir del bucle de logros
    }

    // Si se ingreso un numero valido, mostrar los detalles del logro
    int seleccion = atoi(opcion);
    if (seleccion >= 1 && seleccion <= cantidadLogros)
    {
      while (1)
      {
        clearScreen();
        printf("Detalles del logro %d:\n", seleccion);
        printf("Nombre: %s\n", nombreLogros[seleccion - 1]);
        printf("Descripcion: %s\n", descripcionLogros[seleccion - 1]);
        printf("Fecha de obtencion: %s\n", fechaObtencion[seleccion - 1]);
        printf("\n0. Volver\n");
        printf("Seleccione una opcion: ");
        char opcionDetalles;
        opcionDetalles = getOption();
        if (opcionDetalles == '0')
        {
          printf("Volviendo...\n");
          Sleep(1000);
          break; // Salir del bucle de detalles
        }
        else
        {
          printf("Opcion invalida\n");
          Sleep(1000);
        }
      }
    }
  }
  // Liberar memoria
  for (int i = 0; i < cantidadLogros; i++)
  {
    free(nombreLogros[i]);
    free(descripcionLogros[i]);
    free(fechaObtencion[i]);
  }
  free(nombreLogros);
  free(descripcionLogros);
  free(fechaObtencion);
  return;
}

void menuVerTiempoJugado(socket_t sock) // cambiar el metodo para que coja sock
{
  char **nombreJuegos = NULL;
  int *tiemposSegundos;
  int cantidadJuegos = 0; // getTiempoJugadoTodosLosJuegos(user, &nombreJuegos, &tiemposSegundos);

  // recibimos la cantidad del servidor
  net::receive_data(sock, &cantidadJuegos, sizeof(cantidadJuegos));

  // recibimos la lista de nombres de juegos
  nombreJuegos = (char **)malloc(cantidadJuegos * sizeof(char *));
  for (int i = 0; i < cantidadJuegos; i++)
  {
    nombreJuegos[i] = (char *)malloc(MAX_STRING_LENGTH);
    net::receive_data(sock, nombreJuegos[i], MAX_STRING_LENGTH);
  }

  // recibimos la lista de tiempos jugados
  tiemposSegundos = (int *)malloc(cantidadJuegos * sizeof(int));
  for (int i = 0; i < cantidadJuegos; i++)
  {
    net::receive_data(sock, &tiemposSegundos[i], sizeof(int));
  }

  while (1)
  {
    clearScreen();
    printf("--- Tiempo Jugado ---\n");
    printf("CantidadJuegos: %i\n", cantidadJuegos);

    if (cantidadJuegos == -1)
    {
      printf("Error al obtener los tiempos jugados\n");
      return;
    }
    if (cantidadJuegos == 0)
    {
      printf("No se han jugado juegos\n");
      return;
    }
    printf("Juegos jugados por %s:\n", currentUser);
    for (int i = 0; i < cantidadJuegos; i++)
    {
      int horas = tiemposSegundos[i] / 3600;
      int minutos = (tiemposSegundos[i] % 3600) / 60;
      int segundos = tiemposSegundos[i] % 60;
      printf("%d. - %s", i + 1, nombreJuegos[i]);
      int espacios = 50 - strlen(nombreJuegos[i]);
      if (espacios < 0)
        espacios = 0;
      for (int j = 0; j < espacios; j++)
      {
        printf(" ");
      }
      printf("%d horas      %d minutos      %d segundos\n", horas, minutos, segundos);
    }

        printf("0. Volver\n");
    printf("Seleccione una opcion: ");
    char opcion[MAX_STRING_LENGTH];
    getString(opcion, MAX_STRING_LENGTH);
    if (strcmp(opcion, "0") == 0)
    {
      printf("Volviendo...\n");
      Sleep(1000);
      // Liberar memoria
      for (int i = 0; i < cantidadJuegos; i++)
      {
        free(nombreJuegos[i]);
      }
      free(nombreJuegos);
      free(tiemposSegundos);
      return;
    }

    // Si se ingreso un numero valido, mostrar las partidas jugadas
    int seleccion = atoi(opcion);
    if (seleccion >= 1 && seleccion <= cantidadJuegos)
    {
      char *nombreJuego = nombreJuegos[seleccion - 1];
      mostrarPartidasJugadas(sock, nombreJuego);
    }
    else
    {
      printf("Opcion invalida\n");
      Sleep(1000);
    }
    printf("Volviendo al menu....\n");
    Sleep(1000);
  }
  // Liberar memoria
  for (int i = 0; i < cantidadJuegos; i++)
  {
    free(nombreJuegos[i]);
  }
  free(nombreJuegos);
  free(tiemposSegundos);
}

void mostrarPartidasJugadas(socket_t sock, char *nombreJuego)
{
  uint8_t opcion_socket = 0x14; // Ver partidas jugadas
  net::send_data(sock, &opcion_socket, sizeof(opcion_socket));
  net::send_data(sock, nombreJuego, strlen(nombreJuego) + 1);

  char **partidas = NULL;
  int *tiemposJugados = NULL;
  int *puntuacionesMaximas = NULL;
  int cantidadPartidas = 0;

  // Obtener las partidas jugadas
  net::receive_data(sock, &cantidadPartidas, sizeof(cantidadPartidas));

  // Recibir la lista de partidas
  partidas = (char **)malloc(cantidadPartidas * sizeof(char *));
  for (int i = 0; i < cantidadPartidas; i++)
  {
    partidas[i] = (char *)malloc(MAX_STRING_LENGTH);
    net::receive_data(sock, partidas[i], MAX_STRING_LENGTH);
  }
  // Recibir la lista de tiempos jugados
  tiemposJugados = (int *)malloc(cantidadPartidas * sizeof(int));
  for (int i = 0; i < cantidadPartidas; i++)
  {
    net::receive_data(sock, &tiemposJugados[i], sizeof(int));
  }
  // Recibir la lista de puntuaciones maximas
  puntuacionesMaximas = (int *)malloc(cantidadPartidas * sizeof(int));
  for (int i = 0; i < cantidadPartidas; i++)
  {
    net::receive_data(sock, &puntuacionesMaximas[i], sizeof(int));
  }

  while (1)
  {
    clearScreen();
    printf("--- Partidas Jugadas ---\n");
    // getPartidasDeJuego(user, nombreJuego, &partidas, &tiemposJugados, &puntuacionesMaximas);
    if (cantidadPartidas == -1)
    {
      printf("Error al obtener las partidas jugadas\n");
      return;
    }
    if (cantidadPartidas == 0)
    {
      printf("No se han jugado partidas de \"%s\"\n", nombreJuego);
      return;
    }
    printf("Partidas jugadas de \"%s\":\n", nombreJuego);
    for (int i = 0; i < cantidadPartidas; i++)
    {
      printf("%d. - Partida: %s", i + 1, partidas[i]);
      int horas = tiemposJugados[i] / 3600;
      int minutos = (tiemposJugados[i] % 3600) / 60;
      int segundos = tiemposJugados[i] % 60;
      printf("     Tiempo Jugado: %d horas %d minutos %d segundos\n", horas, minutos, segundos);
      printf("     Puntuacion Maxima: %d\n", puntuacionesMaximas[i]);
    }

    printf("\n0. Volver\n");
    printf("Seleccione una opcion: ");
    char opcion[MAX_STRING_LENGTH];
    getString(opcion, MAX_STRING_LENGTH);
    if (strcmp(opcion, "0") == 0)
    {
      return;
    }
    else
    {
      printf("Opcion invalida\n");
      Sleep(1000);
    }
    printf("Volviendo al menu....\n");
    Sleep(1000);
  }
  // Liberar memoria
  for (int i = 0; i < cantidadPartidas; i++)
  {
    free(partidas[i]);
  }
  free(partidas);
  free(tiemposJugados);
  free(puntuacionesMaximas);
}

void menuVerAmigos(socket_t sock) // cambiar el metodo para que coja sock
{
  char **nombreAmigos = NULL;
  int *cantidadAmigos = 0;

  // recibimos la cantidad del servidor
  net::receive_data(sock, &cantidadAmigos, sizeof(cantidadAmigos));

  // recibimos la lista de nombres de juegos
  nombreAmigos = (char **)malloc(*cantidadAmigos * sizeof(char *));
  for (int i = 0; i < *cantidadAmigos; i++)
  {
    nombreAmigos[i] = (char *)malloc(MAX_STRING_LENGTH);
    net::receive_data(sock, nombreAmigos[i], MAX_STRING_LENGTH);
  }

  while (1)
  {
    clearScreen();
    printf("--- Amigos ---\n");
    printf("Cantidad de Amigos: %i\n", cantidadAmigos);

    if (*cantidadAmigos == -1)
    {
      printf("Error al obtener los amigos\n");
      return;
    }
    if (*cantidadAmigos == 0)
    {
      printf("No tienes amigos\n");
      return;
    }
    printf("Amigos de %s:\n", currentUser);
    for (int i = 0; i < *cantidadAmigos; i++)
    {

      printf("%d. - %s \n", i + 1, nombreAmigos[i]);
    }

    // printf("\nNumero del amigo para ver sus datos\n");

    printf("0. Volver\n");
    printf("Seleccione una opcion: ");
    char opcion[MAX_STRING_LENGTH];
    getString(opcion, MAX_STRING_LENGTH);
    if (strcmp(opcion, "0") == 0)
    {
      printf("Volviendo...\n");
      Sleep(1000);
      // Liberar memoria
      for (int i = 0; i < *cantidadAmigos; i++)
      {
        free(nombreAmigos[i]);
      }
      free(nombreAmigos);
      return;
    }

    // Si se ingreso un numero valido, mostrar las partidas jugadas
    /* int seleccion = atoi(opcion);
    if (seleccion >= 1 && seleccion <= cantidadJuegos)
    {
      char *nombreJuego = nombreJuegos[seleccion - 1];
      mostrarPartidasJugadas(sock, nombreJuego);
    }
    else
    {
      printf("Opcion invalida\n");
      Sleep(1000);
    }
    printf("Volviendo al menu....\n");
    Sleep(1000);
  }
  // Liberar memoria
  for (int i = 0; i < cantidadJuegos; i++)
  {
    free(nombreJuegos[i]);
  }
  free(nombreJuegos);
    free(tiemposSegundos); */
  }
}

void menuListaROMsNES(socket_t sock)
{
  char roms[200][256];
  int romCount = 0;

  printf("Listado de ROMs disponibles:\n");
  net::receive_data(sock, &romCount, sizeof(romCount));
  // Recibir la lista de ROMs
  for (int i = 0; i < romCount; i++)
  {
    net::receive_data(sock, roms[i], sizeof(roms[i]));
  }

  if (romCount == 0)
  {
    printf("No se encontraron ROMs.\n");
    return;
  }

  int pagina = 0;
  int totalPaginas = (romCount + 19) / 20;

  while (1)
  {
    clearScreen();
    printf("Listado de ROMs NES (Pagina %d de %d):\n", pagina + 1, totalPaginas);

    for (int i = pagina * 20; i < (pagina + 1) * 20 && i < romCount; i++)
    {
      printf("%d. %s\n", i + 1, roms[i]);
    }

    printf("\nOpciones:\n");
    printf("<. Pagina anterior\n");
    printf(">. Pagina siguiente\n");
    printf("0. Salir\n");
    printf("Seleccione un ROM para ejecutar (1-%d) o navegar (<, >): ", romCount);

    char opcion[MAX_STRING_LENGTH];
    getString(opcion, MAX_STRING_LENGTH);

    if (opcion[1] == '\0')
    {
      // Si solo se ingreso un caracter
      if (opcion[0] == '<' && pagina > 0)
      {
        pagina--;
        continue;
      }
      else if (opcion[0] == '>' && pagina < totalPaginas - 1)
      {
        pagina++;
        continue;
      }
      else if (opcion[0] == '0')
      {
        printf("Saliendo...\n");
        Sleep(1000);
        return;
      }
      else if (opcion[0] >= '1' && opcion[0] <= '9')
      {
        int seleccion = opcion[0] - '0';
        uint8_t opcion_socket = 0xE1; // Emular NES
        net::send_data(sock, &opcion_socket, sizeof(opcion_socket));
        net::send_data(sock, roms[seleccion - 1], strlen(roms[seleccion - 1]) + 1);
        printf("Cargando ROM: %s\n", roms[seleccion - 1]);
        emulate_nes(sock);
        return;
      }
    }
    else
    {
      // Si se ingreso mas de un caracter, ver si puede ser un numero de una rom
      int seleccion = atoi(opcion);
      if (seleccion >= 1 && seleccion <= romCount)
      {
        uint8_t opcion_socket = 0xE1; // Emular NES
        net::send_data(sock, &opcion_socket, sizeof(opcion_socket));
        net::send_data(sock, roms[seleccion - 1], strlen(roms[seleccion - 1]) + 1);
        printf("Cargando ROM: %s\n", roms[seleccion - 1]);
        emulate_nes(sock);
        return;
      }
    }
    // Si no se ingreso una opcion valida
    printf("Opcion invalida\n");
    Sleep(1000);
    printf("Volviendo al menu....\n");
    Sleep(1000);
  }
  // Liberar memoria
  for (int i = 0; i < romCount; i++)
  {
    // free(roms[i]); // No es necesario liberar cada rom, ya que son strings dentro de un array estático
  }
  // free(roms);
  return;
}
