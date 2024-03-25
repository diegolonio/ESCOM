#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

int main()
{
    int descriptor_socket, asociados, enviado, recibido, tam_cliente, enviar_otro_mensaje;
    struct sockaddr_in direccion_servidor, direccion_cliente;
    unsigned char buffer[512];

    // Apertura del socket del servidor
    descriptor_socket = socket(AF_INET, SOCK_DGRAM, 0);

    if (descriptor_socket == -1) {
        perror("Error al abrir el socket");
        exit(0);
    } else {
        // Definición de la dirección del servidor
        direccion_servidor.sin_family = AF_INET;
        direccion_servidor.sin_port = htons(8080);
        direccion_servidor.sin_addr.s_addr = INADDR_ANY;

        // Asociación de la dirección al socket del servidor
        asociados = bind(descriptor_socket, (struct sockaddr *)&direccion_servidor, sizeof(direccion_servidor));

        if (asociados == -1) {
            perror("Error al asociar la direccion y el socket");
            exit(0);
        } else {
            tam_cliente = sizeof(direccion_cliente);

            do {
                // Recepción de mensaje
                recibido = recvfrom(descriptor_socket, buffer, 512, 0, (struct sockaddr *)&direccion_cliente, &tam_cliente);

                if (recibido == -1) {
                    perror("Error al recibir el mensaje");
                    exit(0);
                } else {
                    printf("Cliente: %s", buffer);

                    printf("Servidor: ");
                    fgets(buffer, sizeof(buffer), stdin);

                    // Envío de mensaje
                    enviado = sendto(descriptor_socket, buffer, sizeof(buffer)+1, 0, (struct sockaddr *)&direccion_cliente, sizeof(direccion_cliente));

                    if (enviado == -1) {
                        perror("Error al enviar el mensaje");
                        exit(0);
                    }
                }

                printf("\nDeseas mandar otro mensaje? (s/n): ");
                enviar_otro_mensaje = fgetc(stdin);
                printf("\n");
                
                fgetc(stdin); // Consumimos el caracter '\n' de la entrada estándar
            } while (enviar_otro_mensaje == 's' || enviar_otro_mensaje == 'S');
        }
    }

    close(descriptor_socket);

    return 0;
}
