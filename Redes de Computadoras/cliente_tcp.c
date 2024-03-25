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
    int descriptor_socket, conectado, enviado, recibido, enviar_otro_mensaje;
    struct sockaddr_in cliente, servidor;
    unsigned char buffer[512];

    // Apertura del socket del cliente
    descriptor_socket = socket(AF_INET, SOCK_STREAM, 0);

    if (descriptor_socket == -1) {
        perror("Error al abrir el socket");
        exit(0);
    }

    // Definición de los datos del servidor
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(8080);
    servidor.sin_addr.s_addr = inet_addr("192.168.1.94");

    // Intento de conexión con el servidor
    conectado = connect(descriptor_socket, (struct sockaddr *)&servidor, sizeof(servidor));

    if (conectado == -1) {
        perror("Error al intentar establecer la conexión con el servidor");
        close(descriptor_socket);
        exit(0);
    }

    do {
        printf("Tu: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Envío de mensaje
        enviado = write(descriptor_socket, buffer, sizeof(buffer)+1);

        if (enviado == -1) {
            perror("Error al enviar el mensaje");
            shutdown(descriptor_socket, 2);
            close(descriptor_socket);
            exit(0);
        }

        // Recepción de mensaje
        recibido = read(descriptor_socket, buffer, sizeof(buffer));

        if (recibido == -1) {
            perror("Error al recibir el mensaje");
            shutdown(descriptor_socket, 2);
            close(descriptor_socket);
            exit(0);
        }
        
        printf("Servidor: %s", buffer);

        printf("\nDeseas mandar otro mensaje? (s/n): ");
        enviar_otro_mensaje = fgetc(stdin);
        printf("\n");

        fgetc(stdin); // Consumimos el caracter '\n' de la entrada estándar
    } while (enviar_otro_mensaje == 's' || enviar_otro_mensaje == 'S');

    shutdown(descriptor_socket, 2); // Cierre de la conexión TCP
    close(descriptor_socket);

    return 0;
}
