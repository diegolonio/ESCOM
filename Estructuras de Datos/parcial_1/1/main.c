#include <stdio.h>
#include "fracciones.h"

int main(void)
{
    Fraccion fraccion_a, fraccion_b, resultado;
    int opc; // Para guardar la opción que el usuario elija
    char nosi; // Para guardar el sí/no del usuario al finalizar una operación

    do {
        printf("---------------------------------------------------\n");
        printf("Ingresa el numero de operacion que deseas realizar: \n\n");
        printf("1) Suma\n2) Resta\n3) Multiplicacion\n4) Division\n\nOpcion numero: ");
        scanf("%d", &opc);

        if (opc >= 1 && opc <= 4) { // Si el usuario ingresa una opción válida
            printf("\nIngrese el numerador de la primera fraccion: ");
            scanf("%d", &fraccion_a.numerador);
            printf("Ingrese el denominador de la primera fraccion: ");
            scanf("%d", &fraccion_a.denominador);
            printf("\nIngrese el numerador de la segunda fraccion: ");
            scanf("%d", &fraccion_b.numerador);
            printf("Ingrese el denominador de la segunda fraccion: ");
            scanf("%d", &fraccion_b.denominador);

            if (fraccion_a.denominador == 0 || fraccion_b.denominador == 0) {
                printf("\nDivision entre cero.\n");
                break;
            }

            printf("\n");
            mostrar(fraccion_a);
        }

        switch (opc) {
            case 1:
                resultado = sumar(fraccion_a, fraccion_b);
                printf(" + ");
                break;
            case 2:
                resultado = restar(fraccion_a, fraccion_b);
                printf(" - ");
                break;
            case 3:
                resultado = multiplicar(fraccion_a, fraccion_b);
                printf(" * ");
                break;
            case 4:
                resultado = dividir(fraccion_a, fraccion_b);
                printf(" / ");
                break;
            default:
                printf ("\nOpcion no valida.\n");
                break;
        }

        if (opc >= 1 || opc <= 4) {
            mostrar(fraccion_b);
            printf(" = ");
            mostrar(resultado);
            printf("\n");
        }

        printf ("\nDesea realizar otra operacion? (s/n): ");
        scanf ("%s", &nosi);
    }  while (nosi == 'S' || nosi == 's');

    printf("---------------------------------------------------\n");

    return 0;
}