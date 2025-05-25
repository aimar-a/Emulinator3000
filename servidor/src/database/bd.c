#include <stdio.h>
#include <time.h>
#include "sqlite3.h"
#include <string.h>
#include "bd.h"

sqlite3 *db;
const char *db_filename = "resources/database/emulatorBD.sqlite";

const char *DIR_JUEGOS_CSV = "resources/sampleData/juegos.csv";
const char *DIR_USUARIOS_CSV = "resources/sampleData/usuarios.csv";
const char *DIR_AMIGOS_CSV = "resources/sampleData/amigos.csv";
const char *DIR_LOGROS_CSV = "resources/sampleData/logros.csv";
const char *DIR_PARTIDAS_CSV = "resources/sampleData/partidas.csv";
const char *DIR_TIEMPO_JUGADO_CSV = "resources/sampleData/tiempo_jugado.csv";
const char *DIR_LOGROS_USUARIO_CSV = "resources/sampleData/logros_usuario.csv";

int iniciarBaseDeDatos()
{
    // Ejecutar acciones según la configuración
    if (deletebbdd)
    {
        eliminarBaseDeDatos();
    }

    if (createbbdd)
    {
        crearBD();
    }
    if (cleanbbdd)
    {
        limpiarBaseDeDatos();
    }
    if (loadGames)
    {
        cargarJuegos();
    }
    if (loadSampleData)
    {
        cargarSampleData();
    }
}

int abrirBaseDeDatos(sqlite3 **db)
{
    // Abrimos la base de datos
    int rc = sqlite3_open(db_filename, db);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al abrir la base de datos: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    return SQLITE_OK;
}

