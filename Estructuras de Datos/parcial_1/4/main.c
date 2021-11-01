#include <stdio.h>
#include <stdlib.h>
#include "pila.h"

int main (void)
{
    int i;
    Pila mi_pila = crear(5);
    
    for (i = 6; push(&mi_pila, i); i--);

    for (i = 0; i <= mi_pila.cima; i++)
        printf("%d ", mi_pila.datos[i]);
    printf("\n");

    liberar(&mi_pila);

    return 0;
}
