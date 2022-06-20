#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "../include/unidad2.h"

void atender_cliente(void *);

int main()
{
    int contador;
    int sem_servidor, sem_cliente, sem_claves;
    int llaves_conexion[20][2], llave, archivo;
    int *parametros_cliente = NULL, *asientos = NULL;
    char numero_asiento[3], archivo_sem_asiento[15];
    char numero_archivo[4], archivo_shm[8], archivo_sem_cliente[16], archivo_sem_hilo[13];

    /* ------------------------- Memorias compartidas ------------------------- */

    /* Arreglo de asientos */
    asientos = (int *)shm(sizeof(int)*20, "shm_asientos", 'u');

    /** 
     * Memoria compartida para asignar llaves de comunicación al cliente
     * nuevo para que pueda comunicarse con el hilo que le es asignado
     */
    parametros_cliente = (int *)shm(sizeof(int)*2, "shm_claves", 'v');

    /* ------------------------------ Semáforos ------------------------------- */

    /* Semáforo del servidor */
    sem_servidor = semnew(1, "sem_servidor", 'w');

    /* Semáforo del cliente */
    sem_cliente = semnew(0, "sem_cliente", 'x');

    /* Semáforo de claves */
    sem_claves = semnew(0, "sem_claves", 'y');

    /* Semáforos de asientos */
    for (contador = 0; contador < 20; contador++) {
        sprintf(numero_asiento, "%d", contador+1);
        strcpy(archivo_sem_asiento, "sem_asiento_");
        strcat(archivo_sem_asiento, numero_asiento);

        asientos[contador] = semnew(1, archivo_sem_asiento, contador+1);
    }

    /* --------------------------- Configuraciones ---------------------------- */

    /* Llenado del arreglo con las llaves de conexión para la memoria compartida con hilos */
    for (contador = 0; contador < 20; contador++) {
        llaves_conexion[contador][0] = contador + 97;
        llaves_conexion[contador][1] = true;
    }

    /* Hilos para atender a los clientes */
    pthread_attr_t atributos;
	pthread_t hilos[20];
	pthread_attr_init(&atributos);
	pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_DETACHED);

    /* Reinicio del contador de número de archivo */
    contador = 0;

    /* ------------------------ Asignación de claves -------------------------- */

    system("clear");

    while (1) {
        /* Obtención de las claves */
        llave = llaves_conexion[contador][0];
        archivo = contador+300;

        /* La llave asignada ya no está disponible */
        llaves_conexion[contador][1] = false;

        /* Conversión del número de archivo asignado a cadena */
        sprintf(numero_archivo, "%d", archivo);

        /* Memoria compartida del hilo asignado */
        strcpy(archivo_shm, "shm_");
        strcat(archivo_shm, numero_archivo);
        shm(sizeof(int)*2, archivo_shm, llave);

        /* Semáforo del hilo */
        strcpy(archivo_sem_hilo, "sem_hilo_");
        strcat(archivo_sem_hilo, numero_archivo);
        semnew(0, archivo_sem_hilo, llave+400);

        /* Semáforo del cliente */
        strcpy(archivo_sem_cliente, "sem_cliente_");
        strcat(archivo_sem_cliente, numero_archivo);
        semnew(0, archivo_sem_cliente, llave+500);

        down(sem_claves, 0);

        /* Envío de las claves al cliente */
        parametros_cliente[0] = llave;
        parametros_cliente[1] = archivo;

        up(sem_cliente, 0);
        down(sem_claves, 0);

        /* Creación del hilo asignado */
        pthread_create(&hilos[contador], &atributos, (void *)atender_cliente, (void *)parametros_cliente);

        up(sem_cliente, 0);

        contador++;
    }

    return 0;
}

void atender_cliente(void *argumentos)
{
    pthread_t id = pthread_self();
    int contador;
    int *claves_comunicacion_cliente = (int *)argumentos;
    int llave, archivo;
    int *chat, *asientos;
    int sem_hilo, sem_cliente;
    char numero_archivo[4], archivo_shm[8], archivo_sem_cliente[16], archivo_sem_hilo[13];
    char numero_asiento[3], archivo_sem_asiento[15];
    bool comprado, comprar, apartado, apartar, otra_compra = true;
    int asiento;

    /* Obtención de las claves del servidor */
    llave = claves_comunicacion_cliente[0];
    archivo = claves_comunicacion_cliente[1];

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

    /* ------------------------- Memorias compartidas ------------------------- */

    /* Arreglo se asientos */
    asientos = (int *)shm(sizeof(int)*20, "shm_asientos", 'u');

    /* Chat */
    chat = (int *)shm(sizeof(int)*2, archivo_shm, llave);

    /* ------------------------------ Semáforos ------------------------------- */

    /* Semáforos de asientos */
    for (contador = 0; contador < 20; contador++) {
        sprintf(numero_asiento, "%d", contador+1);
        strcpy(archivo_sem_asiento, "sem_asiento_");
        strcat(archivo_sem_asiento, numero_asiento);

        asientos[contador] = sem(archivo_sem_asiento, contador+1);
    }

    /* Semáforo del hilo */
    sem_hilo = semnew(0, archivo_sem_hilo, llave+400);

    /* Semáforo del cliente */
    sem_cliente = semnew(0, archivo_sem_cliente, llave+500);

    /* ------------------------ Inicio de comunicación ------------------------ */

    printf("[Hilo %ld] Nueva conexión.\n", id);

    while (otra_compra == true) {
        down(sem_hilo, 0);
        
        /* Se obtiene el número de asiento del usuario */
        asiento = chat[0];

        /* Se determina el estado del asiento elegido */
        if (down(asientos[asiento], IPC_NOWAIT) == false) {
            printf("[Hilo %ld] El asiento %d ya está apartado.\n", id, asiento+1);
            apartado = true;
        } else {
            printf("[Hilo %ld] Asiento %d apartado.\n", id, asiento+1);
            apartado = false;
        }

        /* Se envía el estado del asiento al usuario */
        chat[1] = apartado;

        up(sem_cliente, 0);
        down(sem_hilo, 0);

        /* Se obtiene la decisión de compra del usuario */
        comprar = chat[1];

        if (comprar == true) {
            printf("[Hilo %ld] El asiento %d ha sido comprado.\n", id, asiento+1);
            comprado = true;
        } else { // Si el usuario decide no comprar el asiento
            printf("[Hilo %ld] Compra del asiento %d cancelada.\n", id, asiento+1);
            
            /* Se libera el asiento */
            if (apartado == false) {
                up(asientos[asiento], 0);
            }

            comprado = false;
        }

        /* Se envía la confirmación de compra al usuario */
        chat[1] = comprado;

        up(sem_cliente, 0);
        down(sem_hilo, 0);

        /* Se obtiene la decisión de compra nueva del usuario */
        otra_compra = chat[1];

        up(sem_cliente, 0);
    }

    /* Si el usuario no requiere realizar otra compra */
    printf("[Hilo %ld] Conexión terminada.\n", id);

    pthread_exit(NULL);
}
