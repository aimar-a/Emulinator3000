#include <stdio.h>
#include "sqlite3.h"
#include <string.h>
#include "bd.h"

sqlite3 *db;
const char *db_filename = "emulatorBD.sqlite";

int abrirBaseDeDatos(sqlite3 **db) {
    int rc = sqlite3_open(db_filename, db);
    if (rc) {
        fprintf(stderr, "No se pudo abrir la base de datos: %s\n", sqlite3_errmsg(*db));
        return rc;
    }
    return SQLITE_OK;
}
// Función para crear la BD y su estructura
void crearBD() {
    sqlite3_stmt *stmt;

    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
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
                  "FOREIGN KEY (usuario_record) REFERENCES USUARIOS(user));";

    sqlite3_prepare_v2(db, sql2, -1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Creamos la tabla PARTIDA
    char sql3[] = "CREATE TABLE IF NOT EXISTS PARTIDA ("
                  "id_partida INTEGER PRIMARY KEY AUTOINCREMENT,"
                  "user VARCHAR(20) NOT NULL,"
                  "id_juego INTEGER NOT NULL,"
                  "tiempo_jugado INT NOT NULL,"
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
                  "descripcion TEXT NOT NULL,"
                  "id_juego INTEGER NOT NULL,"
                  "FOREIGN KEY (id_juego) REFERENCES JUEGO(id_juego));";

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

void limpiarBaseDeDatos() {
    int rc = abrirBaseDeDatos(&db);
    if (rc != SQLITE_OK) return;

    // Lista de todas tus tablas
    const char *tablas[] = {
        "AMIGOS",
        "LOGROS_USUARIO",
        "LOGROS",
        "TIEMPO_JUGADO",
        "PARTIDA",
        "JUEGO",
        "USUARIOS",
        NULL  // Marca el final del array
    };

    char *errMsg = NULL;
    const char **tabla = tablas;
    
    while (*tabla) {
        char sql[256];
        snprintf(sql, sizeof(sql), "DELETE FROM %s;", *tabla);
        
        rc = sqlite3_exec(db, sql, 0, 0, &errMsg);
        if (rc != SQLITE_OK) {
            fprintf(stderr, "Error al limpiar la tabla %s: %s\n", *tabla, errMsg);
            sqlite3_free(errMsg);
        } else {
            printf("Tabla %s limpiada exitosamente.\n", *tabla);
        }
        tabla++;
    }

    // Reiniciamos los autoincrementales
    const char *resetAI = "DELETE FROM sqlite_sequence;";
    rc = sqlite3_exec(db, resetAI, 0, 0, &errMsg);
    if (rc != SQLITE_OK) {
        fprintf(stderr, "Error al resetear autoincrementales: %s\n", errMsg);
        sqlite3_free(errMsg);
    }

    sqlite3_close(db);
    printf("Base de datos limpiada completamente.\n");
}

void eliminarBaseDeDatos() {
    if (remove(db_filename) == 0) {
        printf("Base de datos eliminada exitosamente.\n");
    } else {
        perror("Error al eliminar la base de datos");
    }
}
//creamos las funciones para insertar datos en las tablas
void insertarUsuarios(char* user, char* contraseña) {

    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "INSERT INTO USUARIOS (user, contraseña) VALUES (?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user, strlen(user), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contraseña, strlen(contraseña), SQLITE_STATIC);  // Esté último paso ahí que repasarlo

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

   // printf("✅ Usuario '%s' insertado correctamente.\n", user); lo comento que sino sale en el cmd to lo raro

    sqlite3_close(db);
}

void insertarPartida(char* user, int idjuego, int tiempojugado, int puntmax){

    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "INSERT INTO PARTIDA (user, id_juego, tiempo_jugado, puntuacion_maxima) VALUES (?, ?, ?, ?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user, strlen(user), SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, idjuego);  
    sqlite3_bind_int(stmt, 3, tiempojugado);
    sqlite3_bind_int(stmt, 4, puntmax);


    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("✅ Partida '%i' guardada correctamente.\n", idjuego);

    sqlite3_close(db);
}

void insertarJuego(char* titulo, char* rom){ //no insertamos puntuacion y usuario record
                                             //ya que estos los meteremos con update

    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "INSERT INTO JUEGO (titulo, rom) VALUES (?, ?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, titulo, strlen(titulo), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, rom, strlen(rom), SQLITE_STATIC);  



    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
  
    printf("✅ Juego '%s' insertado correctamente.\n",titulo);

    sqlite3_close(db);
}

void insertarTiempoJugado(int tiempojugado, char* user, int idjuego){

    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
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

void insertarLogros(char* nombre, char* descripcion, int idjuego){

    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
        return;
    }

    sqlite3_stmt *stmt;

    char sql[] = "INSERT INTO LOGROS (nombre, descripcion, id_juego) VALUES (?, ?, ?);";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);


    sqlite3_bind_text(stmt, 1, nombre, strlen(nombre), SQLITE_STATIC);  
    sqlite3_bind_text(stmt, 2, descripcion, strlen(descripcion), SQLITE_STATIC);  
    sqlite3_bind_int(stmt, 3, idjuego);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("✅ Logro '%s' insertado correctamente.\n", nombre);

    sqlite3_close(db);
}

void insertarLogrosUsuarios(char* user, int idlogro, char* fecha ){ //en principio guardamos la FECHA como texto ya veremos mas adelante si hay que cambiarlo o no

    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
        return;
    }

    sqlite3_stmt *stmt;

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

void insertarAmigos(char* user1, char* user2, char* estado){ //en principio guardamos ESTADO como texto ya veremos mas adelante si hay que cambiarlo o no

    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
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

    sqlite3_close(db);
}

// Creamos las funciones para actualizar las tablas

// updateTiempoJugado(de un usuario a un juego)


void updateTiempoJugado(int tiempoJugado, char* user, int id_juego) {

    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
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

void updateEstado_Amigos(char* user1, char* user2, char* estado) {

    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
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


//updateContrasena (de usuario)

//da error "Database is locked"
void updateContrasena(char* newcontrasena, char* user) { 

    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
        return;
    }

    sqlite3_stmt *stmt;
    char sql[] = "UPDATE USUARIOS SET contraseña = ? WHERE user = ?";

    // Preparar la consulta
    if (sqlite3_prepare_v2(db, sql, -1, &stmt, NULL) != SQLITE_OK) {
        printf("Error en la preparación de la consulta: %s\n", sqlite3_errmsg(db));
        sqlite3_close(db);
        return;
    }

    // Vincular los valores a la consulta
    sqlite3_bind_text(stmt, 1, newcontrasena, strlen(newcontrasena), SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, user, strlen(user), SQLITE_TRANSIENT);

    // Ejecutar la consulta
    if (sqlite3_step(stmt) != SQLITE_DONE) {
        printf("Error al ejecutar el UPDATE: %s\n", sqlite3_errmsg(db));
        sqlite3_finalize(stmt);
        sqlite3_close(db);
        return;
    }

    // Impresión de éxito
   // printf("✅ Contraseña del usuario '%s' actualizada correctamente.\n", user);

    // Limpiar
    sqlite3_finalize(stmt);
    sqlite3_close(db);
}

//updateUsuarioRecord (Juego)

void updateUsuarioPuntuacionRecord(char* usuario, int newPunt, int idjuego){

    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
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

int getPuntuacionRecord(int idJuego){
    int puntuacionRecord = 0;
    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
    }

    sqlite3_stmt *stmt;

    char sql[] = "SELECT puntuacion_record FROM JUEGO WHERE id_juego = ?";


    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_int(stmt, 1, idJuego);


    if (sqlite3_step(stmt) == SQLITE_ROW) {

        puntuacionRecord = sqlite3_column_int(stmt, 0);  

    }  
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return puntuacionRecord;

}


//funcion para comprobar si existe un usuario y contraseña

bool existeUsuarioYPas(char* name, char* pass){

    bool existe = false;
    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
    }

    sqlite3_stmt *stmt;

    char sql[] = "SELECT * FROM USUARIOS WHERE user = ? AND contraseña = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);


    sqlite3_bind_text(stmt, 1, name, strlen(name), SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, pass, strlen(pass), SQLITE_STATIC);

    if(sqlite3_step(stmt)==SQLITE_ROW){

        existe = true;
        
    }
    sqlite3_finalize(stmt);


    sqlite3_close(db);
    
    return existe;

}


//funcion para comprobar si existe un usuario 

bool existeUsuario(char* name){

    bool existe = false;
    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
    }

    sqlite3_stmt *stmt;

    char sql[] = "SELECT * FROM USUARIOS WHERE user = ?";

    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);


    sqlite3_bind_text(stmt, 1, name, strlen(name), SQLITE_STATIC);
    if(sqlite3_step(stmt)==SQLITE_ROW){

        existe = true;
        
    }
    sqlite3_finalize(stmt);


    sqlite3_close(db);
    
    return existe;

}

