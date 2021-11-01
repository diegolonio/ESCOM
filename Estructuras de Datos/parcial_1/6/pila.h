#ifndef _PILA_H_
#define _PILA_H_

#include <stdbool.h>

#define TIPODATO Delimitador
#define FORMATO "%c"

typedef struct Delimitador {
    char delimitador;
    int posicion;
} Delimitador;

typedef struct Nodo {
    TIPODATO dato;
    struct Nodo *siguiente;
} Nodo;

Nodo *push(Nodo *nodo, TIPODATO dato);
TIPODATO pop(Nodo **nodo);
TIPODATO cima(Nodo *nodo);
bool esta_vacia(Nodo *nodo);
void mostrar(Nodo *nodo);
void liberar(Nodo **nodo);

#include "pila.c"

#endif /* _PILA_H_ */
