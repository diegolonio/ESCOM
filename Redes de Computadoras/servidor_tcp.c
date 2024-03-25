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
    int socket_maestro, nuevo_socket, asociados, len_cliente, enviado, recibido, enviar_otro_mensaje;
    struct sockaddr_in servidor, cliente;
    unsigned char buffer[512];

    // Apertura del socket del servidor
    socket_maestro = socket(AF_INET, SOCK_STREAM, 0);

    if (socket_maestro == -1) {
        perror("Error al abrir el socket");
        exit(0);
    }

    // Definición de los datos del servidor
    servidor.sin_family = AF_INET;
    servidor.sin_port = htons(8080);
    servidor.sin_addr.s_addr = INADDR_ANY;

    // Asociación del socket maestro con el servidor
    asociados = bind(socket_maestro, (struct sockaddr *)&servidor, sizeof(servidor));

    if (asociados == -1) {
        perror("Error al asociar el socket con el servidor");
        close(socket_maestro);
        exit(0);
    }

    // Configuración del socket maestro en modo de escucha de solicitudes de conexión (pasivo)
    // La cola de solicitudes de conexión es de máximo 5
    if (listen(socket_maestro, 5) == -1) {
        perror("No se puede escuchar a través del socket");
        close(socket_maestro);
        exit(0);
    }

    len_cliente = sizeof(cliente);

    // Limpieza de los datos en la estructura del cliente
    memset(&cliente, 0x00, len_cliente);

    // A la escucha de una solicitud de conexión...
    nuevo_socket = accept(socket_maestro, (struct sockaddr *)&cliente, &len_cliente);

    if (nuevo_socket == -1) {
        perror("Ocurrió un error al intentar establecer la conexión");
        close(socket_maestro);
        exit(0);
    }

    do {
        // Recepción de mensaje
        recibido = read(nuevo_socket, buffer, sizeof(buffer));

        if (recibido == -1) {
            perror("Error al recibir el mensaje");
            shutdown(nuevo_socket, 2);
            close(nuevo_socket);
            close(socket_maestro);
            exit(0);
        }

        printf("Cliente: %s", buffer);

        printf("Servidor: ");
        fgets(buffer, sizeof(buffer), stdin);

        // Envío de mensaje
        enviado = write(nuevo_socket, buffer, sizeof(buffer));

        if (enviado == -1) {
            perror("Error al enviar el mensaje");
            shutdown(nuevo_socket, 2);
            close(nuevo_socket);
            close(socket_maestro);
            exit(0);
        }

        printf("\nDeseas mandar otro mensaje? (s/n): ");
        enviar_otro_mensaje = fgetc(stdin);
        printf("\n");
        
        fgetc(stdin); // Consumimos el caracter '\n' de la entrada estándar
    } while (enviar_otro_mensaje == 's' || enviar_otro_mensaje == 'S');

    shutdown(nuevo_socket, 2); // Cierre de la conexión TCP
    close(socket_maestro);
    close(nuevo_socket);

    return 0;
}
