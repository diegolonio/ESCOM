#include <unistd.h>
#include <stdio.h>
#include <string.h>

void strrev(char cadena[])
{
	int i;
	int longitud = strlen(cadena);

	for (i = 0; i < (int)(longitud/2); i++) {
		cadena[i] = cadena[i] + cadena[longitud-i-1];
		cadena[longitud-i-1] = cadena[i] - cadena[longitud-i-1];
		cadena[i] = cadena[i] - cadena[longitud-i-1];
	}
}

void binario(char cadena[], int numero)
{
	char digito[2];
	int auxiliar = numero;

	sprintf(digito, "%d", auxiliar%2);
	strcpy(cadena, digito);
	auxiliar /= 2;

	while (auxiliar != 0) {
		sprintf(digito, "%d", auxiliar%2);
		strcat(cadena, digito);
		auxiliar /= 2;
	}

	strrev(cadena);
}

void rellenar(char cadena_rellenada[], char cadena[], char relleno[], int longitud, char lado[])
{
	int i;

	if (!strcmp(lado, "d")) {
		strcpy(cadena_rellenada, cadena);

		for (i = 0; i < longitud-strlen(cadena); i++)
			strcat(cadena_rellenada, relleno);
	} else {
		if (strlen(cadena) < longitud) {
			strcpy(cadena_rellenada, relleno);

			for (i = 0; i < longitud-strlen(cadena)-1; i++)
				strcat(cadena_rellenada, relleno);

			strcat(cadena_rellenada, cadena);
		} else {
			strcpy(cadena_rellenada, cadena);
		}
	}
}

void virtual(char cadena_direccion_virtual[], int pagina, int direccion)
{
	int i;
	char cadena_pagina[5];
	char cadena_direccion[13];
	char cadena_pagina_rellenada[5];
	char cadena_direccion_rellenada[13];
	
	binario(cadena_pagina, pagina);
	binario(cadena_direccion, direccion);

	rellenar(cadena_pagina_rellenada, cadena_pagina, "0", 4, "i");
	rellenar(cadena_direccion_rellenada, cadena_direccion, "0", 12, "i");

	strcpy(cadena_direccion_virtual, cadena_pagina_rellenada); 
	strcat(cadena_direccion_virtual, cadena_direccion_rellenada);
}

void fisica(char cadena_direccion_fisica[], int marco, int direccion)
{
	int i;
	char cadena_marco[4];
	char cadena_direccion[13];
	char cadena_marco_rellenada[4];
	char cadena_direccion_rellenada[13];
	
	binario(cadena_marco, marco);
	binario(cadena_direccion, direccion);

	rellenar(cadena_marco_rellenada, cadena_marco, "0", 3, "i");
	rellenar(cadena_direccion_rellenada, cadena_direccion, "0", 12, "i");

	strcpy(cadena_direccion_fisica, cadena_marco_rellenada);
	strcat(cadena_direccion_fisica, cadena_direccion_rellenada);
}

int main(void)
{
	char cadena_direccion[13];
	char cadena_pagina[5];
	char cadena_marco[4];
	char direccion_virtual[17];
	char direccion_fisica[16];

	int direccion;
	int pagina;
	int marco = 2;

	binario(cadena_marco, marco);

	for (pagina = 0; pagina < 16; pagina++) {
		binario(cadena_pagina, pagina);

		for (direccion = 0; direccion < 4096; direccion++) {
			binario(cadena_direccion, direccion);

			virtual(direccion_virtual, pagina, direccion);
			fisica(direccion_fisica, marco, direccion);

			printf("\nDireccion: %s\nPagina: %s\nMarco: %s\n\n", cadena_direccion, cadena_pagina, cadena_marco);
			printf("Direccion virtual: %s\n", direccion_virtual);
			printf(" Direccion fisica: %s\n", direccion_fisica);

			sleep(5);
		}
	}
	
	return 0;
}
