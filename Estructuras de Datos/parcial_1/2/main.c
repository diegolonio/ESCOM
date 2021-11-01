#include <stdio.h>
#include "complejos.h"

int main (void)
{
    Complejo un_numero;
    Complejo otro_numero;

    un_numero.real = -7.0;
    un_numero.imag = 9.0;
    otro_numero.real = 9.0;
    otro_numero.imag = -10.0;

    mostrar(un_numero); printf("\n");
    mostrar(otro_numero); printf("\n");
    mostrar(sumar(un_numero, otro_numero)); printf("\n");
    mostrar(restar(un_numero, otro_numero)); printf("\n");
    mostrar(multiplicar(un_numero, otro_numero)); printf("\n");
    mostrar(dividir(un_numero, otro_numero)); printf("\n");
    mostrarp(polar(un_numero)); printf("\n");
    mostrarp(polar(otro_numero)); printf("\n");

    return 0;
}