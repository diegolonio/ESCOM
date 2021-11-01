#include <stdio.h>
#include "delimitadores.h"

int main(void)
{
    char expresion[TAM_MAX_LINEA];

    printf("Ingresa tu expresion: ");
    fgets(expresion, sizeof(expresion), stdin);
    printf("\n");

    verificar_delimitadores(expresion);

    return 0;
}
