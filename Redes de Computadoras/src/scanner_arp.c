#include <stdio.h>
#include <string.h>
#include <sqlite3.h>
#include "../include/scanner_arp.h"

/**
 * gestionar_error - gestionar los errores generados por una sentencia SQL
 * 
 * @db: El objeto que permite manejar la base de datos con sqlite
 * 
 * La función muestra el mensaje de error en la salida de error
 * estándar y retorna el código de error.
 */
int gestionar_error(sqlite3 *db)
{
    fprintf(stderr, "Error: %s\n", sqlite3_errmsg(db));

    return sqlite3_errcode(db);
}

/**
 * mostrar_db - obtener una fila de una consulta SELECT a una base de datos
 * 
 * @ptr: Información que se pasa a la función
 * @numero_columnas: El número de campos de la tabla
 * @valores_celdas: Los valores de la fila actual
 * @nombres_columnas: El nombre de los campos de la tabla
 * 
 * La función permite obtener los valores de una fila de una consulta
 * SELECT a una base de datos y manejarlos como se necesite.
 */
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

/**
 * ip_cadena - convertir una dirección IP de 4 bytes en la notación
 * de puntos y números
 * 
 * @arreglo_ip: El arreglo que contiene los 4 dígitos de la dirección
 * IP en notación decimal
 * @cadena_ip: La cadena donde será alamcenada la dirección IP en la
 * notación de puntos y números
 * 
 * La función toma los dígitos de la dirección IP almacenada en
 * @arreglo_ip y los formatea en la notación xxx.xxx.xxx.xxx que es
 * almacenada en @cadena_ip.
 */
void ip_cadena(unsigned char *arreglo_ip, char *cadena_ip)
{
    int i;
    char numero[4];

    for (i = 0; i < 4; i++) {
        sprintf(numero, "%d", arreglo_ip[i]);

        if (i == 0) {
            strcpy(cadena_ip, numero);
        } else {
            strcat(cadena_ip, numero);
        }

        if (i < 3) {
            strcat(cadena_ip, ".");
        }
    }
}

/**
 * mac_cadena - convertir una dirección MAC de 6 bytes en la notación
 * MAC de dos puntos
 * 
 * @arreglo_mac: El arreglo que contiene los 6 dígitos de la dirección
 * MAC en notación decimal
 * @cadena_mac: La cadena donde será almacenada la dirección MAC en la
 * notación de dos puntos
 * 
 * La función toma los dígitos de la dirección MAC almacenada en
 * @arreglo_mac y los formatea en la notación xx:xx:xx:xx:xx:xx que
 * es almacenada en @cadena_mac. 
 */
void mac_cadena(unsigned char *arreglo_mac, char *cadena_mac)
{
    int i;
    char numero[3];

    for (i = 0; i < 6; i++) {
        sprintf(numero, "%.2x", arreglo_mac[i]);

        if (i == 0) {
            strcpy(cadena_mac, numero);
        } else {
            strcat(cadena_mac, numero);
        }

        if (i < 5) {
            strcat(cadena_mac, ":");
        }
    }
}
