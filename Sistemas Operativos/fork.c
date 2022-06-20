#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> // Para el manejo de procesos
#include <wait.h> // Para poder usar sleep();
#include <sys/types.h> // Para el manejo de procesos

int main()
{
    pid_t pid; // pid_t es la redefinición de un entero
    
    /* El programa le dice al sistema operativo que quiere crear un nuevo proceso
       hijo. Todas las líneas de código antes de la sentencia pid = fork(); son
       copiadas al proceso hijo.
    */
    pid = fork();
    
    if (pid == -1) { // Verificamos que no hubo problema al crear el proceso
        perror("\nError al crear el proceso.\n");
        exit(-1);
    }

    /* fork va a buscar en el programa un if donde preguntemos si pid == 0 o un
       switch con un case 0 para su implementación. Es importante considerar que
       solo el código que está dentro del if (pid == 0 es copiado en el proceso hijo.
    */
    if (pid == 0) {
        printf("\nHola soy el proceso hijo.\n");
        printf("\nMi identificador es: %d\n", getpid());
        printf("\nMI proceso padre es: %d\n", getppid());
        sleep(2);

        /* Con exit(0) terminamos el proceso hijo, si no lo hacemos, fork seguirá
           copiando en el proceso hijo el código que sigue después del bloque else.
        */
        exit(0);
    } else {
    /* El código dentro del else es ejecutado por el padre, es decir que esta sentencia
        if no tiene el comportamiento que comúnmente tiene, sino que sirve para
        indicar el código que es exclusivo del proceso hijo y el código que es
        exlusivo del proceso padre, de modo que ambos bloques de código se ejecutan
        pero en diferentes procesos.
    */
   
        /* Con wait(NULL) le indicamos al proceso padre que se espere a que
           termine de ejecutarse el proceso hijo para comenzar a ejecutar este
           bloque de código. De otro modo, ambos procesos se ejecutan al mismo tiempo.
        */
        wait(NULL);
        printf("\n\t\t\tHola soy el proceso padre.\n");
        printf("\n\t\t\tMi identificador es: %d\n", getpid());
        printf("\n\t\t\tMi proceso padre es: %d\n", getppid());
    }

    printf("\n\t\t\tHola soy el proceso padre fuera del if.\n");
    printf("\n\t\t\tMi identificador es: %d\n", getpid());
    printf("\n\t\t\tMI proceso padre es: %d\n", getppid());

    return 0;
}
