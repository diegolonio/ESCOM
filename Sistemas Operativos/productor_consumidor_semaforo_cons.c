#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>

#define PERMISOS 0644
#define N 5

int crea_semaforo(key_t, int);
void down(int);
void up(int);
void consume(int);
int entra_al_bufer();

int main()
{
    int dato_consumido;
    int semaforo_mutex, semaforo_vacio, semaforo_lleno;
    key_t llave1, llave2, llave3;

    llave1 = ftok("Prueba1", 'k');
    semaforo_mutex = crea_semaforo(llave1, 1);

    llave2 = ftok("Prueba2", 'l');
    semaforo_vacio = crea_semaforo(llave2, N);

    llave3 = ftok("Prueba3", 'm');
    semaforo_lleno = crea_semaforo(llave3, 0);

    while (1) {
        down(semaforo_lleno);
        down(semaforo_mutex);

        dato_consumido = entra_al_bufer();
        
        up(semaforo_mutex);
        up(semaforo_vacio);
        consume(dato_consumido);
    }

    return 0;
}

int crea_semaforo(key_t llave, int valor_inicial)
{
    int semid = semget(llave, 1, IPC_CREAT|PERMISOS);

    if (semid == -1) {
        return -1;
    }

    semctl(semid, 0, SETVAL, valor_inicial);

    return semid;
}

void down(int semid)
{
    struct sembuf op_p[] = {0, -1, 0};
    semop(semid, op_p, 1);
}

void up(int semid)
{
    struct sembuf op_v[] = {0, +1, 0};
    semop(semid, op_v, 1);
}

void consume(int dato_consumido)
{
    int i;

    for (i = 0; i < 2; i++) {
        printf("\nConsumidor con pid=%d, consumiendo=%d\n", getpid(), i);
        sleep(1);
    }

    printf("\nEl dato consumido es: %d\n", dato_consumido);
}

int entra_al_bufer()
{
    int i, memoria1;
    int *dato;
    key_t llave4;

    llave4 = ftok("Prueba4", 'n');
    memoria1 = shmget(llave4, sizeof(int), IPC_CREAT|0600);
    dato = shmat(memoria1, 0, 0);

    for (i = 0; i < 3; i++) {
        printf("\nProceso con pid=%d, en el bufer=%d\n", getpid(), i);
        sleep(1);
    }

    return *dato;
}
