#include "pila.h"

int main(void)
{
    Nodo *top = NULL;

    top = push(top, 'o');
    top = push(top, 'd');
    top = push(top, 'n');
    top = push(top, 'u');
    top = push(top, 'M');
    top = push(top, ' ');
    top = push(top, 'a');
    top = push(top, 'l');
    top = push(top, 'o');
    top = push(top, 'H');
    
    mostrar(top);

    liberar(&top);

    return 0;
}
