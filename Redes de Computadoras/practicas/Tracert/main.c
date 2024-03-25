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
#include "../../include/ping.h"
#include "../../include/redes.h"

int main(int argc, char **argv)
{
    if (argc < 5 || argc > 5) {
        fprintf(stderr, "%s", "Los argumentos especificados no son correctos\n");
        exit(1);
    }

    char nombre_interfaz[10], cadena_ip_destino[16];
    int un_socket, i, j, perdidos;
    Interfaz interfaz;
    unsigned char trama_enviada[1514], trama_recibida[1514];
    unsigned char ip_destino[4], mac_destino[6], ip_gateway[4], ip_route[4];
    unsigned char subred_fuente[4], subred_destino[4], ethertype[2];
    unsigned char datos[32] = "Diego Armando Apolonio Villegas", identificador[2];
    bool bandera = false;
    struct timeval semilla;
    long tiempo_espera;

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
        exit(0);
    }

    /* Semilla para los numeros aleatorios de identificadores y secuencias */
    gettimeofday(&semilla, NULL);
    srandom(semilla.tv_sec);

    /* Generación del identificador del proceso actual */
    identificador[0] = (int)random();
    identificador[1] = (int)random();

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

    printf("Traza a ");
    imprimir_ip(ip_destino);
    printf(" sobre caminos de 30 saltos como máximo.\n\n");

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
    if (recibir_trama(un_socket, trama_recibida, trama_enviada, filtro_arp, NULL, NULL) == true) {
        bandera = true;
    }

    if (bandera == false) {
        printf("    1 ");
        imprimir_ip(interfaz.direccion_ip);
        printf(" informes: Host de destino inaccesible.\n\nTraza completa.\n");
        close(un_socket);
        exit(1);
    }

    /* Guardado de la dirección MAC a la cual se enviarán las trama de ping */
    memcpy(mac_destino, trama_recibida+6, 6);

    /* Limpieza de las tramas para uso posterior */
    limpiar_trama(trama_enviada);
    limpiar_trama(trama_recibida);
    
    /* Ethertype = 0x0800 (IP) */
    ethertype[0] = 0x08;
    ethertype[1] = 0x00;

    /* Encabezado MAC de los pings */
    encabezado_mac(trama_enviada, mac_destino, interfaz.direccion_mac, ethertype);

    for (i = 1; i <= 30; i++) {
        /* Encabezado IP del ping */
        encabezado_ip(trama_enviada, interfaz.direccion_ip, ip_destino, 59, i, 1);

        if (i < 10) {
            printf ("     %d ", i);
        } else {
            printf ("    %d ", i);
        }

        perdidos = 0;

        /* Envío de los 3 pings por cada TTL */
        for (j = 1; j <= 3; j++) {
            /* Encabezado ICMP del ping */
            encabezado_icmp(trama_enviada, datos, 39, identificador);

            /* Envío del echo */
            if (enviar_trama(un_socket, interfaz.indice, trama_enviada, 73) == true) {
                /* Recepción del reply */
                if (recibir_trama(un_socket, trama_recibida, trama_enviada, filtro_icmp, &tiempo_espera, NULL) == true) {
                    /* Guardado de la dirección IP del router */
                    memcpy(ip_route, trama_recibida+26, 4);

                    if (tiempo_espera < 10) {
                        if (tiempo_espera < 1) {
                            printf("  <1 ms ");
                        } else {
                            printf("   %ld ms ", tiempo_espera);
                        }
                    } else if (tiempo_espera >= 10 && tiempo_espera < 100) {
                        printf("  %ld ms ", tiempo_espera);
                    } else {
                        printf(" %ld ms ", tiempo_espera);
                    }
                } else {
                    perdidos = perdidos + 1;
                    printf("   *    ");
                }
            }
        }

        if (perdidos < 3) {
            printf(" ");
            imprimir_ip(ip_route);
            printf("\n");
        } else {
            printf(" Tiempo de espera agotado para esta solicitud.\n");
        }

        /* Si la terminal destino responde */
        if (memcmp(ip_route, ip_destino, 4) == 0) {
            break;
        }
    }

    printf("\nTraza completa.\n");

    close(un_socket);

    return 0;
}
