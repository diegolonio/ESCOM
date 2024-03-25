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
    int un_socket, i, j, conteo[3], tiempos[4], promedio;
    Interfaz interfaz;
    unsigned char trama_enviada[1514], trama_recibida[1514];
    unsigned char ip_destino[4], mac_destino[6], ip_gateway[4];
    unsigned char subred_fuente[4], subred_destino[4], ethertype[2];
    unsigned char datos[32] = "Diego Armando Apolonio Villegas", identificador[2];
    bool bandera = false;
    ssize_t longitud;
    struct timeval semilla, inicio, final;
    long tiempo_espera = 0, segundos, microsegundos;

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

    printf("Haciendo ping a ");
    imprimir_ip(ip_destino);
    printf(" con 31 bytes de datos:\n");

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

    for (i = 0; i < 4; i++) {
        /* Envío del mensaje de ARP */
        if (enviar_trama(un_socket, interfaz.indice, trama_enviada, 42) == false) {
            fprintf(stderr, "%s", "Error al enviar la trama\n");
            close(un_socket);
            exit(1);
        }

        /* Recepción del mensaje de ARP */
        if (recibir_trama(un_socket, trama_recibida, trama_enviada, filtro_arp, NULL, &longitud) == false) {
            printf("Respuesta desde ");
            imprimir_ip(ip_destino);
            printf(": Host de destino inaccesible.\n");
            bandera = false;
        } else {
            bandera = true;
            break;
        }
    }

    if (bandera == false) {
        printf("\nEstadísticas de ping para ");
        imprimir_ip(ip_destino);
        printf(":\n    Paquetes: enviados = 4, recibidos = 4, perdidos = 0\n");
        printf("    (0%% perdidos)\n");
        close(un_socket);
        exit(1);
    }

    /* Guardado de la dirección MAC a la cual se enviarán las trama de ping */
    memcpy(mac_destino, trama_recibida+6, 6);

    /* Limpieza de las tramas para uso posterior */
    limpiar_trama(trama_enviada);
    limpiar_trama(trama_recibida);

    /* Inicialización del conteo de paquetes */
    for (j = 0; j < 3; j++) {
        conteo[j] = 0;
    }
    
    /* Ethertype = 0x0800 (IP) */
    ethertype[0] = 0x08;
    ethertype[1] = 0x00;

    /* Encabezado MAC de los pings */
    encabezado_mac(trama_enviada, mac_destino, interfaz.direccion_mac, ethertype);

    for (i = i; i < 4; i++) {
        /* Encabezado IP del ping */
        encabezado_ip(trama_enviada, interfaz.direccion_ip, ip_destino, 59, 128, 1);
        /* Encabezado ICMP del ping */
        encabezado_icmp(trama_enviada, datos, 39, identificador);

        gettimeofday(&inicio, NULL);

        /* Envío del ping actual */
        if (enviar_trama(un_socket, interfaz.indice, trama_enviada, 73) == true) {
            conteo[0] = conteo[0] + 1; // La trama se envió

            /* Recepción de la respuesta */
            if (recibir_trama(un_socket, trama_recibida, trama_enviada, filtro_icmp, NULL, &longitud) == false) {
                printf("Tiempo de espera agotado para esta solicitud.\n");
                conteo[2] = conteo[2] + 1; // La trama se perdió
            } else {
                gettimeofday(&final, NULL);

                conteo[1] = conteo[1] + 1; // La trama se recibió

                segundos = final.tv_sec - inicio.tv_sec;
                microsegundos = final.tv_usec - inicio.tv_usec;
                tiempo_espera = ((segundos) * 1000 + microsegundos/1000.0) + 0.5;
                tiempos[i] = tiempo_espera;

                printf("Respuesta desde ");
                imprimir_ip(ip_destino);
                printf(": bytes=%.2ld tiempo=%.2dms TTL=%.2d\n", longitud-14-20-8, tiempos[i], *(trama_recibida+22));
            }
        }
    }

    /* Ordenamiento de los tiempos de ida y vuelva en orden ascendente */
    selection_sort(tiempos, 4);

    /* Tiempo promedio de ida y vuelta de los pings */
    j = 0;

    for (i = 0; i < 4; i++) {
        if (tiempos[i] != 0) {
            j = j + 1;  
        }
    }

    if (j > 0) {
        promedio = (tiempos[0] + tiempos[1] + tiempos[2] + tiempos[3]) / j;
    }

    /* Presentación de las estadísticas */
    printf("\nEstadísticas de ping para ");
    imprimir_ip(ip_destino);
    printf(":\n");
    printf("    Paquetes: enviados = %d, recibidos = %d, perdidos = %d\n", conteo[0], conteo[1], conteo[2]);
    printf("    (%d%% perdidos)\n", (int)(((float)conteo[2]/(float)conteo[0])*100.0));

    /* Presentación de las estadísitcas de tiempos de ida y vuelta de los pings */
    if (conteo[2] < conteo[0]) {
        for (i = 0; i < 4; i++) {
            if (tiempos[i] != 0) {
                break;
            }
        }

        printf("Tiempos aproximados de ida y vuelta en milisegundos:\n");
        printf("    Mínimo = %dms, Máximo = %dms, Media = %dms\n", tiempos[i], tiempos[3], promedio);
    }

    close(un_socket);

    return 0;
}
