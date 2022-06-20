#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/shm.h>
#include <sys/types.h>

void regionCritica();
void regionNoCritica();

int main()
{
    int shmid;
    key_t llave;
    int *turno;

    llave = ftok("Prueba", 'k');
    shmid = shmget(llave, sizeof(int), IPC_CREAT|0600);
    turno = shmat(shmid, 0, 0);

    while (1) {
        while (*turno != 1)
            ;

        regionCritica();

        *turno = 0;

        regionNoCritica();
    }

    return 0;
}

void regionCritica()
{
    int i;

    printf("\nProceso en la región critica con pid=%d\n", getpid());

    for (i = 0; i < 4; i++) {
        printf("\nRegion critica: %d\n", i);
        sleep(1);
    }
}

void regionNoCritica()
{
    int i;

    printf ("\nEn la region NO critica con pid %d\n", getpid());

    for (i = 0; i < 6; i++) {
        printf("\nContando (%d): %d\n", getpid(), i);
        sleep(1);
    }
}
