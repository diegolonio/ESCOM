/* 
 * Nombre: Apolonio Villegas Diego Armando 
 * Práctica: 5
 * Tema: Decisiones y ciclos
 * Opción: Calculadora de vectores
 * Fecha 18/11/2021
 * Grupo: 3CM17
 * Materia: Compiladores
 */

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
	if (vector == NULL)
		return;
	
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
	if (vector == NULL)
		return;

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

int dimension(Vector *vector)
{
	int dimension = 0;

	while (vector->caminante != NULL) {
		dimension = dimension + 1;
		vector->caminante = vector->caminante->siguiente;
	}

	vector->caminante = vector->componente_inicial;

	return dimension;
}

bool son_iguales(Vector *vector_a, Vector * vector_b)
{
	Componente *auxiliar = NULL;
	bool son_iguales = false;

	auxiliar = vector_b->componente_inicial;

	while (vector_a->caminante != NULL) {
		if (auxiliar->valor == vector_a->caminante->valor)
			son_iguales = true;
		else
			son_iguales = false;

		auxiliar = auxiliar->siguiente;
		vector_a->caminante = vector_a->caminante->siguiente;
	}

	vector_a->caminante = vector_a->componente_inicial;

	return son_iguales;
}

Vector *duplicar(Vector *vector)
{
	Vector *nuevo_vector = crear_vector(NULL);

	while (vector->caminante != NULL) {
		insertar_componente(nuevo_vector, vector->caminante->valor);
		vector->caminante = vector->caminante->siguiente;
	}

	vector->caminante = vector->componente_inicial;

	return nuevo_vector;
}

/* Operaciones con vectores */

Vector *suma(Vector *vector_a, Vector* vector_b)
{
	Vector *resultado = crear_vector(NULL);
	Vector *auxiliar = duplicar(vector_b);


	while (vector_a->caminante != NULL && auxiliar->caminante != NULL) {
		insertar_componente(resultado, vector_a->caminante->valor+auxiliar->caminante->valor);
		vector_a->caminante = vector_a->caminante->siguiente;
		auxiliar->caminante = auxiliar->caminante->siguiente;
	}

	vector_a->caminante = vector_a->componente_inicial;
	auxiliar->caminante = auxiliar->componente_inicial;
	liberar_vector(auxiliar);
	
	return resultado;
}

Vector *resta(Vector *vector_a, Vector *vector_b)
{
	Vector *resultado = crear_vector(NULL);
	Vector *auxiliar = duplicar(vector_b);

	while (vector_a->caminante != NULL && auxiliar->caminante != NULL) {
		insertar_componente(resultado, vector_a->caminante->valor-auxiliar->caminante->valor);
		vector_a->caminante = vector_a->caminante->siguiente;
		auxiliar->caminante = auxiliar->caminante->siguiente;
	}

	vector_a->caminante = vector_a->componente_inicial;
	auxiliar->caminante = auxiliar->componente_inicial;
	liberar_vector(auxiliar);

	return resultado;
}

int punto(Vector *vector_a, Vector *vector_b)
{
	int resultado = 0;
	Vector *auxiliar = duplicar(vector_b);

	while (vector_a->caminante != NULL && auxiliar->caminante != NULL) {
		resultado += vector_a->caminante->valor * auxiliar->caminante->valor;
		vector_a->caminante = vector_a->caminante->siguiente;
		auxiliar->caminante = auxiliar->caminante->siguiente;
	}

	vector_a->caminante = vector_a->componente_inicial;
	auxiliar->caminante = auxiliar->componente_inicial;
	liberar_vector(auxiliar);

	return resultado;
}

Vector *cruz(Vector *vector_a, Vector *vector_b)
{
	Vector *resultado = crear_vector(NULL);
	Vector *auxiliar = duplicar(vector_b);
	int componentes_a[3], componentes_b[3], contador;

	for (contador = 0; contador < 3; contador++) {
		componentes_a[contador] = vector_a->caminante->valor;
		componentes_b[contador] = auxiliar->caminante->valor;
		vector_a->caminante = vector_a->caminante->siguiente;
		auxiliar->caminante = auxiliar->caminante->siguiente;
	}

	insertar_componente(resultado, componentes_a[1]*componentes_b[2]-componentes_a[2]*componentes_b[1]);
	insertar_componente(resultado, componentes_a[2]*componentes_b[0]-componentes_a[0]*componentes_b[2]);
	insertar_componente(resultado, componentes_a[0]*componentes_b[1]-componentes_a[1]*componentes_b[0]);

	vector_a->caminante = vector_a->componente_inicial;
	auxiliar->caminante = auxiliar->componente_inicial;
	liberar_vector(auxiliar);
	
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
