//#3
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>
#include <string.h>

int main()
{
    int udp_socket; // Descriptor del socket
    int lbind, tamanio;
    struct sockaddr_in local, remota;
    unsigned char mensaje[100] = "Hola mundo cliente";

    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (udp_socket == -1) {
         perror("Error al abrir el socket");
         exit(0);
    } else {
        perror("Exito al abrir el socket");

        // Definición de la estructura local
        local.sin_family = AF_INET;
        local.sin_port = htons(8080); // 0 para obtener una dirección aleatoria
        local.sin_addr.s_addr = INADDR_ANY;

        // Asociamos la dirección con el socket
        lbind = bind(udp_socket, (struct sockaddr *)&local, sizeof(local)); 

        if (lbind == -1) {
            perror("Error en el bind");
            exit(0);
        } else {
            perror("Exito en el bind");

            // Definición de la estructura remota
            remota.sin_family = AF_INET;
            remota.sin_port = htons(9090);
            remota.sin_addr.s_addr = inet_addr("10.0.2.15");

            // Envío del mensaje
            tamanio = sendto(udp_socket, mensaje, strlen(mensaje)+1, 0, (struct sockaddr *)&remota, sizeof(remota));

            if (tamanio == -1) {
                perror("Error al enviar");
                exit(0);
            } else {
                perror("Exito al enviar");
            }
        }
    }

    close(udp_socket);

    return 0;
}