// Función para crear la BD y su estructura
void crearBD()
{
    sqlite3_stmt *stmt;

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return;
    }

    // Creamos la tabla USUARIOS
    char sql1[] = "CREATE TABLE IF NOT EXISTS USUARIOS ("
                  "user VARCHAR(20) PRIMARY KEY,"
                  "contraseña VARCHAR(20) NOT NULL);";

    sqlite3_prepare_v2(db, sql1, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Creamos la tabla JUEGO
    char sql2[] = "CREATE TABLE IF NOT EXISTS JUEGO ("
                  "id_juego INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "titulo VARCHAR(100) NOT NULL,"
                  "rom VARCHAR(100) NOT NULL,"
                  "puntuacion_record INT DEFAULT 0,"
                  "usuario_record VARCHAR(20) DEFAULT '',"
                  "emulador VARCHAR(10) NOT NULL,"
                  "FOREIGN KEY (usuario_record) REFERENCES USUARIOS(user));";

    sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Creamos la tabla PARTIDA
    char sql3[] = "CREATE TABLE IF NOT EXISTS PARTIDA ("
                  "id_partida INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "user VARCHAR(20) NOT NULL,"
                  "fecha_inicio TIMESTAMP NOT NULL,"
                  "fecha_fin TIMESTAMP NOT NULL,"
                  "id_juego INTEGER NOT NULL,"
                  "puntuacion_maxima INT NOT NULL,"
                  "FOREIGN KEY (user) REFERENCES USUARIOS(user),"
                  "FOREIGN KEY (id_juego) REFERENCES JUEGO(id_juego));";

    sqlite3_prepare_v2(db, sql3, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Creamos la tabla TIEMPO_JUGADO
    char sql4[] = "CREATE TABLE IF NOT EXISTS TIEMPO_JUGADO ("
                  "tiempo_jugado INT DEFAULT 0,"
                  "user VARCHAR(20),"
                  "id_juego INTEGER,"
                  "PRIMARY KEY (user, id_juego),"
                  "FOREIGN KEY (user) REFERENCES USUARIOS(user),"
                  "FOREIGN KEY (id_juego) REFERENCES JUEGO(id_juego));";

    sqlite3_prepare_v2(db, sql4, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Creamos la tabla LOGROS
    char sql5[] = "CREATE TABLE IF NOT EXISTS LOGROS ("
                  "id_logro INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "nombre VARCHAR(20) NOT NULL,"
                  "descripcion TEXT NOT NULL);";

    sqlite3_prepare_v2(db, sql5, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Creamos la tabla LOGROS_USUARIO
    char sql6[] = "CREATE TABLE IF NOT EXISTS LOGROS_USUARIO ("
                  "user VARCHAR(20),"
                  "id_logro INTEGER,"
                  "fecha TIMESTAMP NOT NULL,"
                  "PRIMARY KEY (user, id_logro),"
                  "FOREIGN KEY (user) REFERENCES USUARIOS(user),"
                  "FOREIGN KEY (id_logro) REFERENCES LOGROS(id_logro));";

    sqlite3_prepare_v2(db, sql6, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Creamos la tabla AMIGOS
    char sql7[] = "CREATE TABLE IF NOT EXISTS AMIGOS ("
                  "user1 VARCHAR(20),"
                  "user2 VARCHAR(20),"
                  "estado TEXT CHECK(estado IN ('pendiente', 'aceptado', 'rechazado')) NOT NULL,"
                  "PRIMARY KEY (user1, user2),"
                  "FOREIGN KEY (user1) REFERENCES USUARIOS(user),"
                  "FOREIGN KEY (user2) REFERENCES USUARIOS(user));";

    sqlite3_prepare_v2(db, sql7, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Cerramos la base de datos
    sqlite3_close(db);
}

void limpiarBaseDeDatos()
{
    int rc = abrirBaseDeDatos(&db);
    if (rc != SQLITE_OK)
        return;

    // Lista de todas tus tablas
    const char *tablas[] = {
        "AMIGOS",
        "LOGROS_USUARIO",
        "LOGROS",
        "TIEMPO_JUGADO",
        "PARTIDA",
        "JUEGO",
        "USUARIOS",
        NULL // Marca el final del array
    };

    char *errMsg = NULL;
    const char **tabla = tablas;

    while (*tabla)
    {
        char sql[256];
        snprintf(sql, sizeof(sql), "DELETE FROM %s;", *tabla);

        rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
        if (rc != SQLITE_OK)
        {
            fprintf(stderr, "Error al limpiar la tabla %s: %s\n", *tabla, errMsg);
            sqlite3_free(errMsg);
        }
        else
        {
            printf("Tabla %s limpiada exitosamente.\n", *tabla);
        }
        tabla++;
    }

    // Reiniciamos los autoincrementales
    const char *resetAI = "DELETE FROM sqlite_sequence;";
    rc = sqlite3_exec(db, resetAI, 0, 0, &errMsg);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Error al resetear autoincrementales: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
    printf("Base de datos limpiada completamente.\n");
}

void eliminarBaseDeDatos()
{
    if (remove(db_filename) == 0)
    {
        printf("Base de datos eliminada exitosamente.\n");
    }
    else
    {
        perror("Error al eliminar la base de datos");
    }
}

void cargarJuegos()
{ // Al crear la bbdd aparte de crearla en si añadimos todos los juegos de nes y chip8 a la tabla "Juego"
    // no se hace la mejor gestion de memoria ya que en lugar de usar memoria dinamica (porq no sabemos el tamano exacto)
    // usamos memoria estatica por lo que estaremos usando mas memoria de la necesaria
    // mejorar esto para futuras entregas
    char romsCP8[200][MAX_ROM_PATH_LENGTH];
    int count = 0;
    listarROMsRecursivo("resources/chip8-roms", romsCP8, &count);

    // recorrer roms CHIP8 y en cada iteracion insertar en la BD
    for (int i = 0; i < count; i++)
    {
        // printf("%d. %s\n", i + 1, romsCP8[i]);

        char nombre[256];

        // Extraer el nombre
        extraerNombreROM(romsCP8[i], nombre); // esto guarda en nombre el "titulo" del juego

        // printf("%s\n", nombre);

        insertarJuego(nombre, romsCP8[i]); // por defecto al crear la Bd la puntuacion record y el usuario record sera 0;
    }

    // lo mismo pero para insertar los roms de la NES
    char romsNES[200][MAX_ROM_PATH_LENGTH];
    int count2 = 0;

    listarROMsRecursivoNES("resources/nes-roms", romsNES, &count2);

    // recorrer roms CHIP8 y en cada iteracion insertar en la BD
    for (int i = 0; i < count2; i++)
    {
        // printf("%d. %s\n", i + 1, romsNES[i]);

        char nombre2[256];

        // Extraer el nombre
        extraerNombreROM(romsNES[i], nombre2); // esto guarda en nombre el "titulo" del juego

        // printf("%s\n", nombre2);

        insertarJuego(nombre2, romsNES[i]); // por defecto al crear la Bd la puntuacion record y el usuario record sera 0;
    }
}

void cargarSampleData()
{
    // Cargar datos de los CSV
    cargarLogrosDeCSV(DIR_LOGROS_CSV);
    cargarUsuariosDeCSV(DIR_USUARIOS_CSV);
    cargarPartidasDeCSV(DIR_PARTIDAS_CSV);
    cargarTiempoJugadoDeCSV(DIR_TIEMPO_JUGADO_CSV);
    cargarLogrosUsuarioDeCSV(DIR_LOGROS_USUARIO_CSV);
    cargarAmigosDeCSV(DIR_AMIGOS_CSV);
}

void cargarLogrosDeCSV(char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo))
    {
        char nombre[50], descripcion[200];
        int idlogro;

        sscanf(linea, "%d,%[^,],%[^,]", &idlogro, nombre, descripcion);
        insertarLogros(idlogro, nombre, descripcion);
    }

    fclose(archivo);
}

void cargarUsuariosDeCSV(char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo))
    {
        char user[20], contraseña[20];
        sscanf(linea, "%[^,],%[^,]", user, contraseña);
        insertarUsuarios(user, contraseña);
    }

    fclose(archivo);
}

void cargarPartidasDeCSV(char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo))
    {
        char user[20], fechaInicio[30], fechaFin[30];
        int idjuego, puntmax;

        int camposLeidos = sscanf(linea, "%[^,],%[^,],%[^,],%d,%d", user, fechaInicio, fechaFin, &idjuego, &puntmax);
        if (camposLeidos == 6)
        {

            insertarPartida(user, idjuego, puntmax, fechaInicio, fechaFin);
        }
        else
        {
            printf("Error leyendo línea: %s\n", linea);
        }
    }

    fclose(archivo);
}

