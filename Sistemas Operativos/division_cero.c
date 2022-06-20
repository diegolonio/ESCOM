#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <wait.h>
#include <sys/types.h>

int main()
{
    int i = 1;

    printf("\nHola, soy un proceso muy chido.\n");
    printf("\nMi identificador es: %d\n", getpid());
    printf("\nMi proceso padre es: %d\n", getppid());
    printf("\nAhora voy a cometer un error.\n");
    sleep(5);
    printf("\nEste valor es un error: %d\n", 67/--i);
    printf("\nHola, sigo siendo un proceso muy chido.\n");

    return 0;
}
