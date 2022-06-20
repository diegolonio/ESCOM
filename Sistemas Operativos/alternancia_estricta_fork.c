#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/types.h>

void regionCritica(int);
void regionNoCriticaProcesoHijo(int);
void regionNoCriticaProcesoPadre(int);

int main()
{
    pid_t pid;
    int PID;

    /* Creación de memoria compartida */

    int shmid; // Identificador de la memoria compartida
    int *turno; // Variable de conexión entre el proceso (programa) y la memoria compartida
    key_t llave; // Llave para poder entrar a la memoria compartida
    
    /* Creación de la llave para acceder a la memoria compartida.
       El primer parámetro es el archivo en el que se va a guardar la llave durante la ejecución de los procesos.
       El segundo parámetro es la contraseña de la llave, debe ser un número entero.

       NOTA: El archivo donde se guarde la llave debe estar en el mismo directorio que el programa. Si el archivo
          no existe, el sistema operativo nos "presta" un archivo. Sin embargo, no nos garantiza que todos los
       procesos encuentren ese archivo.
    */
    llave = ftok("Prueba", 'k');

    /* Creamos la memoria compartida.
       shmget devuelve un folio de creación de la memoria compartida.
       - El primer parámetro es la llave que se le va a asignar a la memoria compartida.
       - El segundo parámetro es el tamaño de la memoria compartida.
       - El tercer parámetro indica los permisos. En este caso, de lectura y escritura.
    */
    shmid = shmget(llave, sizeof(int), IPC_CREAT|0600);

    /* Hacemos la conexión entre la memoria compartida y el proceso.
       - El primer parámetro es el folio generado cuando se creó la memoria compartida.
       - El segundo parámetro es el incio de la memoria compartida.
       - El tercer parámetro es el final de la memoria compartida.

       shmat retorna un apuntador a la memoria compartida.

       NOTA: pasar 0 como segundo y tercer argumentos de shmat indica que se va conectar toda la memoria
       compartida con el proceso.
    */
    turno = shmat(shmid, 0, 0);

    *turno = 0; // Inicializamos la variable de conexión

    /* Fin de la creación de la memoria compartida */

    pid = fork();

    if (pid == -1) {
        printf("\nError al crear el proceso\n");
        exit(-1);
    }

    if (pid == 0) {
        while (1) {
            /* Mientras que turno es distinto de 0, es decir, algún otro proceso está
               en la región crítica, el proceso hijo no hace nada y se queda esperando activamente.
            */
            while (*turno != 0)
                ;

            /* Cuando turno cambia a 0, es decir, es el turno del proceso hijo,
               entonces este entra en la región crítica. Obtenemos el pid del proceso hijo.
            */
            PID = getpid();
            regionCritica(PID);

            /* El proceso hijo actualiza la variable para que el proceso con el turno 1
               entre en la región crítica.
            */
            *turno = 1;

            /* El proceso hijo hace otras cosas que debe hacer fuera de la región crítica.
            */
            regionNoCriticaProcesoHijo(PID);
        }
    } else {
        while (1) {
            /* La misma situación que el proceso hijo es planteada para el proceso padre, solo que
               a este le corresponde el turno 1.
            */
            while (*turno != 1)
                ;

            PID = getpid();

            regionCritica(PID);

            *turno = 0;

            regionNoCriticaProcesoPadre(PID);
        }
    }

    return 0;
}

void regionCritica(int PID)
{
    int i;

    printf("\nProceso en la region critica con pid=%d\n", PID);

    for (i = 0; i < 3; i++) {
        printf("\nRegion critica: %d\n", i);
        sleep(1);
    }
}

void regionNoCriticaProcesoHijo(int PID)
{
    int i;

    printf("\nHijo en la region NO critica con pid=%d\n", PID);
    
    for (i = 0; i < 25; i++) {
        printf("\nHijo: %d\n", i);
        sleep(1);
    }
}

void regionNoCriticaProcesoPadre(int PID)
{
    int i;

    printf("\nPadre en la region NO critica con pid=%d\n", PID);
    
    for (i = 0; i < 5; i++) {
        printf("\nPadre: %d\n", i);
        sleep(1);
    }
}
