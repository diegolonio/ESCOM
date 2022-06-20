#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

void *hilo(void *);

int main()
{
    pthread_t id_hilo;
    int argumentos[2] = {2, 3};

    printf("\nCreación del hilo...\n");
    
    /* Los argumentos que se pasen al hilo no deben tener ningun tipo de dato definido */
    pthread_create(&id_hilo, NULL, hilo, (void *)argumentos);

    printf("\nHilo creado. Esperando su finalización...\n");
    pthread_join(id_hilo, NULL);

    printf("\nHilo finalizado...\nValor 1: %i\nValor 2: %i\n", *argumentos, *(argumentos+1));

    return 0;
}

void *hilo(void *argumentos)
{
    int i;
    int *parametros = (int *)argumentos;
    
    printf("\nEstamos en el hilo...\n");
    printf("\nValor 1: %i\nValor 2: %i\n", *parametros, *(parametros+1));

    *parametros = 7;
    *(parametros+1) = 8;

    for (i = 0; i < 4; i++) {
        sleep(1);
    }

    pthread_exit(NULL);
}
