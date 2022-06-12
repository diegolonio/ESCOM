/* 
 * Nombre: Apolonio Villegas Diego Armando 
 * Práctica: 7
 * Tema: Funciones y procedimientos
 * Opción: Calculadora de vectores
 * Fecha 27/11/2021
 * Grupo: 3CM17
 * Materia: Compiladores
 */

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
Instruccion *cima_subprograma = programa; /* Inicio del subprograma actual */
bool retornando; /* true si hay una sentencia 'return' */

/* Configuración de la pila de función/procedimiento */
#define NUMMARCOS 100
Marco marcos[NUMMARCOS];
Marco *cima_marcos;

/* ---------------------------- Funciones sobre la máquina ---------------------------- */

void iniciar_codigo() /* Iniciar la generación de código */
{
    cima_programa = cima_subprograma;
    cima_pila = pila;
    cima_marcos = marcos;
    retornando = false;
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
    for (contador_programa = instruccion; *contador_programa != PARO && !retornando; )
            (*(*contador_programa++))();
}

/* ---------------------- Funciones sobre la pila del intérprete ---------------------- */

Datum pop() /* Sacar el elemento de la cima de la pila del interprete */
{
    if (cima_pila <= pila)
        ejecutar_error("no hay más elementos en la pila", (char *)0);

    return *--cima_pila;
}

void push(Datum nuevo_elemento) /* Insertar un elemento en la cima de la pila del intérprete */
{
    if (cima_pila >= &pila[TAMPILA])
        ejecutar_error("tamaño de pila excedido", (char *)0);

    *cima_pila++ = nuevo_elemento;
}

/* --------------------------- Instrucciones de la máquina --------------------------- */

void imprimir_vector()
{
    Datum elemento;

    elemento = pop();
    mostrar_vector(elemento.vector);
}

void imprimir_escalar()
{
    Datum elemento;

    elemento = pop();
    printf("%d", elemento.escalar);
}

void imprimir_cadena()
{
    printf("%s", (char *)*contador_programa++);
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

void o_logico()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento1.escalar = (int)(elemento1.escalar != 0 || elemento2.escalar != 0);
    push(elemento1);
}

void y_logico()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento1.escalar = (int)(elemento1.escalar != 0 && elemento2.escalar != 0);
    push(elemento1);
}

void mayor_que()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento1.escalar = (int)(elemento1.escalar > elemento2.escalar);
    push(elemento1);
}

void mayor_igual()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento1.escalar = (int)(elemento1.escalar >= elemento2.escalar);
    push(elemento1);
}

void menor_que()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento1.escalar = (int)(elemento1.escalar < elemento2.escalar);
    push(elemento1);
}

void menor_igual()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento1.escalar = (int)(elemento1.escalar <= elemento2.escalar);
    push(elemento1);
}

void vectores_iguales()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento1.escalar = (int)son_iguales(elemento1.vector, elemento2.vector);
    push(elemento1);
}

void vectores_no_iguales()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento1.escalar = (int)(!son_iguales(elemento1.vector, elemento2.vector));
    push(elemento1);
}

void escalares_iguales()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento1.escalar = (int)(elemento1.escalar == elemento2.escalar);
    push(elemento1);
}

void escalares_no_iguales()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento1.escalar = (int)(elemento1.escalar != elemento2.escalar);
    push(elemento1);
}

void modulo()
{
    Datum elemento1, elemento2;

    elemento2 = pop();
    elemento1 = pop();
    elemento1.escalar = (int)(elemento1.escalar % elemento2.escalar);
    push(elemento1);
}

void negacion()
{
    Datum elemento;

    elemento = pop();
    elemento.escalar = (int)(elemento.escalar == 0);
    push(elemento);
}

void si()
{
    Datum elemento;
    Instruccion *guardar_contador_programa = contador_programa; /* Cuerpo del condicional */

    ejecutar(guardar_contador_programa+3); /* Evalución de la condición */
    elemento = pop();

    if (elemento.escalar)
        ejecutar(*((Instruccion **)(guardar_contador_programa)));
    else if (*((Instruccion **)(guardar_contador_programa+1)))
        ejecutar(*((Instruccion **)(guardar_contador_programa+1))); /* Si la condición no se cumplió */

    if (!retornando)
        contador_programa = *((Instruccion **)(guardar_contador_programa+2)); /* Siguiente instrucción después la secuencia de instrucciones del condicional */
}

