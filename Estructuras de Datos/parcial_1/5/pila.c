#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pila.h"

Nodo *push(Nodo *nodo, TIPODATO dato)
{
    Nodo *nuevo_nodo = NULL;

    nuevo_nodo = (Nodo *)malloc(sizeof(Nodo));
    nuevo_nodo->dato = dato;
    nuevo_nodo->siguiente = nodo;

    return nuevo_nodo;
}

TIPODATO pop(Nodo **nodo)
{
    if (esta_vacia(*nodo))
        return false;

    Nodo *auxiliar = *nodo;
    TIPODATO dato = auxiliar->dato;

    *nodo = (*nodo)->siguiente;
    free(auxiliar);

    return dato;
}

TIPODATO cima(Nodo *nodo)
{
    if (!esta_vacia(nodo))
        return nodo->dato;

    return 0;
}

bool esta_vacia(Nodo *nodo)
{
    if (nodo != NULL)
        return false;

    printf("Tu pila esta vacia.\n");

    return true;
}

void mostrar(Nodo *nodo)
{
    if (!esta_vacia(nodo)) {
        Nodo *auxiliar = nodo;

        while (auxiliar != NULL) {
            printf(FORMATO, auxiliar->dato);
            auxiliar = auxiliar->siguiente;
        }

        printf("\n");
    }
}

void liberar(Nodo **pila)
{
    if (esta_vacia(*pila))
        return;

    Nodo *caminante = *pila, *auxiliar = NULL;

    while (caminante != NULL) {
        auxiliar = caminante;
        caminante = caminante->siguiente;
        auxiliar->dato = 0;
        auxiliar->siguiente = NULL;
        free(auxiliar);
    }

    *pila = NULL;
}
