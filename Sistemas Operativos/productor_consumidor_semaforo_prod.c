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
int produce();
void entra_al_bufer(int);

int main()
{
    int dato_producido;
    int semaforo_mutex, semaforo_vacio, semaforo_lleno;
    key_t llave1, llave2, llave3;

    llave1 = ftok("Prueba1", 'k');
    semaforo_mutex = crea_semaforo(llave1, 1);

    llave2 = ftok("Prueba2", 'l');
    semaforo_vacio = crea_semaforo(llave2, N);
    
    llave3 = ftok("Prueba3", 'm');
    semaforo_lleno = crea_semaforo(llave3, 0);

    while (1) {
        dato_producido = produce();

        down(semaforo_vacio);
        down(semaforo_mutex);

        entra_al_bufer(dato_producido);

        up(semaforo_mutex);
        up(semaforo_lleno);
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

int produce()
{
    int i;

    for (i = 0; i < 2; i++) {
        printf("\nProductor con pid=%d, produciendo=%d\n", getpid(), i);
        sleep(1);
    }

    printf("\nEl dato generado es: %d\n", i);

    return i;
}

void entra_al_bufer(int dato_producido)
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

    *dato = dato_producido;
    printf("\nDato guardado en el bufer: %d\n", *dato);
}
