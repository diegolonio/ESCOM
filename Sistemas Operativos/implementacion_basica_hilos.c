#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

void *hilo(void *);

int main()
{    
    int i;
    pthread_t id_hilo;

    pthread_create(&id_hilo, NULL, hilo, NULL);
    printf("\nHilo creado.\n");
    
    for (i = 0; i < 4; i++) {
        printf ("\nProceso padre...\n");
        sleep (1);
    }
    
    printf("\nEsperando su finalización.\n");
    
    pthread_join(id_hilo, NULL);
    printf("\nHilo finalizado...\n");

    return 0;
}

void *hilo(void *)
{
    int i;

    for (i = 0; i < 4; i++) {
        printf("\nDentro del hilo...\n");
        sleep(2);
    }

    pthread_exit(NULL);
}
