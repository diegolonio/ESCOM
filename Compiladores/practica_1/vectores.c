#include <stdio.h>
#include <math.h>
#include "vectores.h"

/* Implementación de vectores */

Componente *crear_componente(int valor, Componente *siguiente)
{
	Componente *nueva_componente = NULL;

	nueva_componente = (Componente *)malloc(sizeof(Componente));
	nueva_componente->valor = valor;
	nueva_componente->siguiente = siguiente;

	return nueva_componente;
}

Vector *crear_vector(Componente *componente)
{
	Vector *nuevo_vector = NULL;

	nuevo_vector = (Vector *)malloc(sizeof(Vector));
	nuevo_vector->componente_inicial = componente;
	nuevo_vector->caminante = nuevo_vector->componente_inicial;

	return nuevo_vector;
}

void insertar_componente(Vector *vector, int valor)
{
	Componente *nueva_componente = crear_componente(valor, NULL);

	if (vector->caminante == NULL) {
		vector->componente_inicial = nueva_componente;
		vector->caminante = vector->componente_inicial;
	} else {
		while (vector->caminante->siguiente != NULL)
			vector->caminante = vector->caminante->siguiente;

		vector->caminante->siguiente = nueva_componente;
		vector->caminante = vector->componente_inicial;
	}
}

void mostrar_vector(Vector *vector)
{
	printf("[%d", vector->caminante->valor);
	vector->caminante = vector->caminante->siguiente;

	while (vector->caminante != NULL) {
		printf(", %d", vector->caminante->valor);
		vector->caminante = vector->caminante->siguiente;
	}

	printf("]\n");
	vector->caminante = vector->componente_inicial;
}

void liberar_vector(Vector *vector)
{
	Componente *auxiliar = vector->caminante->siguiente;

	while (vector->componente_inicial->siguiente != NULL) {
		while (auxiliar->siguiente != NULL) {
			vector->caminante = vector->caminante->siguiente;
			auxiliar = auxiliar->siguiente;
		}

		vector->caminante->siguiente = NULL;
		free(auxiliar);
		vector->caminante = vector->componente_inicial;
		auxiliar = vector->caminante->siguiente;
	}

	auxiliar = vector->componente_inicial;
	vector->componente_inicial = NULL;
	vector->caminante = NULL;
	free(auxiliar);
	free(vector);
}

/* Operaciones con vectores */

Vector *suma(Vector *vector_a, Vector* vector_b)
{
	Vector *resultado = crear_vector(NULL);

	while (vector_a->caminante != NULL && vector_b->caminante != NULL) {
		insertar_componente(resultado, vector_a->caminante->valor+vector_b->caminante->valor);
		vector_a->caminante = vector_a->caminante->siguiente;
		vector_b->caminante = vector_b->caminante->siguiente;
	}

	vector_a->caminante = vector_a->componente_inicial;
	vector_b->caminante = vector_b->componente_inicial;
	
	return resultado;
}

Vector *resta(Vector *vector_a, Vector *vector_b)
{
	Vector *resultado = crear_vector(NULL);

	while (vector_a->caminante != NULL && vector_b->caminante != NULL)
	{
		insertar_componente(resultado, vector_a->caminante->valor-vector_b->caminante->valor);
		vector_a->caminante = vector_a->caminante->siguiente;
		vector_b->caminante = vector_b->caminante->siguiente;
	}

	vector_a->caminante = vector_a->componente_inicial;
	vector_b->caminante = vector_b->componente_inicial;

	return resultado;
}

int punto(Vector *vector_a, Vector *vector_b)
{
	int resultado = 0;

	while (vector_a->caminante != NULL && vector_b->caminante != NULL)
	{
		resultado += vector_a->caminante->valor * vector_b->caminante->valor;
		vector_a->caminante = vector_a->caminante->siguiente;
		vector_b->caminante = vector_b->caminante->siguiente;
	}

	vector_a->caminante = vector_a->componente_inicial;
	vector_b->caminante = vector_b->componente_inicial;

	return resultado;
}

Vector *cruz(Vector *vector_a, Vector *vector_b)
{
	Vector *resultado = crear_vector(NULL);
	int componentes_a[3], componentes_b[3], contador;

	for (contador = 0; contador < 3; contador++) {
		componentes_a[contador] = vector_a->caminante->valor;
		componentes_b[contador] = vector_b->caminante->valor;
		vector_a->caminante = vector_a->caminante->siguiente;
		vector_b->caminante = vector_b->caminante->siguiente;
	}

	insertar_componente(resultado, componentes_a[1]*componentes_b[2]-componentes_a[2]*componentes_b[1]);
	insertar_componente(resultado, componentes_a[2]*componentes_b[0]-componentes_a[0]*componentes_b[2]);
	insertar_componente(resultado, componentes_a[0]*componentes_b[1]-componentes_a[1]*componentes_b[0]);

	vector_a->caminante = vector_a->componente_inicial;
	vector_b->caminante = vector_b->componente_inicial;
	
	return resultado;
}

Vector *ppescalar(int escalar, Vector *vector)
{
	Vector *resultado = crear_vector(NULL);

	while (vector->caminante != NULL) {
		insertar_componente(resultado, vector->caminante->valor*escalar);
		vector->caminante = vector->caminante->siguiente;
	}

	vector->caminante = vector->componente_inicial;

	return resultado;
}

int norma(Vector *vector)
{
	double resultado = 0.0;

	while (vector->caminante != NULL) {
		resultado += pow((double)vector->caminante->valor, 2);
		vector->caminante = vector->caminante->siguiente;
	}

	vector->caminante = vector->componente_inicial;
	resultado = sqrt(resultado);

	return (int)resultado;
}