void cargarTiempoJugadoDeCSV(char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo))
    {
        char user[20];
        int idjuego, tiempojugado;

        sscanf(linea, "%[^,],%d,%d", user, &tiempojugado, &idjuego);
        insertarTiempoJugado(tiempojugado, user, idjuego);
    }

    fclose(archivo);
}

void cargarLogrosUsuarioDeCSV(char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo))
    {
        char user[20], fecha[20];
        int idlogro;

        sscanf(linea, "%[^,],%d,%[^,]", user, &idlogro, fecha);
        insertarLogrosUsuarios(user, idlogro, fecha);
    }

    fclose(archivo);
}

void cargarAmigosDeCSV(char *nombreArchivo)
{
    FILE *archivo = fopen(nombreArchivo, "r");
    if (archivo == NULL)
    {
        printf("Error al abrir el archivo %s\n", nombreArchivo);
        return;
    }

    char linea[256];
    while (fgets(linea, sizeof(linea), archivo))
    {
        char user1[20], user2[20], estado[20];
        sscanf(linea, "%[^,],%[^,],%[^,]", user1, user2, estado);
        insertarAmigos(user1, user2, estado);
    }

    fclose(archivo);
}

// creamos las funciones para insertar datos en las tablas
void insertarUsuarios(char *user, char *contraseña)
{
    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "INSERT INTO USUARIOS (user, contraseña) VALUES (?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user, strlen(user), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contraseña, strlen(contraseña), SQLITE_STATIC); // Esté último paso ahí que repasarlo

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // printf("✅ Usuario '%s' insertado correctamente.\n", user); lo comento que sino sale en el cmd to lo raro

    sqlite3_close(db);
}

void insertarPartida(char *user, int idjuego, int puntmax, char *fechaInicio, char *fechaFin)
{

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "INSERT INTO PARTIDA (user,fecha_inicio, fecha_fin, id_juego, puntuacion_maxima) VALUES (?, ?, ?, ?,?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user, strlen(user), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, fechaInicio, strlen(fechaInicio), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, fechaFin, strlen(fechaFin), SQLITE_STATIC);

    sqlite3_bind_int(stmt, 4, idjuego);
    sqlite3_bind_int(stmt, 5, puntmax);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("✅ Partida '%i' guardada correctamente.\n", idjuego);

    sqlite3_close(db);
}

int getPuntosTotalesUsuario(char *user, char *emulador)
{
    sqlite3 *db;
    if (abrirBaseDeDatos(&db))
    {
        // Error al abrir la base de datos
        return -1; // o algún valor para indicar error
    }

    // const char *sql = "SELECT SUM(puntuacion_maxima) FROM PARTIDA WHERE user = ? AND emulador = ?;";
    const char *sql = "SELECT SUM(puntuacion_maxima) FROM PARTIDA p JOIN JUEGO j ON p.id_juego = j.id_juego WHERE p.user = ? AND j.emulador = ?;";
    sqlite3_stmt *stmt;
    int totalPuntos = 0;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Bind de parámetros
    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, emulador, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);
    if (rc == SQLITE_ROW)
    {
        // Obtenemos el resultado
        if (sqlite3_column_type(stmt, 0) != SQLITE_NULL)
        {
            totalPuntos = sqlite3_column_int(stmt, 0);
        }
        else
        {
            totalPuntos = 0; // No tiene partidas con ese emulador
        }
    }
    else
    {
        printf("Error ejecutando la consulta: %s\n", sqlite3_errmsg(db));
        totalPuntos = -1;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return totalPuntos;
}

void insertarJuego(char *titulo, char *rom)
{ // no insertamos puntuacion y usuario record
  // ya que estos los meteremos con update

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "INSERT INTO JUEGO (titulo, rom, emulador) VALUES (?, ?, ?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, titulo, strlen(titulo), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, rom, strlen(rom), SQLITE_STATIC);
    if (strstr(rom, "chip8") != NULL)
    {
        sqlite3_bind_text(stmt, 3, "CHIP8", strlen("CHIP8"), SQLITE_STATIC);
    }
    else if (strstr(rom, "nes") != NULL)
    {
        sqlite3_bind_text(stmt, 3, "NES", strlen("NES"), SQLITE_STATIC);
    }
    else
    {
        sqlite3_bind_text(stmt, 3, "OTHER", strlen("OTHER"), SQLITE_STATIC);
    }

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("✅ Juego '%s' insertado correctamente.\n", titulo);

    sqlite3_close(db);
}

