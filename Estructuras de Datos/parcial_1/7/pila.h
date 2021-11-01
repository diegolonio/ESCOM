#ifndef _PILA_H_
#define _PILA_H_

#include <stdbool.h>

#define TIPODATO char
#define FORMATO "%s"
#define TAM_MAX_NUM 20

typedef struct Nodo {
    TIPODATO dato[TAM_MAX_NUM];
    struct Nodo *siguiente;
} Nodo;

Nodo *push(Nodo *nodo, TIPODATO dato[]);
TIPODATO *pop(Nodo **nodo);
TIPODATO *cima(Nodo *nodo);
bool esta_vacia(Nodo *nodo);
void mostrar(Nodo *nodo);

#include "pila.c"

#endif /* _PILA_H_ */