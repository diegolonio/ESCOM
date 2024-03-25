#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "../../include/calculadora.h"
#include "../../include/redes.h"

int main()
{
    bool bandera = false;
    int opcion, parametro, prefijo, subred, contador = 1, cantidad_subredes;
    char listar, cadena_id[17], cadena_ip[17];
    unsigned char id[4], ip[4], mascara[4];
    FILE *direcciones = NULL;

    printf("Ingresa la direccion IP: ");
    fgets(cadena_id, sizeof(cadena_id), stdin);

    cadena_id[strlen(cadena_id)-1] = '\0'; // Se elimina el salto de línea al final

    if (ip_arreglo(id, cadena_id) == false) {
        fprintf(stderr, "%s", "Direccion de subred no valida\n");
        exit(1);
    }

    if (es_id_subred(id) == false) {
        fprintf(stderr, "%s", "La direccion ingresada no es un identificador de subred\n");
        exit(1);
    }

    if (es_privada(id) == true) {
        printf("\n Es una red privada");
    }

    switch (clase_teorica(id)) {
        case 1:
            if (es_privada(id) == true) {
                printf("\n Mascara de subred: 255.0.0.0 (/8)\n");
            } else {
                printf("\n Clase A\n Mascara de subred: 255.0.0.0 (/8)\n");
            }
            break;
        case 2:
        	if (es_privada(id) == true) {
        		printf("\n Mascara de subred: 255.240.0.0 (/12)\n");
        	} else {
            	printf("\n Clase B\n Mascara de subred: 255.255.0.0 (/16)\n");
        	}
            break;
        case 3:
        	if (es_privada(id) == true) {
        		printf("\n Mascara de subred: 255.255.0.0 (/16)\n");
        	} else {
	            printf("\n Clase C\n Mascara de subred: 255.255.255.0 (/24)\n");
	        }
            break;
        default:
            printf("La direccion IP que ingresaste no entra en las clasificaciones A, B o C.\n");
            exit(1);
            break;
    }

    printf("\nElige la especificacion que quieres definir de la red: \n");
    printf("\n 1. Numero de hosts\n 2. Numero de subredes\n 3. Prefijo\n\nElegir la opcion numero: ");

    scanf("%d", &opcion);
    fgetc(stdin);

    switch (opcion) {
        case 1:
            printf("\n Numero de hosts: ");

            if (entero(&parametro) == true) {
                bandera = hosts(id, clase_practica(id), parametro, NULL, &cantidad_subredes, mascara, &prefijo);
            }

            break;
        case 2:
            printf("\n Numero de subredes: ");

            if (entero(&parametro) == true) {
                bandera = subredes(id, clase_practica(id), parametro, NULL, &cantidad_subredes, mascara, &prefijo);
            }

            break;
        case 3:
            printf("\n Prefijo: /");

            if (entero(&parametro) == true) {
                bandera = pre(id, clase_practica(id), parametro, NULL, &cantidad_subredes, mascara, &prefijo);
            }

            break;
        default:
            fprintf(stderr, "%s", " Opcion no valida\n");
            exit(1);
            break;
    }

    if (bandera == false) {
        exit(1);
    }

    fgetc(stdin);

    if (cantidad_subredes > 0) {
        printf("\nQuieres listar los identifcadores de las subredes? (s/n): ");
        listar = fgetc(stdin);
        fgetc(stdin);

        if (listar == 'n' || listar == 'N') {
            exit(1);
        }

        if ((listar != 's' && listar != 'S')) {
            printf("Anulado\n");
            exit(1);
        }

        printf("Calculando identificadores de subredes...\n\n");
        calcular_subredes(id, mascara);
        listar_subredes(prefijo);
    }

    if (cantidad_subredes > 0) {
        printf("\nQuieres listar las direcciones IP de los hosts de una subred en especifico? (s/n): ");
    } else {
        printf("\nQuieres listar las direcciones IP de los hosts? (s/n): ");
    }

    listar = fgetc(stdin);
    fgetc(stdin);

    if (listar == 'n' || listar == 'N') {
        system("rm -f subredes");
        exit(1);
    }

    if ((listar != 's' && listar != 'S')) {
        printf("Anulado\n");
        system("rm -f subredes");
        exit(1);
    }

    if (cantidad_subredes > 0) {
        printf("Ingresa el numero de subred: ");
        
        if (entero(&subred) == false) {
            system("rm -f subredes");
            exit(1);
        }

        if (subred > cantidad_subredes || subred < 1) {
            fprintf(stderr, "%s", "La subred ingresada no existe\n");
            system("rm -f subredes");
            exit(1);
        }

        direcciones = fopen("subredes", "r");

        while (fgets(cadena_ip, sizeof(cadena_ip), direcciones) != NULL) {
            if (contador == subred) {
                cadena_ip[strlen(cadena_ip)-1] = '\0';
                break;
            }

            contador = contador + 1;
        }

        fclose(direcciones);
        ip_arreglo(ip, cadena_ip);
    }

    printf("Calculando identificadores de hosts...\n\n");

    if (cantidad_subredes > 0) {
        calcular_hosts(ip, mascara);
    } else {
        calcular_hosts(id, mascara);
    }

    listar_hosts();

    return 0;
}