void insertarTiempoJugado(int tiempojugado, char *user, int idjuego)
{

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "INSERT INTO TIEMPO_JUGADO (tiempo_jugado, user, id_juego) VALUES (?, ?, ?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, tiempojugado);
    sqlite3_bind_text(stmt, 2, user, strlen(user), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, idjuego);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("✅ Tiempo jugado '%i' guardado correctamente.\n", tiempojugado);

    sqlite3_close(db);
}

void insertarLogros(int idlogro, char *nombre, char *descripcion)
{
    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "INSERT INTO LOGROS (id_logro, nombre, descripcion) VALUES (?, ?, ?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, idlogro);
    sqlite3_bind_text(stmt, 2, nombre, strlen(nombre), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, descripcion, strlen(descripcion), SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("✅ Logro '%s' insertado correctamente.\n", nombre);

    sqlite3_close(db);
}

void insertarLogrosUsuarios(char *user, int idlogro, char *fecha)
{
    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return;
    }

    sqlite3_stmt *stmt;
    int existe = 0;

    // Verificar si el usuario ya tiene el logro
    char checkSql[] = "SELECT 1 FROM LOGROS_USUARIO WHERE user = ? AND id_logro = ?";
    if (sqlite3_prepare_v2(db, checkSql, -1, &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_bind_text(stmt, 1, user, strlen(user), SQLITE_STATIC);
        sqlite3_bind_int(stmt, 2, idlogro);

        if (sqlite3_step(stmt) == SQLITE_ROW)
        {
            existe = 1;
        }
        sqlite3_finalize(stmt);
    }

    if (existe)
    {
        printf("⚠️ El usuario '%s' ya tiene el logro '%i'. No se insertó de nuevo.\n", user, idlogro);
        sqlite3_close(db);
        return;
    }

    // Insertar el logro si no existe
    char sql[] = "INSERT INTO LOGROS_USUARIO (user, id_logro, fecha) VALUES (?, ?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user, strlen(user), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, idlogro);
    sqlite3_bind_text(stmt, 3, fecha, strlen(fecha), SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("✅ Logro '%i' del usuario '%s' insertado correctamente.\n", idlogro, user);

    sqlite3_close(db);
}

void insertarAmigos(char *user1, char *user2, char *estado)
{ // en principio guardamos ESTADO como texto ya veremos mas adelante si hay que cambiarlo o no

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "INSERT INTO AMIGOS (user1, user2, estado) VALUES (?, ?, ?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user1, strlen(user1), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user2, strlen(user2), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, estado, strlen(estado), SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("✅ Usuarios '%s' y '%s' han registrados su amistad en estado '%s' correctamente.\n", user1, user2, estado);

    int cantidad1 = contarAmigos(user1);
    int cantidad2 = contarAmigos(user2);

    // Conseguimos la fecha

    time_t t = time(NULL);               // Obtener tiempo actual
    struct tm *tm_info = localtime(&t);  // Convertir a fecha local
    char hoy[11];
    if (tm_info != NULL) {
        strftime(hoy, sizeof(hoy), "%Y-%m-%d", tm_info);  // Formatear como string
    } else {
        hoy[0] = '\0'; // En caso de error
    }

    int FirstFriend = 14;
    int SocialLife = 15;

    if (cantidad1 >= 1) {
        insertarLogrosUsuarios(user1, FirstFriend, hoy);
    } else if (cantidad1 >= 3)
    {
        insertarLogrosUsuarios(user1, SocialLife, hoy);
    }
    
    if (cantidad2 >= 1) {
        insertarLogrosUsuarios(user2, FirstFriend, hoy);
    } else if (cantidad2 >= 3)
    {
        insertarLogrosUsuarios(user2, SocialLife, hoy);
    }

    sqlite3_close(db);
}

// Creamos las funciones para actualizar los datos que nos interesan de las tablas

// updateTiempoJugado(de un usuario a un juego)

void updateTiempoJugado(int tiempoJugado, char *user, int id_juego)
{

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "UPDATE TIEMPO_JUGADO SET tiempo_jugado = ? WHERE user = ? AND id_juego = ?;";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, tiempoJugado);
    sqlite3_bind_text(stmt, 2, user, strlen(user), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 3, id_juego);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("✅ Tiempo Jugado '%i' actualizado correctamente.\n", tiempoJugado);

    sqlite3_close(db);
}

// updateEstado_Amigos(entre dos usuarios)

void updateEstado_Amigos(char *user1, char *user2, char *estado)
{

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "UPDATE AMIGOS SET estado = ? WHERE user1 = ? AND user2 = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, estado, strlen(estado), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, user1, strlen(user1), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 3, user2, strlen(user2), SQLITE_STATIC);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("✅ El estado entre el user '%s' y el user '%s' ha sido actualizado a '%S' correctamente", user1, user2, estado);

    sqlite3_close(db);
}

// updateContrasena (de usuario)

// da error "Database is locked"
bool updateContrasena(char *newcontrasena, char *user)
{

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return false;
    }

    sqlite3_stmt *stmt;
    char sql[] = "UPDATE USUARIOS SET contraseña = ? WHERE user = ?";

    // Preparar la consulta
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Error en la preparación de la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return false;
    }

    // Vincular los valores a la consulta
    sqlite3_bind_text(stmt, 1, newcontrasena, strlen(newcontrasena), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user, strlen(user), SQLITE_TRANSIENT);

    // Ejecutar la consulta
    if (sqlite3_step(stmt) != SQLITE_DONE)
    {
        printf("Error al ejecutar el UPDATE: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return false;
    }

    // Impresión de éxito
    // printf("✅ Contraseña del usuario '%s' actualizada correctamente.\n", user);

    // Limpiar
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return true;
}

