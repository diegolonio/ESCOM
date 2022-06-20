#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>

void regionCritica(int);
void regionNoCriticaProcesoHijo(int);
void regionNoCriticaProcesoPadre(int);

int main()
{
    pid_t pid;
    int PID;

    int shmid1;
    int shmid2;
    int shmid3;
    key_t llave1;
    key_t llave2;
    key_t llave3;
    int *hijo_desea_entrar;
    int *padre_desea_entrar;
    int *proceso_favorecido;

    llave1 = ftok("Prueba", 'k');
    llave2 = ftok("Prueba", 'l');
    llave3 = ftok("Prueba", 'm');
    shmid1 = shmget(llave1, sizeof(int), IPC_CREAT|0600);
    shmid2 = shmget(llave2, sizeof(int), IPC_CREAT|0600);
    shmid3 = shmget(llave3, sizeof(int), IPC_CREAT|0600);
    hijo_desea_entrar = shmat(shmid1, 0, 0);
    padre_desea_entrar = shmat(shmid2, 0, 0);
    proceso_favorecido = shmat(shmid3, 0, 0);
    *hijo_desea_entrar = 1;
    *padre_desea_entrar = 0;
    *proceso_favorecido = 1;

    pid = fork();

    if (pid == -1) {
        printf("\nError al crear el proceso.\n");
        exit(-1);
    }
    
    if (pid == 0) {
        while (1) {
            *hijo_desea_entrar = 1;

            while (*padre_desea_entrar) {
                if (*proceso_favorecido == 2) {
                    *hijo_desea_entrar = 0;

                    while (*proceso_favorecido == 2)
                        ;
                    
                    *hijo_desea_entrar = 1;
                }
            }

            PID = getpid();

            regionCritica(PID);

            *proceso_favorecido = 2;
            *hijo_desea_entrar = 0;

            regionNoCriticaProcesoHijo(PID);
        }
    } else {
        while (1) {
            *padre_desea_entrar = 1;

            while (*hijo_desea_entrar) {
                if (*proceso_favorecido == 1) {
                    *padre_desea_entrar = 0;

                    while (*proceso_favorecido == 1)
                        ;
                    
                    *padre_desea_entrar = 1;
                }
            }

            PID = getpid ();

            regionCritica (PID);

            *proceso_favorecido = 1;
            *padre_desea_entrar = 0;
            
            regionNoCriticaProcesoPadre(PID);
        }
    }

    return 0;
}

void regionCritica(int PID)
{
    int i;

    printf("\nProceso con pid=%d en la region critica.\n", PID);

    for (i = 0; i < 3; i++) {
        printf("\nRegion critica: %d\n", i);
        sleep(1);
    }
}

void regionNoCriticaProcesoHijo(int PID)
{
    int i;

    printf("\nHijo con pid=%d en la region NO critica.\n", PID);

    for (i = 0; i < 20; i++) {
        printf("\nHijo: %d\n", i);
        sleep(1);
    }
}

void regionNoCriticaProcesoPadre(int PID)
{
    int i;

    printf("\nPadre con pid=%d en la region NO critica.\n", PID);
    
    for (i = 0; i < 3; i++) {
        printf("\nPadre: %d\n", i);
        sleep(1);
    }
}
