#include <stdio.h>
#include <stdlib.h>
#include "pila.h"

Nodo *push(Nodo *nodo, TIPODATO dato)
{
    Nodo *nuevo_nodo;

    nuevo_nodo = (Nodo *)malloc(sizeof(Nodo));
    nuevo_nodo->dato = dato;
    nuevo_nodo->siguiente = nodo;

    return nuevo_nodo;
}

TIPODATO pop(Nodo **nodo)
{
    if (esta_vacia(*nodo)) {
        Delimitador nil;

        nil.delimitador = '\0';
        nil.posicion = -1;

        return nil;
    }

    Nodo *auxiliar = *nodo;

    TIPODATO dato = auxiliar->dato;
    *nodo = (*nodo)->siguiente;
    free (auxiliar);

    return dato;
}

TIPODATO cima(Nodo *nodo)
{
    if (!esta_vacia(nodo))
        return nodo->dato;

    Delimitador nil;

    nil.delimitador = '\0';
    nil.posicion = -1;

    return nil;
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
            printf(FORMATO" ", auxiliar->dato.delimitador);
            auxiliar = auxiliar->siguiente;
        }

        printf("\n");
    }
}

void liberar(Nodo **nodo)
{
    if (esta_vacia(*nodo))
        return;

    Nodo *caminante = *nodo, *auxiliar = NULL;

    while (caminante != NULL) {
        auxiliar = caminante;
        caminante = caminante->siguiente;
        auxiliar->dato.delimitador = '\0';
        auxiliar->dato.posicion = -1;
        auxiliar->siguiente = NULL;
        free(auxiliar);
    }

    *nodo = NULL;
}
