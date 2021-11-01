#ifndef _PILA_H_
#define _PILA_H_

#include <stdbool.h>

typedef int TIPODATO;

typedef struct Pila {
    TIPODATO *datos;
    int cima;
    int capacidad;
} Pila;

Pila crear(int capacidad);
bool push(Pila *pila, TIPODATO elemento);
TIPODATO pop(Pila *pila);
TIPODATO cima(Pila pila);
bool es_null(Pila *pila);
bool esta_vacia(Pila pila);
bool esta_llena(Pila pila);
void liberar(Pila *pila);

#include "pila.c"

#endif /* _PILA_H_ */
