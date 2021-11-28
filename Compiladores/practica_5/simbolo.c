/* 
 * Nombre: Apolonio Villegas Diego Armando 
 * Práctica: 5
 * Tema: Decisiones y ciclos
 * Opción: Calculadora de vectores
 * Fecha 18/11/2021
 * Grupo: 3CM17
 * Materia: Compiladores
 */

#include <stdlib.h>
#include <string.h>
#include "vectores.h"
#include "vectores.tab.h"

static Simbolo *tabla_de_simbolos = NULL; /* Lista simplemente enlazada de la tabla de símbolos */

Simbolo *buscar(char *nombre)
{
    Simbolo *caminante;

    for (caminante = tabla_de_simbolos; caminante != (Simbolo *)0; caminante = caminante->siguiente)
        if (strcmp(caminante->nombre, nombre) == 0)
            return caminante;

    return 0; /* Símbolo no encontrado */
}

Simbolo *instalar(char *nombre, int tipo, Vector *vector, int escalar)
{
    Simbolo *nuevo_simbolo;
    
    nuevo_simbolo = (Simbolo *)emalloc(sizeof(Simbolo));
    nuevo_simbolo->nombre = emalloc(strlen(nombre)+1);
    strcpy(nuevo_simbolo->nombre, nombre);
    nuevo_simbolo->tipo = tipo;
    nuevo_simbolo->u.vector = vector;
    nuevo_simbolo->u.escalar = escalar;
    nuevo_simbolo->siguiente = tabla_de_simbolos;
    tabla_de_simbolos = nuevo_simbolo;

    return nuevo_simbolo;
}

char *emalloc(unsigned tamanio)
{
    char *memoria;

    memoria = malloc(tamanio);

    if (memoria == 0)
        ejecutar_error("sin memoria", (char *)0);

    return memoria;
}
