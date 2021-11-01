#ifndef _COMPLEJOS_H_
#define _COMPLEJOS_H_

/**
 * Tamaño máximo de caracteres con los que se puede
 * representar a un número complejo tanto en su forma
 * binómica como en su forma polar, así como cualquier
 * otro tipo de número.
 */
#define TAM_MAX_NUM 20

typedef struct Complejo {
    float real;
    float imag;
} Complejo;

typedef struct Polar {
    float modulo;
    float angulo;
} Polar;

float real (Complejo numero);
float imag (Complejo numero);
Complejo sumar(Complejo numero_1, Complejo numero_2);
Complejo restar(Complejo numero_1, Complejo numero_2);
Complejo multiplicar(Complejo numero_1, Complejo numero_2);
Complejo reciproco(Complejo numero);
Complejo dividir(Complejo numero_1, Complejo numero_2);
Complejo conjugado(Complejo numero_1);
float modulo(Complejo numero);
float angulo(Complejo numero);
Polar polar(Complejo numero);
void mostrar(Complejo numero);
void mostrarp(Polar numero);
void agregar_signo_i(char numero[], char destino[]);

#include "complejos.c"

#endif /* _COMPLEJOS_H_ */
