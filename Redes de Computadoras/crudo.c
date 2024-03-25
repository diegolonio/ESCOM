#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <net/ethernet.h>
#include <net/if.h>

int indice;
unsigned char mac_broadcast[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
unsigned char mac_origen[6], ip_origen[4], mascara_subred[4];
unsigned char ethertype[2] = {0x0c, 0x0c};
unsigned char trama_enviada[1514], trama_recibida[1514];
unsigned char prueba[4] = {8, 8, 8, 8};

void obtener_datos(int);
void estructura_trama(unsigned char *);
void enviar_trama(int, int, unsigned char *);
void imprimir_trama(unsigned char *, int);
void recibir_trama(int, unsigned char *);

int main()
{
    int descriptor_socket;

    // Apertura del socket
    descriptor_socket = socket(AF_PACKET, SOCK_RAW, htons(ETH_P_ALL));

    if (descriptor_socket == -1) {
        perror("Error al abrir el socket.");
        exit(0);
    } else {
        obtener_datos(descriptor_socket);
        estructura_trama(trama_enviada);
        enviar_trama(descriptor_socket, indice, trama_enviada);
        recibir_trama(descriptor_socket, trama_recibida);
    }

    close(descriptor_socket);

    return 0;
}

void obtener_datos(int descriptor_socket)
{
    int i;
    struct ifreq nic;
    unsigned char nombre[20];

    printf("Ingresa el nombre de la interfaz: ");
    fgets(nombre, sizeof(nombre), stdin);

    nombre[strlen(nombre)-1] = '\0'; // Eliminar el salto de línea al final del nombre
    strcpy(nic.ifr_name, nombre);

    // Obtener el índice la interfaz a partir del nombre
    if (ioctl(descriptor_socket, SIOCGIFINDEX, &nic) == -1) {
        perror("Error al obtener el indice");
        exit(0);
    } else {
        indice = nic.ifr_ifindex;
        printf("Indice: %d\n", indice);

        // Obtener la dirección de hardware
        if (ioctl(descriptor_socket, SIOCGIFHWADDR, &nic) == -1) {
            perror("Error al obtener la dirección MAC");
            exit(0);
        } else {
            memcpy(mac_origen, nic.ifr_hwaddr.sa_data, 6);
            printf("Direccion MAC: ");

            for (i = 0; i < 6; i++) {
                printf("%.2x", mac_origen[i]);

                if (i < 5) {
                    printf(":");
                }
            }
            
            printf("\n");

            // Obtener la dirección IP
            if (ioctl(descriptor_socket, SIOCGIFADDR, &nic) == -1) {
                perror("Error al obtener la direccion IP");
                exit(0);
            } else {
                printf("Direccion IP: ");
                memcpy(ip_origen, nic.ifr_addr.sa_data+2, 4);

                for (i = 0; i < 4; i++) {
                    printf("%d", ip_origen[i]);

                    if (i < 3) {
                        printf(".");
                    }
                }

                printf("\n");

                // Obtener la máscara de subred
                if (ioctl(descriptor_socket, SIOCGIFNETMASK, &nic) == -1) {
                    perror("Error al obtener la mascara de subred");
                    exit(0);
                } else {
                    printf("Mascara de subred: ");
                    memcpy(mascara_subred, nic.ifr_netmask.sa_data+2, 4);

                    for (i = 0; i < 4; i++) {
                        printf("%d", mascara_subred[i]);

                        if (i < 3) {
                            printf(".");
                        }
                    }

                    printf("\n");
                }
            }
        }
    }
}

void estructura_trama(unsigned char *trama)
{
    // Dirección destino
    memcpy(trama+0, mac_broadcast, 6);
    // Dirección fuente
    memcpy(trama+6, mac_origen, 6);
    // Ethertype
    memcpy(trama+12, ethertype, 2);
    // Datos
    memcpy(trama+14, "Diego Armando Apolonio Villegas", 32);
}

void enviar_trama(int descriptor_socket, int indice, unsigned char *trama)
{
    int enviado;
    struct sockaddr_ll interfaz;

    memset(&interfaz, 0x00, sizeof(interfaz)); // Borramos los datos de la estructura
    // Configuramos los datos de la estructura
    interfaz.sll_family = AF_PACKET;
    interfaz.sll_protocol = htons(ETH_P_ALL);
    interfaz.sll_ifindex = indice;

    // Envío de la trama
    enviado = sendto(descriptor_socket, trama, 60, 0, (struct sockaddr *)&interfaz, sizeof(interfaz));

    if (enviado == -1) {
        perror("La trama no pudo ser enviada");
        exit(0);
    } else {
        perror("La trama ha sido enviada con exito");
    }
}

void imprimir_trama(unsigned char *paquete, int tamanio)
{
    int i;

    for (i = 0; i < tamanio; i++) {
        if (i % 16 == 0) {
            printf("\n");
        }

        printf("%.2x ", paquete[i]);
    }

    printf("\n");
}

void recibir_trama(int descriptor_socket, unsigned char *trama)
{
    int recibido, bandera = 0;
    struct timeval inicio, final;
    long tiempo_espera = 0, segundos, microsegundos;    

    gettimeofday(&inicio, NULL);

    /* NOTA: si requiero conocer la información de la interfaz de red desde la
     * que me fueron enviados los datos entonces debo definir su estructura e
     * incluirla en los parámetros de la función recvfrom().
     */
    while (tiempo_espera < 5000) {
        recibido = recvfrom(descriptor_socket, trama, 1514, MSG_DONTWAIT, NULL, 0);

        if (recibido == -1) {
            perror("Error al recibir la trama");
            exit(0);
        } else {
            // Imprimir solo las tramas cuya dirección fuente o dirección destino sea la mía
            if (memcmp(trama+0, mac_broadcast, 6) == 0 && memcmp(trama+6, mac_origen, 6) == 0) {
                imprimir_trama(trama, recibido);
                bandera = 1;
            }
        }

        gettimeofday(&final, NULL);

        segundos = final.tv_sec - inicio.tv_sec;
        microsegundos = final.tv_usec - inicio.tv_usec;
        tiempo_espera = ((segundos) * 1000 + microsegundos/1000.0) + 0.5;

        if (bandera == 1) {
            break;
        }
    }
    
    printf("Tiempo transcurrido: %ld milisegundos\n", tiempo_espera);
}
