#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "pila.h"

Nodo *fragmentar(char cadena[])
{
    char operando[TAM_MAX_NUM];
    Nodo *operandos = NULL;
    Nodo *auxiliar = NULL;
    int i, j;

    j = 0;
    for (i = 0; i < strlen(cadena); i++) {
        if (cadena[i] == '+' || cadena[i] == '-' || cadena[i] == '*' || cadena[i] == '/' || cadena[i] == '^' || \
            cadena[i] == '(' || cadena[i] == ')' || cadena[i] == '[' || cadena[i] == ']' || cadena[i] == '{' || \
            cadena[i] == '}' || cadena[i] == '\n' || cadena[i] == ' ') {
            if (operando[0] == '1' || operando[0] == '2' || operando[0] == '3' || operando[0] == '4' || \
                operando[0] == '5' || operando[0] == '6' || operando[0] == '7' || operando[0] == '8' || \
                operando[0] == '9' || operando[0] == '0') {
                operando[j] = '\0';
                auxiliar = push(auxiliar, operando);
                operando[0] = '\0';
            }

            if (cadena[i] != '\n' && cadena[i] != ' ') {
                operando[0] = cadena[i];
                operando[1] = '\0';
                auxiliar = push(auxiliar, operando);
            }

            j = 0;
        }

        if (cadena[i] == '1' || cadena[i] == '2' || cadena[i] == '3' || cadena[i] == '4' || \
            cadena[i] == '5' || cadena[i] == '6' || cadena[i] == '7' || cadena[i] == '8' || \
            cadena[i] == '9' || cadena[i] == '0') {
            operando[j] = cadena[i];
            j++;
        }
    }

    while (auxiliar != NULL) {
        operandos = push(operandos, auxiliar->dato);
        auxiliar = auxiliar->siguiente;
    }

    return operandos;
}

int string_a_int (char numero[])
{
    return atoi(numero);
}

long int evaluar(Nodo *expresion)
{
    Nodo *operandos = NULL, *caminante = NULL;
    char resultado[TAM_MAX_NUM], string_resultado[TAM_MAX_NUM];
    long int numero_1, numero_2, int_resultado;
    int contador;

    caminante = expresion;

    contador = 0;
    while (caminante != NULL) {
        if (caminante->dato[0] == '+' || caminante->dato[0] == '-' || caminante->dato[0] == '*' || caminante->dato[0] == '/' || \
            caminante->dato[0] == '^') {
            if (esta_vacia(operandos)) {
                contador++;

                break;
            }

            strcpy(string_resultado, pop(&operandos));
            numero_2 = string_a_int(string_resultado);
            
            if (esta_vacia(operandos)) {
                contador++;

                break;
            }

            strcpy(string_resultado, pop(&operandos));
            numero_1 = string_a_int(string_resultado);

            switch (caminante->dato[0]) {
                case '+':
                    int_resultado = numero_1 + numero_2;
                    break;

                case '-':
                    int_resultado = numero_1 - numero_2;
                    break;

                case '*':
                    int_resultado = numero_1 * numero_2;
                    break;

                case '/':
                    int_resultado = numero_1 / numero_2;
                    break;

                case '^':
                    int_resultado = powl(numero_1, numero_2);
                    break;
            }

            sprintf(string_resultado, "%ld", int_resultado);
            operandos = push(operandos, string_resultado);
        } else {
            operandos = push(operandos, caminante->dato);
        }

        caminante = caminante->siguiente;
        contador++;
    }

    if (esta_vacia(operandos)) {
        printf("Operador sin utilizar en la posicion: %d\n", contador);

        return 0;
    }
    
    strcpy(string_resultado, pop(&operandos));
    int_resultado = string_a_int(string_resultado);
    
    if (!esta_vacia(operandos)) {
        contador = contador - 3;
        printf("Operador sin utilizar en la posicion: %d\n", contador);

        return 0;
    }

    return int_resultado;
}
