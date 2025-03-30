#include "menu_cmd.h"

void clearScreen()
{
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
    // Verificar si es un solo caracter válido (sin contar '\n')
    if (buffer[0] != '\n' && buffer[1] == '\n')
    {
      return buffer[0];
    }
  }

  return ' '; // Devolver espacio si la entrada es inválida
}

void getString(char *str, int maxLen)
{
  if (fgets(str, maxLen, stdin) != NULL)
  {
    size_t len = strlen(str);
    if (len > 0 && str[len - 1] == '\n')
    {
      str[len - 1] = '\0'; // Reemplaza el salto de línea con terminador nulo
    }
  }
  else
  {
    str[0] = '\0'; // Si no se lee nada, establece la cadena como vacía
  }
}

char *currentUser = NULL;

void menuUsuario()
{
  // opciones:
  //   1.Iniciar sesion --> Lleva a otro menu donde poner Usuario y contraseña
  //   2.Regsitrar --> Lleva a otro menu donde poner NUEVOUSUARIO y NUEVACONTRASEÑA

  // 4.En MenuInicial meter opcion de cambiar Contraseña
  // 5. el SALIR  de menuInicial tiene que ir a menuUsuario

  bool pass = false;

  while (pass == false)
  {
    clearScreen();
    printf("Seleccione una de las siguientes opciones\n");
    printf("1. INICIAR SESIÓN: \n");
    printf("2. REGISTRARSE: \n");
    printf("0. Salir\n");
    printf("Opción: ");

    char option = getOption();

    if (option == '1')
    {
      clearScreen();
      printf("--- Iniciar Sesión ---\n");
      printf("Introduce tu Usuario: ");
      char usuario[MAX_STRING_LENGTH];
      getString(usuario, MAX_STRING_LENGTH);

      printf("\nIntroduce tu Contraseña: ");
      char contra[MAX_STRING_LENGTH];
      getString(contra, MAX_STRING_LENGTH);

      // implementacion BD (comprobar si existe un usuario con esa contraseña)
      if (existeUsuarioYPas(usuario, contra) == true)
      {
        pass = true;
        printf("Accediendo.....\n");
        currentUser = usuario;
        Sleep(1000);
        menuInicial();
        break;
      }
      else
      {
        printf("Usuario o contraseña incorrectos\n");
        Sleep(400);
        printf("Volviendo al menu....\n");
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

      printf("\nContraseña: ");
      char contra[MAX_STRING_LENGTH];
      getString(contra, MAX_STRING_LENGTH);

      // implementacion BD (insertar usuario)
      if (usuario[0] != '\0' && contra[0] != '\0make')
      {
        if (existeUsuario(usuario) != true)
        {
          currentUser = usuario;
          insertarUsuarios(usuario, contra);
          printf("Registro Exitoso\n");
          Sleep(1000);
          menuInicial();
          break;
        }
        else
        {
          printf("Ese nombre de usuario ya existe, prueba a poner otro nombre\n");
          Sleep(500);
          printf("Volviendo al menu....\n");
          Sleep(1000);
        }
      }
      else
      {
        printf("No se pueden guardar como usuario o contraseña valores vacios\n");
        Sleep(500);
        printf("Volviendo al menu....\n");
        Sleep(1000);
      }
    }
    else if (option == '0')
    {

      printf("Cerrando....\n");
      Sleep(1000);
      break;
    }
    else
    {
      printf("Opción inválida\n");
      Sleep(1000);
      printf("Volviendo al menu....\n");
      Sleep(1000);
    }
  }
}

// esto no funciona porque hay que meter el codigo en un bucle
void menuCambioContraseña()
{
  clearScreen();
  printf("--- Cambio de Contraseña ---\n");
  printf("Estas con el usuario: %s\n\n", currentUser);

  printf("Introduce tu contraseña actual: "); // Para afianzar de que es el usuario el que quiere cambiarlo

  char contraActual[MAX_STRING_LENGTH];
  getString(contraActual, MAX_STRING_LENGTH);

  if (comprobarContraseña(currentUser, contraActual) == true)
  {
    printf("Contraseña correcta\n\n");
    while (1)
    {
      printf("Introduzca nueva contraseña: ");

      char contraNueva1[MAX_STRING_LENGTH];
      getString(contraNueva1, MAX_STRING_LENGTH);

      printf("Vuelva a introducir la nueva contraseña: ");

      char contraNueva2[MAX_STRING_LENGTH];
      getString(contraNueva2, MAX_STRING_LENGTH);

      // Comprobar que las contraseñas coinciden
      if (strcmp(contraNueva1, contraNueva2) != 0)
      {
        printf("Las contraseñas no coinciden. Inténtalo de nuevo.\n\n");
        Sleep(1000);
        continue;
      }

      updateContrasena(contraNueva1, currentUser);
      printf("Cambio de Contraseña Exitoso....\n");
      Sleep(1000);
      return;
    }
  }
  printf("Contraseña incorrecta, fuera de aqui impostor!!\n");
  Sleep(1000);
  return;
}

void menuInicial()
{
  char option = ' ';

  while (option != '0')
  {
    option = ' ';
    clearScreen();
    printf("--------- EMULINATOR 3000 ---------\n");
    printf("Bienvenido %s!\n", currentUser);
    printf("Seleccione la opción deseada:\n");
    printf("\n-- Emular Consola --\n");
    printf("1. CHIP-8\n");
    printf("2. NES\n");
    printf("\nc. Configuración\n");
    printf("\n0. Salir\n");
    printf("Opción: ");

    option = getOption();

    switch (option)
    {
    case '1':
      showInitialWindow(); // a veces da core dump error, si no se soluciona para la entrega usar el otro
      // menuListaROMs();
      break;
    case '2':
      menuAdvertenciaNES();
      break;
    case 'c':
    case 'C':
      menuConfiguracion();
      break;
    case '0':
      printf("Volviendo al menu anterior....\n");
      Sleep(1000);
      menuUsuario();
      break;
    default:
      printf("Opción inválida\n");
      Sleep(1000);
      printf("Volviendo al menu....\n");
      Sleep(1000);
      break;
    }
  }
}

void menuConfiguracion()
{
  char option = ' ';

  while (option != '0')
  {
    option = ' ';
    clearScreen();
    printf("--- Configuración ---\n");
    printf("Seleccione una opción:\n");
    printf("1. Cambiar Contraseña\n");
    printf("2. Cambiar Escala de Pantalla Chip8\n");
    printf("3. Cambiar Escala de Pantalla SuperChip8\n");
    printf("4. Cambiar Escala de Pantalla NES\n");
    printf("0. Volver\n");
    printf("Opción: ");

    option = getOption();

    switch (option)
    {
    case '1':
      menuCambioContraseña();
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
      printf("Opción inválida\n");
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
      printf("Escala inválida. Inténtalo de nuevo.\n");
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
      printf("Escala inválida. Inténtalo de nuevo.\n");
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
      printf("Escala inválida. Inténtalo de nuevo.\n");
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
    printf("Opción: ");

    option = getOption();

    switch (option)
    {
    case 's':
    case 'S':
      printf("Cargando emulador NES...\n");
      Sleep(1000);
      nes_launch();
    case 'n':
    case 'N':
      printf("Volviendo al menu principal...\n");
      Sleep(1000);
      option = 'n';
      break;
    default:
      printf("Opción inválida\n");
      Sleep(1000);
      break;
    }
  }
}

// Ya no se usa
void menuChip8()
{
  char option = ' ';
  while (option != '0')
  {
    option = ' ';
    clearScreen();
    printf("CHIP-8\n");
    printf("Seleccione una opción:\n");
    printf("1. Cargar ROM\n");
    printf("2. Configuración\n");
    printf("0. Volver\n");
    printf("Opción: ");

    option = getOption();

    switch (option)
    {
    case '1':
      menuListaROMs();
      break;
    case '2':
      printf("Configurando...\n");
      break;
    case '0':
      printf("Volviendo...\n");
      break;
    default:
      printf("Opción inválida\n");
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

void menuListaROMs()
{
  char roms[200][256];
  int romCount = 0;

  printf("Listado de ROMs disponibles:\n");
  listarROMsRecursivo(ROMS_PATH, roms, &romCount);

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
    printf("Listado de ROMs (Página %d de %d):\n", pagina + 1, totalPaginas);

    for (int i = pagina * 20; i < (pagina + 1) * 20 && i < romCount; i++)
    {
      printf("%d. %s\n", i + 1, roms[i]);
    }

    printf("\nOpciones:\n");
    printf("<. Página anterior\n");
    printf(">. Página siguiente\n");
    printf("0. Salir\n");
    printf("Seleccione un ROM para ejecutar (1-%d) o navegar (<, >): ", romCount);

    char opcion[MAX_STRING_LENGTH];
    getString(opcion, MAX_STRING_LENGTH);

    if (opcion[1] == '\0')
    {
      // Si solo se ingresó un carácter
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

        chip8cpuLaunch(roms[seleccion - 1]);
        return;
      }
    }
    else
    {
      // Si se ingreso mas de un caracter, ver si puede ser un numero de una rom
      int seleccion = atoi(opcion);
      if (seleccion >= 1 && seleccion <= romCount)
      {
        chip8cpuLaunch(roms[seleccion - 1]);
        return;
      }
    }
    // Si no se ingresó una opción válida
    printf("Opción inválida\n");
    Sleep(1000);
    printf("Volviendo al menu....\n");
    Sleep(1000);
  }
}
