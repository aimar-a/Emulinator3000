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




}