void mientras()
{
    Datum elemento;
    Instruccion *guardar_contador_programa = contador_programa; /* Cuerpo del ciclo */

    ejecutar(guardar_contador_programa+2); /* Evaluación de la condición */
    elemento = pop();

    while (elemento.escalar) {
        ejecutar(*((Instruccion **)(guardar_contador_programa))); /* Cuerpo del ciclo */

        if (retornando)
            break;

        ejecutar(guardar_contador_programa+2); /* Evaluación de la condición */
        elemento = pop();
    }

    if (!retornando)
        contador_programa = *((Instruccion **)(guardar_contador_programa+1)); /* Siguiente instrucción después de la secuencia de instrucciones del ciclo */
}

void para()
{
    Datum elemento;
    Instruccion *guardar_contador_programa = contador_programa; /* Cuerpo del ciclo */

    ejecutar(guardar_contador_programa+4); /* Declaración de la variable de control */
    ejecutar(*((Instruccion **)(guardar_contador_programa))); /* Evaluación de la condición */

    elemento = pop();
    
    while (elemento.escalar) {
        ejecutar(*((Instruccion **)(guardar_contador_programa+2))); /* Cuerpo del ciclo */

        if (retornando)
            break;

        ejecutar(*((Instruccion **)(guardar_contador_programa+1))); /* Paso */
        ejecutar(*((Instruccion **)(guardar_contador_programa))); /* Evaluación de la condición */
        elemento = pop();
    }

    if (!retornando)
        contador_programa = *((Instruccion **)(guardar_contador_programa+3)); /* Siguiente instrucción después de la secuencia de instrucciones del ciclo */
}

void llamada()
{
    Simbolo *simbolo = (Simbolo *)contador_programa[0]; /* Entrada de la tabla de símbolos con la función */

    if (cima_marcos++ >= &marcos[NUMMARCOS-1])
        ejecutar_error(simbolo->nombre, "(func/proc) tamaño de pila excedido");

    cima_marcos->simbolo = simbolo;
    cima_marcos->numero_argumentos = (int)contador_programa[1];
    cima_marcos->instruccion_despues_de_retornar = contador_programa + 2;
    cima_marcos->nesimo_argumento = cima_pila - 1; /* Último argumento */
    ejecutar(simbolo->u.definicion);
    retornando = false;
}

void retornar() /* Retornar común de una función o de un procedimiento */
{
    int contador;

    for (contador = 0; contador < cima_marcos->numero_argumentos; contador++)
        pop();

    contador_programa = (Instruccion *)cima_marcos->instruccion_despues_de_retornar;
    --cima_marcos;
    retornando = true;
}

void funcion_retornar() /* Retornar desde una función */
{
    Datum elemento;

    if (cima_marcos->simbolo->tipo == PROCEDIMIENTO)
        ejecutar_error(cima_marcos->simbolo->nombre, "(proc) retorna un valor");

    elemento = pop(); /* Mantener el valor que retornó la función */
    retornar();
    push(elemento);
}

void procedimiento_retornar() /* Retornar desde un procedimiento */
{
    if (cima_marcos->simbolo->tipo == FUNCION)
        ejecutar_error(cima_marcos->simbolo->nombre, "(func) no retorna un valor");

    retornar();
}

Vector **obtener_argumento()
{
    int numero_argumentos = (int)*contador_programa++;

    if (numero_argumentos > cima_marcos->numero_argumentos)
        ejecutar_error(cima_marcos->simbolo->nombre, "faltan argumentos");

    return &cima_marcos->nesimo_argumento[numero_argumentos-cima_marcos->numero_argumentos].vector;
}

void argumento()
{
    Datum elemento;

    elemento.vector = *obtener_argumento();
    push(elemento);
}

void asignacion_argumento()
{
    Datum elemento;

    elemento = pop();
    push(elemento);
    *obtener_argumento() = elemento.vector;
}
