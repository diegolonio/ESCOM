#include <stdio.h>
#include <string.h>
#include <math.h>
#include "complejos.h"

float real(Complejo numero)
{
    return numero.real;
}

float imag(Complejo numero)
{
    return numero.imag;
}

Complejo sumar(Complejo numero_1, Complejo numero_2)
{
    Complejo resultado;
    
    resultado.real = numero_1.real + numero_2.real;
    resultado.imag = numero_1.imag + numero_2.imag;

    return resultado;
}

/**
 * restar - restar dos números complejos en su forma binómica.
 * 
 * @numero_1: el primer número complejo
 * @numero_2: el segundo número complejo
 * 
 * Realiza la suma de @numero_1 y el inverso aditivo de @numero_2
 * por medio de la función sumar().
 * Esto debido a que a - b = a + (-b);
 */
Complejo restar(Complejo numero_1, Complejo numero_2)
{
    numero_2.real = -1 * numero_2.real;
    numero_2.imag = -1 * numero_2.imag;

    return sumar(numero_1, numero_2);
}

Complejo multiplicar(Complejo numero_1, Complejo numero_2)
{
    Complejo resultado;

    resultado.real = numero_1.real * numero_2.real - numero_1.imag * numero_2.imag;
    resultado.imag = numero_1.real * numero_2.imag + numero_1.imag * numero_2.real;

    return resultado;
}

/**
 * reciproco - obtener el inverso multiplicativo de un número complejo en
 * su forma binómica.
 * 
 * @numero: el numero complejo del que se obtendrá su recíproco
 * 
 * Si A es un número complejo, entonces su inverso multiplicativo
 * se define como A^(-1) = 1/A.
 */
Complejo reciproco(Complejo numero)
{
    Complejo resultado;

    resultado.real = numero.real / (numero.real * numero.real + numero.imag * numero.imag);
    resultado.imag = -1 * numero.imag / (numero.real * numero.real + numero.imag * numero.imag);

    return resultado;
}

/**
 * dividir - dividir dos números complejos en su forma binómica.
 * 
 * @numero_1: el primer número
 * @numero_2: el segundo número
 * 
 * Multiplica @numero_1 por el inverso multiplicativo de @numero_2,
 * es decir, @numero_1 * (1/@numero_2). 
 */
Complejo dividir(Complejo numero_1, Complejo numero_2)
{
    return multiplicar(numero_1, reciproco(numero_2));
}

Complejo conjugado(Complejo numero)
{
    Complejo resultado;

    resultado.real = numero.real;
    resultado.imag = numero.imag * -1;

    return resultado;
}

float modulo(Complejo numero)
{
    return sqrtf(powf(numero.real, 2) + powf(numero.imag, 2));
}

float angulo(Complejo numero)
{
    return atanf(numero.imag/numero.real) * 180 / M_PI + 180;
}

Polar polar(Complejo numero)
{
    Polar resultado;

    resultado.modulo = modulo(numero);
    resultado.angulo = angulo(numero);

    return resultado;
}

void mostrarp(Polar numero)
{
    char modulo[TAM_MAX_NUM];
    char angulo[TAM_MAX_NUM];
    char polar_formateado[TAM_MAX_NUM];

    /**
     * Convertimos el valor del módulo y el ángulo de @numero
     * a cadenas. Los valores tienen 2 cifras decimales.  
     */
    sprintf(modulo, "%.2f", numero.modulo);
    sprintf(angulo, "%.2f", numero.angulo);

    /**
     * El formato que decidí utilizar para mostrar un número
     * complejo en su forma polar es: modulo;ángulo°
     */
    strcpy(polar_formateado, modulo);
    strcat(polar_formateado, ";");
    strcat(polar_formateado, angulo);
    strcat(polar_formateado, "°");

    printf("%s", polar_formateado);
}

void mostrar(Complejo numero)
{
    char real[TAM_MAX_NUM];
    char imag[TAM_MAX_NUM];
    char imag_formateado[TAM_MAX_NUM];
    char complejo_formateado[TAM_MAX_NUM];

    /**
     * Convertimos el valor del módulo y el ángulo de @numero
     * a cadenas. Los valores tienen 2 cifras decimales.  
     */
    sprintf(real, "%.2f", numero.real);
    sprintf(imag, "%.2f", numero.imag);

    /**
     * El formato que decidí utilizar para mostrar un número
     * complejo en su forma binómica es: a±ib
     */
    agregar_signo_i(imag, imag_formateado);

    strcpy(complejo_formateado, real);
    strcat(complejo_formateado, imag_formateado);

    printf("%s", complejo_formateado);
}

/**
 * agregar_signo_i - agregar el signo de la parte imaginaria de
 * un número complejo así como la unidad imaginaria i.
 * 
 * @numero: parte imaginaria de cierto número complejo
 * @destino: arreglo de caracteres en el que se va a copiar
 * la parte imaginaria ya formateada.
 * 
 * Ya que decidí representar a un número complejo en su forma
 * binómica con el formato: a±ib, es necesario dar a la parte
 * imaginaria del numero complejo el formato: ±ib.
 */
void agregar_signo_i(char numero[], char destino[])
{
    int longitud; // Número de caracteres de @numero
    int indice;

    longitud = strlen(numero) + 1; // +1 para agregar '\0'
    char tmp[longitud+2]; // +2 para el signo y la unidad imaginaria 1

    /**
     * En el formato: ±ib, el segundo caracter es la unidad
     * imaginaria i, es decir, la posición 1 del arreglo tmp[]
     */
    tmp[1] = 'i';

    if (numero[0] != '-') { // Si el número NO es negativo
        tmp[0] = '+'; 
        
        /* Copiamos el número positivo */
        for (indice = 0; indice < longitud; indice++)
            tmp[indice+2] = numero[indice];
    } else {
        tmp[0] = '-';

        /* Copiamos el número negativo sin el signo */
        for (indice = 1; indice < longitud; indice++)
            tmp[indice+1] = numero[indice];
    }

    strcpy(destino, tmp);
}
