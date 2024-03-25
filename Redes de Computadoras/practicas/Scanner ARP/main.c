#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include <sqlite3.h>
#include "../../include/scanner_arp.h"
#include "../../include/redes.h"

int main()
{
    sqlite3 *db = NULL;
    char sentencia[100], nombre_interfaz[10];
    int un_socket, host, i;
    Interfaz interfaz;
    unsigned char trama_enviada[1514], trama_recibida[1514];
    unsigned char ip_destino[4], mac_destino[6], ethertype[2];
    unsigned char cadena_ip[16], cadena_mac[18];
    struct timeval inicio, final;
    long tiempo_espera = 0, segundos, microsegundos;

    /* Creación del archivo de la base de datos */
    touch("direcciones.db");

    /* Apertura de la base de datos */
    if (sqlite3_open("direcciones.db", &db) != SQLITE_OK) {
        return gestionar_error(db);
    }

    /* Borrado de la tabla de direcciones si existe */
    strcpy(sentencia, "DROP TABLE IF EXISTS direcciones");

    if (sqlite3_exec(db, sentencia, NULL, NULL, NULL) != SQLITE_OK) {
        return gestionar_error(db);
    }

    /* Creación de la tabla de direcciones */
    strcpy(sentencia, "CREATE TABLE IF NOT EXISTS direcciones (ip CHAR(15) NOT NULL, mac CHAR(17) NOT NULL)");

    if (sqlite3_exec(db, sentencia, NULL, NULL, NULL) != SQLITE_OK) {
        return gestionar_error(db);
    }

    /* Apertura del socket */
    un_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (un_socket == -1) {
        perror("Error");
        exit(errno);
    }

    /* Obtención del usuario del nombre de la interfaz */
    printf("Ingresa el nombre de la interfaz: ");
    fgets(nombre_interfaz, sizeof(nombre_interfaz), stdin);
    nombre_interfaz[strlen(nombre_interfaz)-1] = '\0';

    if (obtener_datos(un_socket, nombre_interfaz, &interfaz) == false) {
        close(un_socket);
        perror("Error");
        exit(errno);
    }

    /* Obtención del identificador de la subred */
    aplicar_mascara(interfaz.direccion_ip, interfaz.mascara_subred, ip_destino);

    /* Ethertype = 0x0806 (ARP) */
    ethertype[0] = 0x08;
    ethertype[1] = 0x06;

    printf("Haciendo escaneo de la subred...\n\n");

    gettimeofday(&inicio, NULL);

    for (host = 1; host <= 254; host++) {
        ip_destino[3] = host;

        /* Dirección MAC de broadcast */
        for (i = 0; i < 6; i++) {
            mac_destino[i] = 0xff;
        }

        /* Estructuración del encabezado MAC del mensaje de ARP */
        encabezado_mac(trama_enviada, mac_destino, interfaz.direccion_mac, ethertype);
        /* Estructuración del mensaje de ARP */
        mensaje_arp(trama_enviada, interfaz, ip_destino);

        if (enviar_trama(un_socket, interfaz.indice, trama_enviada, 42) == true) {
            if (recibir_trama(un_socket, trama_recibida, trama_enviada, filtro_arp, NULL, NULL) == true) {
                ip_cadena(ip_destino, cadena_ip);
                mac_cadena(trama_recibida+6, cadena_mac);
                
                sprintf(sentencia, "INSERT INTO direcciones (ip, mac) VALUES ('%s', '%s')", cadena_ip, cadena_mac);

                if (sqlite3_exec(db, sentencia, NULL, NULL, NULL) != SQLITE_OK) {
                    return gestionar_error(db);
                }
            }
        }

        /* Limpieza de las tramas para su uso en la siguiente iteración */
        limpiar_trama(trama_enviada);
        limpiar_trama(trama_recibida);
    }

    gettimeofday(&final, NULL);

    segundos = final.tv_sec - inicio.tv_sec;
    microsegundos = final.tv_usec - inicio.tv_usec;
    tiempo_espera = ((segundos) * 1000 + microsegundos/1000.0) + 0.5;

    /* ----------------------------------------- Mostrar datos ---------------------------------------- */
    strcpy(sentencia, "SELECT * FROM direcciones");

    printf(" ---------------------------------------");
    printf("\n |   Direccion IP  |   Direccion MAC   |\n");
    printf(" |-------------------------------------|\n");

    if (sqlite3_exec(db, sentencia, mostrar_db, NULL, NULL) != SQLITE_OK) {
        return gestionar_error(db);
    }

    printf(" ---------------------------------------\n");
    printf("\nTiempo transcurrido: %ld milisegundos\n", tiempo_espera);

    close(un_socket);
    sqlite3_close(db);

    return 0;
}
