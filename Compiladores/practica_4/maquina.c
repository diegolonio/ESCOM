#include <stdio.h>
#include "vectores.h"
#include "vectores.tab.h"

/* Configuración de la pila del intérprete */
#define TAMPILA 256
static Datum pila[TAMPILA]; /* Pila del interprete */
static Datum *cima_pila; /* Siguiente espacio disponible en la pila del interprete */

/* Configuración de la máquina */
#define TAMPROGRAMA 2000
Instruccion programa[TAMPROGRAMA]; /* La máquina */
Instruccion *cima_programa; /* Siguiente espacio disponible para la generación de código */
Instruccion *contador_programa; /* Contador de programa durante la ejecución de una secuencia de instrucciones */

bool tamano_diferente = false; /* ¿Los vectores tienen tamaños diferentes? */

/* ---------------------------- Funciones sobre la máquina ---------------------------- */

void iniciar_codigo() /* Iniciar la generación de código */
{
    cima_pila = pila;
    cima_programa = programa;
}

Instruccion *codigo(Instruccion nueva_instruccion) /* Insertar una nueva instrucción en la máquina */
{
    Instruccion *ocima_programa = cima_programa;

    if (cima_programa >= &programa[TAMPROGRAMA])
        ejecutar_error("programa demasiado grande", (char *)0);

    *cima_programa++ = nueva_instruccion;

    return ocima_programa;
}

void ejecutar_instruccion(Instruccion *instruccion) /* Ejecutar una instrucción de la máquina */
{
    for (contador_programa = instruccion; *contador_programa != STOP; )
            (*(*contador_programa++))();
}

/* ---------------------- Funciones sobre la pila del intérprete ---------------------- */

Datum pop() /* Sacar el elemento de la cima de la pila del interprete */
{
    if (cima_pila <= pila)
        ejecutar_error("tamaño máximo de pila excedido", (char *)0);

    return *--cima_pila;
}

void push(Datum nuevo_elemento) /* Insertar un elemento en la cima de la pila del intérprete */
{
    if (cima_pila >= &pila[TAMPILA])
        ejecutar_error("tamaño mínimo de pila excedido", (char *)0);

    *cima_pila++ = nuevo_elemento;
}

/* --------------------------- Instrucciones de la máquina --------------------------- */

void imprimir_vector()
{
    printf("Imprimir vector\n");
}

void imprimir_escalar()
{
    printf("Imprimir escalar\n");
}

void insertar_variable()
{
    printf("Insertar variable\n");
}

void insertar_escalar()
{
    printf("Insertar escalar\n");
}

void insertar_predefinida()
{
    printf("Insertar función predefinida\n");
}

void asignar()
{
    printf("Asignar\n");
}

void evaluar()
{
    printf("Evaluar\n");
}

void vector_negativo()
{
    printf("Vector negativo\n");
}

void escalar_negativo()
{
    printf("Escalar negativo\n");
}

void maquina_suma()
{
    printf("Suma\n");
}

void maquina_resta()
{
    printf("Resta\n");
}

void maquina_punto()
{
    printf("Producto punto\n");
}

void maquina_cruz()
{
    printf("Producto cruz\n");
}

void maquina_ppescalar()
{
    printf("Producto por escalar\n");
}

void maquina_norma()
{
    printf("Norma\n");
}

void maquina_crear_vector()
{
    printf("Crear vector\n");
}

void maquina_crear_componente()
{
    printf("Crear componente\n");
}

void maquina_crear_primer_componente()
{
    printf("Crear primer componente\n");
}
