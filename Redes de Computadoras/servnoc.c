#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 

int main()
{
    int udp_socket; // Descriptor del socket del servidor
    int lbind, tamanio, tam_cliente;
    struct sockaddr_in servidor, cliente;
    unsigned char mensaje[100] = "Hola mundo servidor";
    unsigned char buffer[512];

    // Apertura del socket del servidor
    udp_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (udp_socket == -1) {
        perror("Error al abrir el socket");
        exit(0);
    } else {
        perror("Exito al abrir el socket");

        // Definición de la dirección que tendrá el socket del servidor
        servidor.sin_family = AF_INET;
        servidor.sin_port = htons(8080);
        servidor.sin_addr.s_addr = INADDR_ANY;

        // Asociación de la dirección con el socket del servidor
        lbind = bind(udp_socket, (struct sockaddr *)&servidor, sizeof(servidor));

        if (lbind == -1) {
            perror("Error en el bind");
            exit(0);
        } else {
            perror("Exito en el bind");

            // Recepción de datos
            tam_cliente = sizeof(cliente);
            tamanio = recvfrom(udp_socket, buffer, 512, 0, (struct sockaddr *)&cliente, &tam_cliente);

            if (tamanio == -1) {
                perror("Erro al recibir");
                exit(0);
            } else {
                perror("Exito al recibir");
                printf("El mensaje es: %s\n", buffer);
            }
        }
    }

    close(udp_socket);

    return 0;
}
