#ifndef _PILA_H_
#define _PILA_H_

#include <stdbool.h>

#define FORMATO "%c" // Para poder imprimir el tipo de dato correcto de acuerdo con TIPODATO

typedef char TIPODATO;

typedef struct Nodo
{
    TIPODATO dato;
    struct Nodo *siguiente;
} Nodo;

Nodo *push(Nodo *nodo, TIPODATO dato);
TIPODATO pop(Nodo **nodo);
TIPODATO cima(Nodo *nodo);
bool esta_vacia(Nodo *nodo);
void mostrar(Nodo *nodo);
void liberar(Nodo **pila);

#include "pila.c"

#endif /* _PILA_H_ */
