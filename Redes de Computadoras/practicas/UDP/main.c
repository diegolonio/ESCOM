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
#include "../../include/udp.h"
#include "../../include/redes.h"

int main(int argc, char **argv)
{
    if (argc < 5 || argc > 5) {
        fprintf(stderr, "%s", "Los argumentos especificados no son correctos\n");
        exit(1);
    }

    char nombre_interfaz[10], cadena_ip_destino[16];
    int un_socket, i, j, puerto_origen;
    Interfaz interfaz;
    unsigned char ip_destino[4], mac_destino[6], ip_gateway[4];
    unsigned char trama_enviada[1514], trama_recibida[1514];
    unsigned char subred_fuente[4], subred_destino[4], ethertype[2];
    unsigned char datos[28] = {0x17, 0xae, 0x01, 0x00, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x03, 0x77, \
                               0x77, 0x77, 0x03, 0x69, 0x70, 0x6e, 0x02, 0x6d, 0x78, 0x00, 0x00, 0x01, 0x00, 0x01};
    bool bandera = false;
    struct timeval semilla;
    long tiempo_espera;
    ssize_t longitud;

    /* Obtención del nombre de la interfaz de red desde los argumentos del comando */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-i") == 0) {
            strcpy(nombre_interfaz, argv[i+1]);
            break;
        }
    }

    if (i == argc) {
        fprintf(stderr, "%s", "No se ha especificado el nombre de la interfaz de red\n");
        exit(1);
    }

    /* Obtención del nombre de la dirección IP destino desde los argumentos del comando */
    for (i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-a") == 0) {
            strcpy(cadena_ip_destino, argv[i+1]);
            break;
        }
    }

    if (i == argc) {
        fprintf(stderr, "%s", "No se ha especificado la direccion IP del destino\n");
        exit(1);
    }

    /* Semilla para los numeros aleatorios de identificadores y secuencias */
    gettimeofday(&semilla, NULL);
    srandom(semilla.tv_sec);

    /* Apertura del socket */
    un_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (un_socket == -1) {
        perror("Error");
        exit(errno);
    }

    /* Obtención de los datos de la interfaz de red */
    if (obtener_datos(un_socket, nombre_interfaz, &interfaz) == false) {
        close(un_socket);
        perror("Error");
        exit(errno);
    }

    /* Obtención de los octetos de la dirección IP destino */
    if (ip_arreglo(ip_destino, cadena_ip_destino) == false) {
        close(un_socket);
        fprintf(stderr, "%s", "Error: Dirección IP no válida\n");
        exit(1);
    }

    /* Obtención de las subredes de la fuente y del destino */
    aplicar_mascara(interfaz.direccion_ip, interfaz.mascara_subred, subred_fuente);
    aplicar_mascara(ip_destino, interfaz.mascara_subred, subred_destino);

    /* Etherype = 0x0806 (ARP) */
    ethertype[0] = 0x08;
    ethertype[1] = 0x06;

    /* Dirección MAC de broadcast */
    for (i = 0; i < 6; i++) {
        mac_destino[i] = 0xff;
    }

    /* Encabezado MAC del mensaje de ARP */
    encabezado_mac(trama_enviada, mac_destino, interfaz.direccion_mac, ethertype);

    if (misma_subred(subred_fuente, subred_destino) == true) {
        /* Estructuración del mensaje de ARP para obtener la dirección MAC del destino */
        mensaje_arp(trama_enviada, interfaz, ip_destino);
    } else {
        if (obtener_gateway(ip_gateway) == false) { // ¿No se pudo obtener la IP de la puerta de enlace?
            fprintf(stderr, "%s", "Error al obtener la direccion IP de la puerta de enlace\n");
            close(un_socket);
            exit(1);
        }

        /* Estructuración del mensaje de ARP para obtener la dirección MAC de la puerta de enlace */
        mensaje_arp(trama_enviada, interfaz, ip_gateway);
    }

    /* Envío del mensaje de ARP */
    if (enviar_trama(un_socket, interfaz.indice, trama_enviada, 42) == false) {
        fprintf(stderr, "%s", "Error al enviar la trama\n");
        close(un_socket);
        exit(1);
    }

    /* Recepción del mensaje de ARP */
    if (recibir_trama(un_socket, trama_recibida, trama_enviada, filtro_arp, NULL, NULL) == false) {
        fprintf(stderr, "%s", "Host de destino inaccesible.\n");
        close(un_socket);
        close(1);
    }

    /* Guardado de la dirección MAC a la cual enviaremos la trama */
    memcpy(mac_destino, trama_recibida+6, 6);

    /* Limpieza de las tramas para uso posterior */
    limpiar_trama(trama_enviada);
    limpiar_trama(trama_recibida);
    
    /* Ethertype = 0x0800 (IP) */
    ethertype[0] = 0x08;
    ethertype[1] = 0x00;

    /* Obtención de un número de puerto efímero */
    puerto_origen = obtener_puerto_efimero();

    /* Encabezado MAC de la solicitud de DNS */
    encabezado_mac(trama_enviada, mac_destino, interfaz.direccion_mac, ethertype);
    
    /* Encabezado IP de la solicitud de DNS */
    encabezado_ip(trama_enviada, interfaz.direccion_ip, ip_destino, 56, 64, 17);
    
    /* Encabezado UDP de la solicitud de DNS */
    encabezado_udp(trama_enviada, puerto_origen, 53, 36, datos);
    
    printf("\n**************** Trama enviada ****************\n");
    imprimir_trama(trama_enviada, 70);
    printf("\n");

    /* Envío de la solicitud de DNS */
    if (enviar_trama(un_socket, interfaz.indice, trama_enviada, 70) == false) {
        close(un_socket);
        perror("Error");
        exit(errno);
    }

    if (recibir_trama(un_socket, trama_recibida, trama_enviada, filtro_udp, &tiempo_espera, &longitud) == false) {
        printf("\nTiempo de espera agotado. Pasaron %ld milisegundos.\n", tiempo_espera);
        close(un_socket);
        exit(0);
    }

    printf("\n**************** Trama recibida ***************\n");
    imprimir_trama(trama_recibida, longitud);
    printf("\n\nLa dirección IP de www.ipn.mx es: ");
    imprimir_ip(trama_recibida+96);
    printf("\n\nTrama recibida después de %ld milisegundos.\n", tiempo_espera);

    close(un_socket);

    return 0;
}
