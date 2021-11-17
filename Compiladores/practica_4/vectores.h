#ifndef __VECTORES_H__
#define __VECTORES_H__

#include <stdlib.h>
#include <stdbool.h>

/* --------------------------- Implementación de vectores ---------------------------- */

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

/* ---------------------------- Operaciones con vectores ----------------------------- */

Vector *suma(Vector *, Vector *);
Vector *resta(Vector *, Vector *);
int punto(Vector *, Vector *);
Vector *cruz(Vector *, Vector *);
Vector *ppescalar(int, Vector *);
int norma(Vector *);

/* ------------------------------ Funciones del parser ------------------------------- */

void yyerror(char *s);
int yylex();
void advertencia(char *s, char *t);
void ejecutar_error(char *, char *);
void inicializar();

/* -------------------------------- Tabla de símbolos -------------------------------- */

typedef struct Simbolo {
	char *nombre;
	short tipo;
	union {
		int escalar;
		Vector *vector;
		int (*apuntador)(Vector *);
	} u;
	struct Simbolo *siguiente;
} Simbolo;

Simbolo *instalar(char *, int, Vector *, int);
Simbolo *buscar(char *);
char *emalloc(unsigned);

/* --------------------------------- Máquina de pila --------------------------------- */

typedef union Datum { /* Tipo de dato de la pila del interprete */
	Componente *Componente; /* Componente */
	Simbolo *simbolo; /* Variable, Vector, Escalar, Función predefinida */
} Datum;

typedef void (*Instruccion)(); /* Tipo de dato: instrucción de máquina */
#define STOP (Instruccion)0

extern Instruccion programa[];

/* ---------------------------- Funciones sobre la máquina ---------------------------- */

extern void iniciar_codigo();
extern Instruccion *codigo(Instruccion);
extern void ejecutar_instruccion(Instruccion *);

/* ---------------------- Funciones sobre la pila del intérprete ---------------------- */

extern Datum pop();
extern void push(Datum);

/* --------------------------- Instrucciones de la máquina --------------------------- */

extern void imprimir_vector();
extern void imprimir_escalar();
extern void insertar_variable();
extern void insertar_escalar();
extern void insertar_predefinida();
extern void asignar();
extern void evaluar();
extern void vector_negativo();
extern void escalar_negativo();
extern void maquina_suma();
extern void maquina_resta();
extern void maquina_punto();
extern void maquina_cruz();
extern void maquina_ppescalar();
extern void maquina_norma();
extern void maquina_crear_vector();
extern void maquina_crear_componente();
extern void maquina_crear_primer_componente();

#endif /* _VECTORES_H_ */