// updateUsuarioRecord (Juego)

void updateUsuarioPuntuacionRecord(char *usuario, int newPunt, int idjuego)
{

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "UPDATE JUEGO SET usuario_record = ?, puntuacion_record = ? WHERE id_juego = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, usuario, strlen(usuario), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, newPunt);
    sqlite3_bind_int(stmt, 3, idjuego);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("✅ Usuario y Puntuacion record actualizada correctamente.");

    sqlite3_close(db);
}

int getPuntuacionRecord(int idJuego)
{
    int puntuacionRecord = 0;
    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return -1;
    }

    sqlite3_stmt *stmt;

    char sql[] = "SELECT puntuacion_record FROM JUEGO WHERE id_juego = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, idJuego);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {

        puntuacionRecord = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return puntuacionRecord;
}

// funcion para comprobar si existe un usuario y contraseña

bool existeUsuarioYPas(char *name, char *pass)
{

    bool existe = false;
    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return false;
    }

    sqlite3_stmt *stmt;

    char sql[] = "SELECT * FROM USUARIOS WHERE user = ? AND contraseña = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, name, strlen(name), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass, strlen(pass), SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {

        existe = true;
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);

    return existe;
}

// funcion para comprobar si existe un usuario

bool existeUsuario(char *name)
{

    bool existe = false;
    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return false;
    }

    sqlite3_stmt *stmt;

    char sql[] = "SELECT * FROM USUARIOS WHERE user = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, name, strlen(name), SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {

        existe = true;
    }
    sqlite3_finalize(stmt);

    sqlite3_close(db);

    return existe;
}

// Función para obtener la contraseña de un usuario

bool comprobarContraseña(char *user, char *password)
{

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return false;
    }

    sqlite3_stmt *stmt;

    char sql[] = "SELECT * FROM USUARIOS WHERE user = ? AND contraseña = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, password, -1, SQLITE_STATIC);
    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return true;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return false;
}

int getIdJuego(char *romjuego)
{

    int idJuego = 0;
    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return -1;
    }

    sqlite3_stmt *stmt;

    char sql[] = "SELECT id_juego FROM JUEGO WHERE rom = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, romjuego, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {

        idJuego = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return idJuego;
}

bool hajugado(char *user, int id_juego)
{

    bool hajugado = false;
    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return hajugado;
    }

    sqlite3_stmt *stmt;

    const char sql[] = "SELECT id_juego FROM TIEMPO_JUGADO WHERE user = ? AND id_juego = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id_juego);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {

        hajugado = true;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return hajugado;
}

/*
    // Creamos la tabla TIEMPO_JUGADO
    char sql4[] = "CREATE TABLE IF NOT EXISTS TIEMPO_JUGADO ("
    "tiempo_jugado INT DEFAULT 0,"
    "user VARCHAR(20),"
    "id_juego INTEGER,"
    "PRIMARY KEY (user, id_juego),"
    "FOREIGN KEY (user) REFERENCES USUARIOS(user),"
    "FOREIGN KEY (id_juego) REFERENCES JUEGO(id_juego));"; */

int getTiempoJugado(char *user, int idJuego)
{

    int tiempoJugado = 0;
    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return 0;
    }

    sqlite3_stmt *stmt;

    char sql[] = "SELECT tiempo_jugado FROM TIEMPO_JUGADO WHERE user = ? AND id_juego = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, idJuego);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {

        tiempoJugado = sqlite3_column_int(stmt, 0);
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return tiempoJugado;
}

