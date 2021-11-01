#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pila.h"

Nodo *push(Nodo *nodo, TIPODATO dato[])
{
    Nodo *nuevo_nodo;

    nuevo_nodo = (Nodo *)malloc(sizeof(Nodo));
    strcpy(nuevo_nodo->dato, dato);
    nuevo_nodo->siguiente = nodo;

    return nuevo_nodo;
}

TIPODATO *pop(Nodo **nodo)
{
    if (esta_vacia(*nodo)) {
        printf("Tu pila esta vacia.\n");

        return 0;
    }

    Nodo *auxiliar = *nodo;

    TIPODATO dato[TAM_MAX_NUM];
    TIPODATO *elemento;

    strcpy(dato, auxiliar->dato);
    elemento = dato;
    *nodo = (*nodo)->siguiente;
    free(auxiliar);

    return elemento;
}

TIPODATO *cima(Nodo *nodo)
{
    if (!esta_vacia(nodo)) {
        TIPODATO *dato = nodo->dato;

        return dato;
    }

    return 0;
}

bool esta_vacia(Nodo *nodo)
{
    if (nodo != NULL)
        return false;

    return true;
}

void mostrar(Nodo *nodo)
{
    if (!esta_vacia(nodo)) {
        Nodo *auxiliar = nodo;

        while (auxiliar != NULL) {
            printf(FORMATO" ", auxiliar->dato);
            auxiliar = auxiliar->siguiente;
        }

        printf("\n");
    }
}
