#include <stdio.h>
#include "pila.h"
#include "evaluar_posfijas.h"

int main(void)
{
    Nodo *expresion = NULL;
    char cadena[TAM_MAX_NUM];

    printf("Ingresa tu expresion: ");
    fgets(cadena, sizeof(cadena), stdin);
    printf("\n");

    expresion = fragmentar(cadena);

    /**
     * Aún no resuelvo la situación en la que la expresión está mal y la función
     * evaluar() retorna 0, ya que el 0 podría ser el resultado de una expresión
     * correcta, en general, lo mismo sucede para cualquier número entero.
     * Por el momento, la única forma de saber si el 0 retornado por la función
     * es debido a un error es con el mensaje de error impreso por pantalla.
     */
    printf("El resultado es %ld\n", evaluar(expresion));

    return 0;
}