/**
 * @brief Obtiene el tiempo jugado de todos los juegos de un usuario.
 *
 * Esta función devuelve la cantidad de juegos jugados por un usuario y asigna
 * los nombres de los juegos y los tiempos jugados a los punteros proporcionados.
 * Es responsabilidad del usuario liberar la memoria asignada a los punteros
 * después de utilizarlos.
 *
 * @param user El nombre del usuario del cual se desea obtener la información.
 * @param nombreJuegos Puntero doble donde se almacenarán los nombres de los juegos.
 *                      La memoria será asignada dinámicamente dentro de la función.
 * @param tiempos Puntero donde se almacenarán los tiempos jugados de cada juego.
 *                La memoria será asignada dinámicamente dentro de la función.
 * @return int La cantidad de juegos jugados por el usuario o -1 si ocurrió un error.
 */
int getTiempoJugadoTodosLosJuegos(char *user, char ***nombreJuegos, int **tiempos)
{
    int cantidadJuegos = 0;

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return -1;
    }

    sqlite3_stmt *stmt;

    // Contamos la cantidad de juegos jugados por el usuario
    char countSql[] = "SELECT COUNT(*) FROM TIEMPO_JUGADO WHERE user = ?";
    if (sqlite3_prepare_v2(db, countSql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        cantidadJuegos = sqlite3_column_int(stmt, 0);
    }
    else
    {
        printf("Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_finalize(stmt);

    // Reservamos memoria para los nombres de los juegos y los tiempos
    *nombreJuegos = (char **)malloc(cantidadJuegos * sizeof(char *));
    if (*nombreJuegos == NULL)
    {
        printf("Error al asignar memoria para los nombres de los juegos\n");
        sqlite3_close(db);
        return -1;
    }

    *tiempos = (int *)malloc(cantidadJuegos * sizeof(int));
    if (*tiempos == NULL)
    {
        printf("Error al asignar memoria para los tiempos jugados\n");
        free(*nombreJuegos);
        sqlite3_close(db);
        return -1;
    }

    // Obtenemos los nombres de los juegos y los tiempos jugados
    char sql[] = "SELECT JUEGO.titulo, TIEMPO_JUGADO.tiempo_jugado FROM TIEMPO_JUGADO "
                 "INNER JOIN JUEGO ON TIEMPO_JUGADO.id_juego = JUEGO.id_juego WHERE TIEMPO_JUGADO.user = ?";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        free(*nombreJuegos);
        free(*tiempos);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *titulo = (const char *)sqlite3_column_text(stmt, 0);
        int tiempoJugado = sqlite3_column_int(stmt, 1);

        (*nombreJuegos)[i] = strdup(titulo); // Copiamos el nombre del juego
        if ((*nombreJuegos)[i] == NULL)
        {
            printf("Error al asignar memoria para el nombre del juego\n");
            for (int j = 0; j < i; j++)
            {
                free((*nombreJuegos)[j]);
            }
            free(*nombreJuegos);
            free(*tiempos);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return -1;
        }

        (*tiempos)[i] = tiempoJugado;
        i++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return cantidadJuegos;
}

int getPartidasDeJuego(char *user, char *nombreJuego, char ***partidas, int **tiemposJugados, int **puntuacionesMaximas)
{
    int cantidadPartidas = 0;

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return -1;
    }

    sqlite3_stmt *stmt;

    // Contamos la cantidad de partidas jugadas por el usuario
    char countSql[] = "SELECT COUNT(*) FROM PARTIDA WHERE user = ? AND id_juego = (SELECT id_juego FROM JUEGO WHERE titulo = ?)";
    if (sqlite3_prepare_v2(db, countSql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, nombreJuego, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        cantidadPartidas = sqlite3_column_int(stmt, 0);
    }
    else
    {
        printf("Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_finalize(stmt);

    // Reservamos memoria para las partidas y los tiempos
    *partidas = (char **)malloc(cantidadPartidas * sizeof(char *));
    if (*partidas == NULL)
    {
        printf("Error al asignar memoria para las partidas\n");
        sqlite3_close(db);
        return -1;
    }

    *tiemposJugados = (int *)malloc(cantidadPartidas * sizeof(int));
    if (*tiemposJugados == NULL)
    {
        printf("Error al asignar memoria para los tiempos jugados\n");
        free(*partidas);
        sqlite3_close(db);
        return -1;
    }
    *puntuacionesMaximas = (int *)malloc(cantidadPartidas * sizeof(int));
    if (*puntuacionesMaximas == NULL)
    {
        printf("Error al asignar memoria para las puntuaciones maximas\n");
        free(*partidas);
        free(*tiemposJugados);
        sqlite3_close(db);
        return -1;
    }

    // Obtenemos las partidas jugadas
    char sql[] = "SELECT id_partida, tiempo_jugado, puntuacion_maxima FROM PARTIDA WHERE user = ? AND id_juego = (SELECT id_juego FROM JUEGO WHERE titulo = ?)";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        free(*partidas);
        free(*tiemposJugados);
        free(*puntuacionesMaximas);
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, nombreJuego, -1, SQLITE_STATIC);

    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        int idPartida = sqlite3_column_int(stmt, 0);
        int tiempoJugado = sqlite3_column_int(stmt, 1);
        int puntuacionMaxima = sqlite3_column_int(stmt, 2);

        (*partidas)[i] = (char *)malloc(20 * sizeof(char)); // Reservamos memoria para el id de la partida
        if ((*partidas)[i] == NULL)
        {
            printf("Error al asignar memoria para el id de la partida\n");
            for (int j = 0; j < i; j++)
            {
                free((*partidas)[j]);
            }
            free(*partidas);
            free(*tiemposJugados);
            free(*puntuacionesMaximas);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return -1;
        }
        sprintf((*partidas)[i], "%d", idPartida); // Convertimos el id de la partida a string

        (*tiemposJugados)[i] = tiempoJugado;
        (*puntuacionesMaximas)[i] = puntuacionMaxima;
        i++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return cantidadPartidas;
}

int getNombreJuegos(char *user, char ***nombreJuegos)
{
    sqlite3_stmt *stmt;
    int rc;
    int count = 0;
    char **games = NULL;

    const char *sql = "SELECT titulo FROM JUEGO WHERE id_juego in (SELECT id_juego FROM TIEMPO_JUGADO WHERE user = ?)";

    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    rc = sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    // First count how many rows we have
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        count++;
    }

    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Failed to step through statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    // Allocate array for game names
    games = (char **)malloc(count * sizeof(char *));
    if (!games)
    {
        sqlite3_finalize(stmt);
        return -1;
    }

    // Reset the statement to run again
    sqlite3_reset(stmt);

    // Now fetch the actual data
    count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *title = (const char *)sqlite3_column_text(stmt, 0);
        games[count] = strdup(title);
        if (!games[count])
        {
            // Handle memory allocation failure
            for (int i = 0; i < count; i++)
            {
                free(games[i]);
            }
            free(games);
            sqlite3_finalize(stmt);
            return -1;
        }
        count++;
    }

    *nombreJuegos = games;
    sqlite3_finalize(stmt);
    return count;
}

int getJuegosDisponibles(char ***nombreJuegos, char *emulador)
{
    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return -1;
    }

    sqlite3_stmt *stmt;
    int rc;
    int count = 0;
    char **games = NULL;

    const char *sql = "SELECT titulo FROM JUEGO WHERE emulador = ?";

    // Prepare the SQL statement
    rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to prepare statement: %s\n", sqlite3_errmsg(db));
        return -1;
    }

    // Bind the emulator parameter
    rc = sqlite3_bind_text(stmt, 1, emulador, -1, SQLITE_STATIC);
    if (rc != SQLITE_OK)
    {
        fprintf(stderr, "Failed to bind parameter: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    // First count how many rows we have
    while ((rc = sqlite3_step(stmt)) == SQLITE_ROW)
    {
        count++;
    }

    if (rc != SQLITE_DONE)
    {
        fprintf(stderr, "Failed to step through statement: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        return -1;
    }

    // Allocate array for game names
    games = (char **)malloc(count * sizeof(char *));
    if (!games)
    {
        sqlite3_finalize(stmt);
        return -1;
    }

    // Reset the statement to run again
    sqlite3_reset(stmt);

    // Now fetch the actual data
    count = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *title = (const char *)sqlite3_column_text(stmt, 0);
        games[count] = strdup(title);
        if (!games[count])
        {
            // Handle memory allocation failure
            for (int i = 0; i < count; i++)
            {
                free(games[i]);
            }
            free(games);
            sqlite3_finalize(stmt);
            return -1;
        }
        count++;
    }
    *nombreJuegos = games;
    sqlite3_finalize(stmt);
    return count;
}

char **getNombreAmigos(char *user, int *cantidad)
{
    *cantidad = 0;
    char **nombreAmigos;

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return NULL;
    }

    sqlite3_stmt *stmt;

    const char *sql = "SELECT user1 FROM AMIGOS WHERE user2 = ? AND estado = 'aceptado'";

    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Error al preparar consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return NULL;
    }

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);

    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        char *nombre = sqlite3_column_text(stmt, 0);

        // realloc para añadir más espacio
        char **temp = realloc(nombreAmigos, (*cantidad + 1) * sizeof(char *));

        nombreAmigos[*cantidad] = strdup(nombre);

        if (nombreAmigos[*cantidad] == NULL)
        {
            printf("Error al asignar memoria para el nombre del juego\n");
            for (int i = 0; i < *cantidad; i++)
            {
                free(nombreAmigos[i]);
            }
            free(nombreAmigos);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return NULL;
        }

        // (*nombreJuegos)[i] = strdup(titulo); // Copiamos el nombre del juego

        // (*tiempos)[i] = tiempoJugado;
        *cantidad++;
    }

    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return nombreAmigos;
}

