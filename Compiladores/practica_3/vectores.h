/* 
 * Nombre: Apolonio Villegas Diego Armando 
 * Práctica: 3
 * Tema: Tabla de símbolos
 * Opción: Calculadora de vectores
 * Fecha 15/11/2021
 * Grupo: 3CM17
 * Materia: Compiladores
 */

#ifndef __VECTORES_H__
#define __VECTORES_H__

#include <stdlib.h>
#include <stdbool.h>

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
int dimension(Vector *);
bool son_iguales(Vector *, Vector *);
Vector *duplicar(Vector *);

/* Operaciones con vectores */

Vector *suma(Vector *, Vector *);
Vector *resta(Vector *, Vector *);
int punto(Vector *, Vector *);
Vector *cruz(Vector *, Vector *);
Vector *ppescalar(int, Vector *);
int norma(Vector *);

/* Funciones del parser */

void yyerror(char *s);
int yylex();
void advertencia(char *s, char *t);
void ejecutar_error(char *, char *);
void inicializar();

/* Tabla de símbolos */

typedef struct Simbolo {
	char *nombre;
	short tipo;
	union {
		Vector *vector;
		int (*apuntador)(Vector *);
	} u;
	struct Simbolo *siguiente;
} Simbolo;

Simbolo *instalar(char *, int, Vector *);
Simbolo *buscar(char *);
char *emalloc(unsigned);

#endif /* _VECTORES_H_ */
