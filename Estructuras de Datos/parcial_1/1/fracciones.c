#include <stdio.h>
#include "fracciones.h"

Fraccion sumar(Fraccion a, Fraccion b)
{
    Fraccion resultado;

    resultado.numerador = (a.numerador * b.denominador) + (b.numerador * a.denominador);
    resultado.denominador = a.denominador * b.denominador;

    return simplificar(resultado);
}

/**
 * restar - restar dos fracciones.
 * 
 * @a: La primera fracción
 * @b: La segunda fracción
 * 
 * La función multiplica el numerador de @b por -1 y realiza
 * la suma de ambas fracciones por medio de la función sumar().
 * Esto partiendo del hecho que a - b = a + (-b).
 */
Fraccion restar(Fraccion a, Fraccion b)
{
    b.numerador = -1 * b.numerador;
    
    return simplificar(sumar(a, b));
}

Fraccion multiplicar(Fraccion a, Fraccion b)
{
    Fraccion resultado;

    resultado.numerador = a.numerador * b.numerador;
    resultado.denominador = a.denominador * b.denominador;

    return simplificar(resultado);
}

/**
 * dividir - división de fracciones.
 * 
 * @a: La primera fracción
 * @b: La segunda fracción
 * 
 * Se intercambian el numerador y el denominador de @b y luego se
 * multiplican ambas fracciones.
 * Esto es debido a que (a/b)/(c/d) = ad/bc = (a/b)*(d/c).
 */
Fraccion dividir(Fraccion a, Fraccion b)
{
    int auxiliar = b.numerador; // Variable auxiliar para hacer el intercambio
    
    b.numerador = b.denominador;
    b.denominador = auxiliar;

    return simplificar(multiplicar(a, b));
}

/**
 * simplificar - simplificar una fracción utilizando el Máximo Común Divisor de su numerador y denominador.
 * 
 * @a: La fracción que va a ser simplificada
 */
Fraccion simplificar(Fraccion a)
{
    int maximo_comun_divisor;

    maximo_comun_divisor = mcd(a.numerador, a.denominador);
    a.numerador = a.numerador / maximo_comun_divisor;
    a.denominador = a.denominador / maximo_comun_divisor;

    return a;
}

/**
 * mcd - obtener el Máximo Común Divisor de dos números utilizando el Algoritmo de Euclides.
 * 
 * @a: El primer número
 * @b: El segundo número
 * 
 * Retorna el Máximo Común Divisor de ambos números.
 */
int mcd(int a, int b)
{
    if (b == 0)
        return a;

    return mcd(b, a % b);
}

void mostrar(Fraccion a)
{
    printf("(%d/%d)", a.numerador, a.denominador);
}
