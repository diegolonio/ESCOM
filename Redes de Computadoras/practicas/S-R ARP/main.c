#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <net/ethernet.h>
#include "../../include/redes.h"

int main()
{
    int un_socket, i;
    Interfaz interfaz;
    char nombre_interfaz[10], cadena_ip_destino[17];
    unsigned char ip_destino[4], mac_destino[6], ethertype[2];
    unsigned char subred[4], subred_destino[4];
    unsigned char trama_enviada[1514], trama_recibida[1514];
    long tiempo_espera;
    ssize_t longitud;

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
    aplicar_mascara(interfaz.direccion_ip, interfaz.mascara_subred, subred);

    printf("Ingresa la dirección IP destino (");

    for (i = 0; i < 3; i++) {
        printf("%d.", subred[i]);
    }

    printf("x): ");

    /* Obtención del usuario de la dirección IP destino */
    fgets(cadena_ip_destino, sizeof(cadena_ip_destino), stdin);
    cadena_ip_destino[strlen(cadena_ip_destino)-1] = '\0';

    if (ip_arreglo(ip_destino, cadena_ip_destino) == false) {
        close(un_socket);
        fprintf(stderr, "%s", "Error: Dirección IP no válida\n");
        exit(1);
    }

    /* Aplicación de la máscara de subred a la dirección IP destino */
    aplicar_mascara(ip_destino, interfaz.mascara_subred, subred_destino);

    if (misma_subred(subred, subred_destino) == false) {
        close(un_socket);
        fprintf(stderr, "%s", "Error: La dirección IP ingresada no pertenece a la subred\n");
        exit(1);
    }

    /* Dirección MAC de broadcast */
    for (i = 0; i < 6; i++) {
        mac_destino[i] = 0xff;
    }

    /* Ethertype 0x0806 (ARP) */
    ethertype[0] = 0x08;
    ethertype[1] = 0x06;

    /* Estructuración del encabezado MAC del mensaje de ARP */
    encabezado_mac(trama_enviada, mac_destino, interfaz.direccion_mac, ethertype);
    /* Estructuración del mensaje de ARP */
    mensaje_arp(trama_enviada, interfaz, ip_destino);

    /* Impresión de la trama enviada */
    printf("\n**************** Trama enviada ****************\n");
    imprimir_trama(trama_enviada, 42);
    printf("\n");

    if (enviar_trama(un_socket, interfaz.indice, trama_enviada, 42) == false) {
        close(un_socket);
        perror("Error");
        exit(errno);
    }

    /* Recepción de la respuesta de ARP */
    if (recibir_trama(un_socket, trama_recibida, trama_enviada, filtro_arp, &tiempo_espera, &longitud) == false) {
        close(un_socket);
        printf("\nTiempo de espera agotado. Pasaron %ld milisegundos.\n", tiempo_espera);
        exit(0);
    }

    /* Impresión de la trama recibida */
    printf("\n*************** Trama recibida ****************\n");
    imprimir_trama(trama_recibida, longitud);
    printf("\n");

    /* Guardado de la dirección MAC del destino */
    memcpy(mac_destino, trama_recibida+6, 6);

    printf("\nLa dirección MAC de ");
    imprimir_ip(ip_destino);
    printf(" es ");
    imprimir_mac(mac_destino);
    printf("\n");

    /* Cierre del socket */
    close(un_socket);

    return 0;
}