// Función para obtener la contraseña de un usuario

bool comprobarContraseña(char* user, char* password) {
    
    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
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




int getIdJuego(char* romjuego){

    int idJuego = 0;
    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
    }

    sqlite3_stmt *stmt;

    char sql[] = "SELECT id_juego FROM JUEGO WHERE rom = ?";


    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, romjuego, -1, SQLITE_STATIC);

    if (sqlite3_step(stmt) == SQLITE_ROW) {

        idJuego = sqlite3_column_int(stmt, 0);  

    }  
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return idJuego;

}




bool hajugado(char* user, int id_juego){

    bool hajugado=false;
    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
    }

    sqlite3_stmt *stmt;

    const char sql[] = "SELECT id_juego FROM TIEMPO_JUGADO WHERE user = ? AND id_juego = ?";


    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, id_juego);


    if (sqlite3_step(stmt) == SQLITE_ROW) {

        hajugado=true;
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

int getTiempoJugado(char* user, int idJuego){

    int tiempoJugado = 0;
    // Abrimos la base de datos
    if (sqlite3_open("emulatorBD.sqlite", &db) != SQLITE_OK) {
        printf("Error al abrir la base de datos\n");
    }

    sqlite3_stmt *stmt;

    char sql[] = "SELECT tiempo_jugado FROM TIEMPO_JUGADO WHERE user = ? AND id_juego = ?";


    sqlite3_prepare_v2(db, sql, -1, &stmt, NULL);

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_int(stmt, 2, idJuego);


    if (sqlite3_step(stmt) == SQLITE_ROW) {

        tiempoJugado = sqlite3_column_int(stmt, 0);  

    }  
    
    sqlite3_finalize(stmt);
    sqlite3_close(db);
    return tiempoJugado;

}