/*
 * Nombre: Apolonio Villegas Diego Armando 
 * Práctica: 1
 * Tema: yacc básico
 * Opción: Calculadora de vectores
 * Fecha 06/10/2021
 * Grupo: 3CM17
 * Materia: Compiladores
 */

#ifndef __VECTORES_H__
#define __VECTORES_H__

#include <stdlib.h>

/* Implementación de vectores */

typedef struct Componente {
	int valor;
	struct Componente *siguiente;
} Componente;

typedef struct Vector {
	Componente *componente_inicial;
	Componente *caminante;
} Vector;

Componente *crear_componente(int, Componente *);
Vector *crear_vector(Componente *);
void insertar_componente(Vector *, int);
void mostrar_vector(Vector *);
void liberar_vector(Vector *);

/* Operaciones con vectores */

Vector *suma(Vector *, Vector *);
Vector *resta(Vector *, Vector *);
int punto(Vector *, Vector *);
Vector *cruz(Vector *, Vector *);
Vector *ppescalar(int, Vector *);
int norma(Vector *);
int dimension(Vector *);

#endif /* _VECTORES_H_ */
