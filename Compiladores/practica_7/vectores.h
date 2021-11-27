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

/* -------------------------------- Tabla de símbolos -------------------------------- */

typedef struct Simbolo {
	char *nombre;
	short tipo;
	union {
		int escalar;
		Vector *vector;
		int (*apuntador)(Vector *);
		void (**definicion)();
		char *cadena;
	} u;
	struct Simbolo *siguiente;
} Simbolo;

Simbolo *instalar(char *, int, Vector *, int);
Simbolo *buscar(char *);
char *emalloc(unsigned);

/* ------------------------------ Funciones del parser ------------------------------- */

void yyerror(char *);
int yylex();
void advertencia(char *, char *);
void ejecutar_error(char *, char *);
void inicializar();
int siguiente(int, int, int);
int backslash(int);
void dentro_de_definicion(char *);
void definir(Simbolo *);

/* --------------------------------- Máquina de pila --------------------------------- */

typedef union Datum { /* Tipo de dato de la pila del interprete */
	int escalar;
	Vector *vector;
	Componente *componente; /* Componente */
	Simbolo *simbolo; /* Variable, Vector, Escalar, Función predefinida */
} Datum;

typedef void (*Instruccion)(); /* Tipo de dato: instrucción de máquina */
#define PARO (Instruccion)0

extern Instruccion programa[];
extern Instruccion *cima_programa; /* Siguiente espacio disponible para la generación de código */
extern Instruccion *cima_subprograma; /* Inicio del subprograma actual */

/* -------------------------- Pila de función/procedimiento --------------------------- */

typedef struct Marco { /* Marco de pila de función/procedimiento */
	Simbolo *simbolo; /* Entrada de la tabla de símbolos */
	Instruccion *instruccion_despues_de_retornar; /* Dónde continuar ejecutando después de retornar */
	Datum *nesimo_argumento; /* n-ésimo en la pila */
	int numero_argumentos; /* Número de argumentos en la pila */
} Marco;

/* ---------------------------- Funciones sobre la máquina ---------------------------- */

void iniciar_codigo();
Instruccion *codigo(Instruccion);
void ejecutar(Instruccion *);

/* ---------------------- Funciones sobre la pila del intérprete ---------------------- */

Datum pop();
void push(Datum);

/* --------------------------- Instrucciones de la máquina --------------------------- */

void imprimir_vector();
void imprimir_escalar();
void imprimir_cadena();
void insertar_variable();
void insertar_escalar();
void ejecutar_predefinida();
void asignar();
void evaluar();
void vector_negativo();
void escalar_negativo();
void maquina_suma();
void maquina_resta();
void maquina_punto();
void maquina_cruz();
void maquina_ppescalar_escexp();
void maquina_ppescalar_expesc();
void maquina_norma();
void maquina_crear_vector();
void maquina_crear_componente();
void maquina_crear_primer_componente();
void o_logico();
void y_logico();
void mayor_que();
void mayor_igual();
void menor_que();
void menor_igual();
void vectores_iguales();
void vectores_no_iguales();
void escalares_iguales();
void escalares_no_iguales();
void negacion();
void si();
void mientras();
void para();
void llamada();
void retornar();
void procedimiento_retornar();
void funcion_retornar();
Vector **obtener_argumento();
void argumento();
void asignacion_argumento();

#endif /* _VECTORES_H_ */
