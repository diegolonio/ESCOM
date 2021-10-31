#ifndef _FRACCIONES_H_
#define _FRACCIONES_H_

typedef struct Fraccion {
    int numerador;
    int denominador;
} Fraccion;

Fraccion sumar(Fraccion a, Fraccion b);
Fraccion restar(Fraccion a, Fraccion b);
Fraccion multiplicar(Fraccion a, Fraccion b);
Fraccion dividir(Fraccion a, Fraccion b);
Fraccion simplificar(Fraccion a);
/* Máximo común divisor */
int mcd(int a, int b);

#include "fracciones.c"
#endif /* _FRACCIONES_H_ */