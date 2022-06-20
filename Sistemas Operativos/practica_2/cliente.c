#include <stdio.h>
#include <stdbool.h>
#include "../include/unidad2.h"

int main()
{
    int contador;
    int llave, archivo;
    int sem_servidor, sem_claves, sem_cliente, sem_hilo;
    int *claves_comunicacion_hilo = NULL, *asientos = NULL, *chat = NULL;
    char numero_archivo[4], archivo_shm[8], archivo_sem_cliente[16], archivo_sem_hilo[13];
    char disponibilidad[100], nosi = 's';
    bool comprado, comprar, apartado, apartar, otra_compra = true;
    int asiento;

    /* ------------------ Memorias compartidas del servidor ------------------- */

    /* Conexión al arreglo de asientos */
    asientos = (int *)shm(sizeof(int)*20, "shm_asientos", 'u');

    /* Conexión a la memoria compartida para la asignación de claves de comunicación con el hilo */
    claves_comunicacion_hilo = (int *)shm(sizeof(int)*2, "shm_claves", 'v');

    /* ------------------------ Semáforos del servidor ------------------------ */

    /* Semáforo del servidor */
    sem_servidor = sem("sem_servidor", 'w');

    /* Semáforo del cliente */
    sem_cliente = sem("sem_cliente", 'x');

    /* Semáforo de claves */
    sem_claves = sem("sem_claves", 'y');

    /* ----------------- Obtención de claves de comunicación ------------------ */

    /* El servidor se ocupa */
    down(sem_servidor, 0);

    up(sem_claves, 0);
    down(sem_cliente, 0);

    /* Obtención de las claves del servidor */
    llave = claves_comunicacion_hilo[0];
    archivo = claves_comunicacion_hilo[1];

    up(sem_claves, 0);
    down(sem_cliente, 0);

    /* Se libera el servidor */
    up(sem_servidor, 0);

    /* ------------------- Obtención de nombres de archivos ------------------- */

    /* Conversión del número de archivo asignado a cadena */
    sprintf(numero_archivo, "%d", archivo);

    /* Archivo: shm_xxx */
    strcpy(archivo_shm, "shm_");
    strcat(archivo_shm, numero_archivo);

    /* Archivo: sem_cliente_xxx */
    strcpy(archivo_sem_cliente, "sem_cliente_");
    strcat(archivo_sem_cliente, numero_archivo);

    /* Archivo: sem_hilo_xxx */
    strcpy(archivo_sem_hilo, "sem_hilo_");
    strcat(archivo_sem_hilo, numero_archivo);

    /* -------------------- Memorias compartidas del hilo --------------------- */

    /* Chat */
    chat = (int *)shm(sizeof(int)*2, archivo_shm, llave);

    /* -------------------------- Semáforos del hilo -------------------------- */

    /* Semáforo del hilo */
    sem_hilo = sem(archivo_sem_hilo, llave+400);

    /* Semáforo del cliente */
    sem_cliente = sem(archivo_sem_cliente, llave+500);

    /* ------------------------ Inicio de comunicación ------------------------ */

    while (otra_compra == true) {
        system("clear");
        
        /* El usuario ingresa el asiento que quiere */
        printf ("Esta es la lista de asientos:\n\n");

        for (contador = 19; contador >= 0; contador--) {
            if (semval(asientos[contador]) == 1) {
                strcat(disponibilidad, "(Disponible)");
            } else {
                strcat(disponibilidad, "  (Ocupado) ");
            }

            printf("|Asiento %02d|", contador+1);

            if (contador % 2 == 0) {
                printf("   ");
                strcat(disponibilidad, "   ");
            }

            if (contador % 4 == 0) {
                printf("\n");
                printf("%s\n\n", disponibilidad);
                strcpy(disponibilidad, "\0");
            }
        }

        printf("Introduce el número de asiento que deseas comprar: ");
        scanf("%d", &asiento);
        fgetc(stdin);

        /* Se pasa el asiento elegido al hilo */
        chat[0] = asiento-1;

        up(sem_hilo, 0);
        down(sem_cliente, 0);

        /* Se obtiene el estado del asiento */
        apartado = chat[1];

        /* Se determina si el asiento se compra o no */
        if (apartado == true) {
            printf("\nEl asiento %d ya ha sido apartado.\n", asiento);
            comprar = false;
        } else {
            printf("\nConfirmar la compra del asiento %d? (s/n): ", asiento);
            nosi = fgetc(stdin);
            fgetc(stdin);

            if (nosi == 's' || nosi == 'S') {
                comprar = true;
            } else {
                comprar = false;
            }
        }

        /* Se envía la comprobación al hilo */
        chat[1] = comprar;

        up(sem_hilo, 0);
        down(sem_cliente, 0);

        /* Se obtiene la confirmación del hilo */
        comprado = chat[1];

        if (comprado == true && (nosi == 's' || nosi == 'S')) {
            printf("\nCompra completada con éxito!\n");
        } else if (comprado == false && nosi != 's' && nosi != 'S') {
            printf("\nSu compra ha sido cancelada.\n");
        }

        /* Se pregunta al usuario si requiere hacer otra compra */
        printf("\nDesea realizar otra compra? (s/n): ");
        nosi = fgetc(stdin);
        fgetc(stdin);

        if (nosi == 's' || nosi == 'S') {
            otra_compra = true;
        } else {
            otra_compra = false;
        }

        /* Se envía la respuesta del usuario al hilo */
        chat[1] = otra_compra;

        up(sem_hilo, 0);
        down(sem_cliente, 0);
    }

    /* Si el usuario ya no requiere hacer otra compra */
    system("clear");

    printf("Vuelva pronto!\n");

    sleep(3);
    system("clear");

    return 0;
}
