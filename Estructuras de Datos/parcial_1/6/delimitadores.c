#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "pila.h"
#include "delimitadores.h"

void crear_indicador(int longitud, char destino[])
{
    char indicador[longitud+2];
    int i;

    for (i = 0; i < longitud+2; i++)
        indicador[i] = ' ';

    indicador[longitud+1] = '\0';
    indicador[longitud] = '\n';
    indicador[longitud-1] = '^';

    strcpy(destino, indicador);
}

void verificar_delimitadores(char expresion[])
{
    Nodo *delimitadores = NULL;
    Delimitador caracter;
    int i;

    i = 0;
    while (expresion[i] != '\0') {
        if (expresion[i] == '(' || expresion[i] == '[' || expresion[i] == '{') {
            caracter.delimitador = expresion[i];
            caracter.posicion = i+1;
            delimitadores = push(delimitadores, caracter);
        }

        if (expresion[i] == ')' || expresion[i] == ']' || expresion[i] == '}') {
            if ((expresion[i] == ')' && cima(delimitadores).delimitador == '(') || \
                (expresion[i] == ']' && cima(delimitadores).delimitador == '[') || \
                (expresion[i] == '}' && cima(delimitadores).delimitador == '{')) {
                    pop(&delimitadores);
                } else {
                    if (esta_vacia(delimitadores)) {
                        char indicador[TAM_MAX_LINEA];

                        crear_indicador(i+1, indicador);
                        printf("Delimitador sin apertura %c.\n", expresion[i]);
                        printf("%s%s", expresion, indicador);
                    }

                    break;
                }
        }

        i++;
    }

    if (!esta_vacia(delimitadores)) {
        char indicador[TAM_MAX_LINEA];

        crear_indicador(delimitadores->dato.posicion, indicador);
        printf("Delimitador sin cierre %c.\n", delimitadores->dato.delimitador);
        printf("%s%s", expresion, indicador);
    } else {
        printf("La expresion es correcta!\n");
    }

    liberar(&delimitadores);
}
