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

void ejecutar(Instruccion *instruccion) /* Ejecutar una instrucción de la máquina */
{
    for (contador_programa = instruccion; *contador_programa != PARO; )
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
    Datum elemento;

    elemento = pop();
    printf("\t");
    mostrar_vector(elemento.vector);
}

void imprimir_escalar()
{
    Datum elemento;

    elemento = pop();
    printf("\t%d\n", elemento.escalar);
}

void insertar_variable()
{
    Datum elemento;

    elemento.simbolo = (Simbolo *)(*contador_programa++);
    push(elemento);
}

void insertar_escalar()
{
    Datum elemento;

    elemento.escalar = ((Simbolo *)*contador_programa++)->u.escalar;
    push(elemento);
}

void ejecutar_predefinida()
{
    Datum elemento;

    elemento = pop();
    elemento.escalar = (*(int (*)())(*contador_programa++))(elemento.vector);
    push(elemento);
}

void asignar()
{
    Datum variable, expresion;

    variable = pop();
    expresion = pop();

    if (variable.simbolo->tipo != VARIABLE && variable.simbolo->tipo != INDEFINIDO)
        ejecutar_error("asignación a no variable", variable.simbolo->nombre);

    variable.simbolo->u.vector = expresion.vector;
    variable.simbolo->tipo = VARIABLE;
    push(variable);
}

void evaluar()
{
    Datum variable;

    variable = pop();

    if (variable.simbolo->tipo == INDEFINIDO)
        ejecutar_error("variable no definida", variable.simbolo->nombre);

    variable.vector = variable.simbolo->u.vector;
    push(variable);
}

void vector_negativo()
{
    Datum elemento;

    elemento = pop();
    elemento.vector = ppescalar(-1, elemento.vector);
    push(elemento);
}

void escalar_negativo()
{
    Datum elemento;

    elemento = pop();
    elemento.escalar = -1 * elemento.escalar;
    push(elemento);
}

void maquina_suma()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();

    if (dimension(elemento1.vector) != dimension(elemento2.vector))
        ejecutar_error("vectores de tamaño diferente", (char *)0);

    elemento1.vector = suma(elemento1.vector, elemento2.vector);
    push(elemento1);
}

void maquina_resta()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();

    if (dimension(elemento1.vector) != dimension(elemento2.vector))
        ejecutar_error("vectores de tamaño diferente", (char *)0);

    elemento1.vector = resta(elemento1.vector, elemento2.vector);
    push(elemento1);
}

void maquina_punto()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();

    if (dimension(elemento1.vector) != dimension(elemento2.vector))
        ejecutar_error("vectores de tamaño diferente", (char *)0);

    elemento1.escalar = punto(elemento1.vector, elemento2.vector);
    push(elemento1);
}

void maquina_cruz()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();

    if (dimension(elemento1.vector) != 3 || dimension(elemento2.vector) != 3)
        ejecutar_error("vectores de dimensión distinta a 3", (char *)0);

    elemento1.vector = cruz(elemento1.vector, elemento2.vector);
    push(elemento1);
}

void maquina_ppescalar_escexp()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento2.vector = ppescalar(elemento1.escalar, elemento2.vector);
    push(elemento2);
}

void maquina_ppescalar_expesc()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento1.vector = ppescalar(elemento2.escalar, elemento1.vector);
    push(elemento1);
}

void maquina_norma()
{
    Datum elemento;

    elemento = pop();
    elemento.escalar = norma(elemento.vector);
    push(elemento);
}

void maquina_crear_vector()
{
    Datum elemento;

    elemento = pop();
    elemento.vector = crear_vector(elemento.componente);
    push(elemento);
}

void maquina_crear_componente()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento2.componente = crear_componente(elemento2.escalar, elemento1.componente);
    push(elemento2);
}

void maquina_crear_primer_componente()
{
    Datum elemento;

    elemento = pop();
    elemento.componente = crear_componente(elemento.escalar, NULL);
    push(elemento);
}
