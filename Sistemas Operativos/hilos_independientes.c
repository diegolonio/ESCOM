#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#define MAX_THREADS 10

void hilo();

int main()
{
    int j;

    pthread_attr_t atributos;
    pthread_t identificadores[MAX_THREADS];
    pthread_attr_init(&atributos);
    pthread_attr_setdetachstate(&atributos, PTHREAD_CREATE_DETACHED);
    
    for (j = 0; j < MAX_THREADS; j++) {
        pthread_create(&identificadores[j], &atributos, (void *)hilo, NULL);
    }

    sleep(5);

    return 0;
}

void hilo()
{
    printf("\nHilo: %ld\n", pthread_self());
    pthread_exit(0);
}
