#ifndef _EVALUAR_POSFIJAS_H_
#define _EVALUAR_POSFIJAS_H_

#define TAM_MAX_LINEA 1000

#include "pila.h"

Nodo *fragmentar(char cadena[]);
int string_a_int(char *numero);
long int evaluar(Nodo *expresion);

#include "evaluar_posfijas.c"

#endif