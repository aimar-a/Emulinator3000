#include <stdio.h>
#include "sqlite3.h"
#include <string.h>
#include "bd.h"


sqlite3 *db;


//crear la Bd y su estructura de la bd si no existe ya 
void crearBD(){

    sqlite3_stmt *stmt;

    
    sqlite3_open("emulatorBD.sqlite", &db); //abrimos o creamos la
    
    //creamos la tabla usuarios
    char sql1[] = "create table if not exists USUARIOS("
                "user VARCHAR(8) PRIMARY KEY,"
                "contraseña VARCHAR(8) NOT NULL);";


    sqlite3_prepare_v2(db, sql1, strlen(sql1)+1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    //creamos la tabla Juego
        char sql2[] = "create table if not exists JUEGO("
        "id_juego INT(3) AUTOINCREMENT PRIMARY KEY,"
        "titulo VARCHAR(20) NOT NULL,"
        "puntuacion_record INT(10) NOT NULL,"
        "usuario_record VARCHAR(8),"
        "FOREIGN KEY (usuario_record) REFERENCES USUARIOS(user));";


    sqlite3_prepare_v2(db, sql2, strlen(sql2)+1, &stmt, NULL);

    

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);
    
    //creamos la tabla Partida
    char sql3[] = "create table if not exists PARTIDA("
                "id_partida INT(3) AUTOINCREMENT PRIMARY KEY,"
                "user VARCHAR(8) NOT NULL,"
                "id_juego INT(3) NOT NULL,"
                "tiempo_jugado INT(5) NOT NULL,"
                "puntuacion_maxima INT(10) NOT NULL," //le he puesto 10 prq ns depende del juego cuales pueden ser las puntuaciones
                "FOREIGN KEY (user) REFERENCES USUARIOS(user)"
                "FOREIGN KEY (id_juego) REFERENCES JUEGO(id_juego));";


    sqlite3_prepare_v2(db, sql3, strlen(sql3)+1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Creamos la tabla Tiempo_Jugado

    char sql4[] = "create table if not exists TIEMPO_JUGADO("
                "tiempo_jugado INT DEFAULT 0," // El valor que le damos de inicio es 0 (luego lo vamos incrementando)
                "user VARCHAR(8) PRIMARY KEY,"
                "id_juego INT(3) PRIMARY KEY,"
                "FOREIGN KEY (user) REFERENCES USUARIOS(user),"
                "FOREIGN KEY (id_juego) REFERENCES JUEGO(id_juego));";

    sqlite3_prepare_v2(db, sql4, strlen(sql4)+1, &stmt, NULL);
    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Creamos la tabla Logros

    char sql5[] = "create table if not exists LOGROS("
                "id_logro INT(4) AUTOINCREMENT PRIMARY KEY,"
                "nombre VARCHAR(20) NOT NULL," // He puesto veinte para darle espacio por si hay algun logro con nombre grande
                "descripcion TEXT NOT NULL," // Comprobas que TEXT está bien
                "id_juego INT(3) NOT NULL,"
                "FOREIGN KEY (id_juego) REFERENCES JUEGO(id_juego));";

    sqlite3_prepare_v2(db, sql5, strlen(sql5)+1, &stmt, NULL);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Creamos la tabla Logros_Usuario

    char sql6[] = "create table if not exists LOGROS_USUARIO ("
                "user VARCHAR(8) PRIMARY KEY,"
                "id_logro INT(4) PRIMARY KEY,"
                "fecha TIMESTAMP NOT NULL," // Comprobar si TIMESTAMP está bien
                "FOREIGN KEY (user) REFERENCES USUARIOS(user),"
                "FOREIGN KEY (id_logro) REFERENCES LOGROS(id_logro));";

    sqlite3_prepare_v2(db, sql6, strlen(sql6)+1, &stmt, NULL);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    // Creamos la tabla Amigos

    char sql7[] = "create table if not exists AMIGOS("
                "user1 VARCHAR(8) PRIMARY KEY,"
                "user2 VARCHAR(8) PRIMARY KEY,"
                "estado TEXT CHECK(estado IN ('pendiente', 'aceptado', 'rechazado')) NOT NULL," // Comprobar el enum (pendiente, aceptado, rechazado)
                "FOREIGN KEY (user1) REFERENCES USUARIOS(user),"
                "FOREIGN KEY (user2) REFERENCES USUARIOS(user))"; 

    qlite3_prepare_v2(db, sql7, strlen(sql7)+1, &stmt, NULL);

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);       

}

void crearUsuario(char* user, char* contraseña) {

    sqlite3_stmt *stmt;

    char sql[] = "INSERT INTO USUARIOS (user, contraseña) VALUES (?, ?);";
    sqlite3_prepare_v2(db, sql, -1, &stmt, 0);

    sqlite3_bind_text(stmt, 1, user, -1, SQLITE_STATIC);
    sqlite3_bind_text(stmt, 2, contraseña, -1, SQLITE_STATIC);  // Esté último paso ahí que repasarlo

    sqlite3_step(stmt);
    sqlite3_finalize(stmt);

    printf("✅ Usuario '%s' insertado correctamente.\n", user);
}