#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "pila.h"

Pila crear(int capacidad)
{
    Pila nueva_pila;

    nueva_pila.datos = (TIPODATO *)malloc(sizeof(TIPODATO)*capacidad);
    nueva_pila.cima = -1;
    nueva_pila.capacidad = capacidad;

    return nueva_pila;
}

bool push(Pila *pila, TIPODATO elemento)
{
    if (es_null(pila) || esta_llena(*pila))
        return false;

    pila->cima = pila->cima + 1;
    *(pila->datos + pila->cima) = elemento;

    return true;
}

TIPODATO pop(Pila *pila)
{
    if (es_null(pila) || esta_vacia(*pila))
        return false;

    int auxiliar; // Para guardar el dato que se encuentra en la cima de la pila

    auxiliar = *(pila->datos + pila->cima);
    *(pila->datos + pila->cima) = 0;
    pila->cima = pila->cima - 1;

    return auxiliar;
}

TIPODATO cima(Pila pila)
{
    if (esta_vacia(pila))
        return false;

    return *(pila.datos + pila.cima);
}

bool es_null(Pila *pila)
{
    if (pila != NULL)
        return false;

    printf("Tu pila es nula.\n");

    return true;
}

bool esta_vacia(Pila pila)
{
    if (pila.cima > -1)
        return false;

    printf("Tu pila esta vacia.\n");

    return true;
}

bool esta_llena(Pila pila)
{
    if (pila.cima < pila.capacidad-1)
        return false;

    printf("Tu pila esta llena.\n");

    return true;
}

void liberar(Pila *pila)
{
    if (es_null(pila))
        return;

    int i = 0;

    while (i < pila->capacidad) {
       *(pila->datos + i) = 0;
       i++;
    }

    free(pila->datos);
    pila->capacidad = 0;
    pila->cima = -1;
}