int getLogros(char *user, char ***nombreLogros, char ***descripcionLogros, char ***fechaObtencion)
{
    int cantidadLogros = 0;

    // Abrimos la base de datos
    if (abrirBaseDeDatos(&db))
    {
        return -1;
    }

    sqlite3_stmt *stmt;

    // Contamos la cantidad de logros obtenidos por el usuario
    char countSql[] = "SELECT COUNT(*) FROM LOGROS_USUARIO WHERE user = ?";
    if (sqlite3_prepare_v2(db, countSql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW)
    {
        cantidadLogros = sqlite3_column_int(stmt, 0);
    }
    else
    {
        printf("Error al ejecutar la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_finalize(stmt);

    // Reservamos memoria para los nombres de los logros y las fechas
    *nombreLogros = (char **)malloc(cantidadLogros * sizeof(char *));
    if (*nombreLogros == NULL)
    {
        printf("Error al asignar memoria para los nombres de los logros\n");
        sqlite3_close(db);
        return -1;
    }

    *descripcionLogros = (char **)malloc(cantidadLogros * sizeof(char *));
    if (*descripcionLogros == NULL)
    {
        printf("Error al asignar memoria para las descripciones de los logros\n");
        free(*nombreLogros);
        sqlite3_close(db);
        return -1;
    }

    *fechaObtencion = (char **)malloc(cantidadLogros * sizeof(char *));
    if (*fechaObtencion == NULL)
    {
        printf("Error al asignar memoria para las fechas de obtención de los logros\n");
        free(*nombreLogros);
        free(*descripcionLogros);
        sqlite3_close(db);
        return -1;
    }

    // Obtenemos los nombres de los logros y las fechas
    char sql[] = "SELECT LOGROS.nombre, LOGROS.descripcion, LOGROS_USUARIO.fecha FROM LOGROS_USUARIO "
                 "INNER JOIN LOGROS ON LOGROS_USUARIO.id_logro = LOGROS.id_logro WHERE LOGROS_USUARIO.user = ?";
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK)
    {
        printf("Error al preparar la consulta: %s\n", sqlite3_errmsg(db));
        free(*nombreLogros);
        free(*descripcionLogros);
        free(*fechaObtencion);
        sqlite3_close(db);
        return -1;
    }
    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    int i = 0;
    while (sqlite3_step(stmt) == SQLITE_ROW)
    {
        const char *nombre = (const char *)sqlite3_column_text(stmt, 0);
        const char *descripcion = (const char *)sqlite3_column_text(stmt, 1);
        const char *fecha = (const char *)sqlite3_column_text(stmt, 2);

        (*nombreLogros)[i] = strdup(nombre); // Copiamos el nombre del logro
        if ((*nombreLogros)[i] == NULL)
        {
            printf("Error al asignar memoria para el nombre del logro\n");
            for (int j = 0; j < i; j++)
            {
                free((*nombreLogros)[j]);
            }
            free(*nombreLogros);
            free(*descripcionLogros);
            free(*fechaObtencion);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return -1;
        }

        (*descripcionLogros)[i] = strdup(descripcion); // Copiamos la descripcion del logro
        if ((*descripcionLogros)[i] == NULL)
        {
            printf("Error al asignar memoria para la descripcion del logro\n");
            for (int j = 0; j < i; j++)
            {
                free((*nombreLogros)[j]);
                free((*descripcionLogros)[j]);
            }
            free(*nombreLogros);
            free(*descripcionLogros);
            free(*fechaObtencion);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return -1;
        }

        (*fechaObtencion)[i] = strdup(fecha); // Copiamos la fecha de obtencion del logro
        if ((*fechaObtencion)[i] == NULL)
        {
            printf("Error al asignar memoria para la fecha de obtencion del logro\n");
            for (int j = 0; j < i; j++)
            {
                free((*nombreLogros)[j]);
                free((*descripcionLogros)[j]);
                free((*fechaObtencion)[j]);
            }
            free(*nombreLogros);
            free(*descripcionLogros);
            free(*fechaObtencion);
            sqlite3_finalize(stmt);
            sqlite3_close(db);
            return -1;
        }

        i++;
    }
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return cantidadLogros;
}

int contarAmigos(char* user) {
    sqlite3 *db;

    if (abrirBaseDeDatos(&db))
    {
        // Error al abrir la base de datos
        return -1; // o algún valor para indicar error
    }

    char* sql = "SELECT COUNT(user1) FROM AMIGOS WHERE user1 = ? or user2 = ?";

    sqlite3_stmt *stmt;

    int rc = sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);
    if (rc != SQLITE_OK)
    {
        printf("Error preparando la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return -1;
    }

    // Bind de parámetros
    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user, -1, SQLITE_TRANSIENT);

    rc = sqlite3_step(stmt);

    printf("%i", rc);

    sqlite3_finalize(stmt);
    sqlite3_close(db);

    return rc;
}