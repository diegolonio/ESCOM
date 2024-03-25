// Primero hay que instalar sqlite3 en nuestro equipo
// Instalación sqlite3: sudo apt install sqlite3 libsqlite3-dev
// Compilación: gcc sqlite.c -o sqlite -l sqlite3

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sqlite3.h>

int gestionar_error(sqlite3 *);
int mostrar_db(void *, int, char **, char **);

int main()
{
    sqlite3 *db = NULL;
    char sentencia[100];

    /* ------------------------------------ Abrir la base de datos ------------------------------------ */
    if (sqlite3_open("direcciones.db", &db) != SQLITE_OK) {
        return gestionar_error(db);
    }

    /* ---------------------------------- Crear tabla de direcciones ---------------------------------- */
    strcpy(sentencia, "CREATE TABLE IF NOT EXISTS direcciones (ip CHAR(15) NOT NULL, mac CHAR(17) NOT NULL)");

    if (sqlite3_exec(db, sentencia, NULL, NULL, NULL) != SQLITE_OK) {
        return gestionar_error(db);
    }

    printf("%s\n", sentencia);

    /* ------------------------------- Insertar un registro en la tabla ------------------------------- */
    sprintf(sentencia, "INSERT INTO direcciones (ip, mac) VALUES ('%s', '%s')", "192.168.100.17", "--:--:--:--:--:--");

    if (sqlite3_exec(db, sentencia, NULL, NULL, NULL) != SQLITE_OK) {
        return gestionar_error(db);
    }

    printf("%s\n", sentencia);

    /* ----------------------------------------- Mostrar datos ---------------------------------------- */
    strcpy(sentencia, "SELECT * FROM direcciones");
    printf("%s\n\n", sentencia);

    printf(" ---------------------------------------");
    printf("\n |   Direccion IP  |   Direccion MAC   |\n");
    printf(" |-------------------------------------|\n");

    if (sqlite3_exec(db, sentencia, mostrar_db, NULL, NULL) != SQLITE_OK) {
        return gestionar_error(db);
    }

    printf(" ---------------------------------------\n");

    /* ---------------------------------------- Eliminar datos ---------------------------------------- */
    sprintf(sentencia, "DELETE FROM direcciones WHERE ip = '%s'", "192.168.100.17");

    if (sqlite3_exec(db, sentencia, NULL, NULL, NULL) != SQLITE_OK) {
        return gestionar_error(db);
    }

    printf("\n%s\n", sentencia);

    /* ------------------------------------ Mostrar datos otra vez ------------------------------------ */
    strcpy(sentencia, "SELECT * FROM direcciones");
    printf("%s\n\n", sentencia);

    printf(" ---------------------------------------");
    printf("\n |   Direccion IP  |   Direccion MAC   |\n");
    printf(" |-------------------------------------|\n");

    if (sqlite3_exec(db, sentencia, mostrar_db, NULL, NULL) != SQLITE_OK) {
        return gestionar_error(db);
    }

    printf(" ---------------------------------------\n");

    sqlite3_close(db);

    return 0;
}

int gestionar_error(sqlite3 *db)
{
    fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));

    return sqlite3_errcode(db);
}

int mostrar_db(void *ptr, int numero_columnas, char **valores_celdas, char **nombres_columnas)
{
    int i, j;

    for (i = 0; i < numero_columnas; i++) {
        if (i == 0) {
            printf(" | ");
        }

        printf("%s", valores_celdas[i]);

        if (i == 0) {
            for (j = 0; j < 15-strlen(valores_celdas[i]); j++) {
                printf(" ");
            }
        }

        printf(" | ");
    }

    printf("\n");

    return 0;
}
