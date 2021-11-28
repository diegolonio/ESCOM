/* 
 * Nombre: Apolonio Villegas Diego Armando 
 * Práctica: 4
 * Tema: Máquina virtual de pila
 * Opción: Calculadora de vectores
 * Fecha 17/11/2021
 * Grupo: 3CM17
 * Materia: Compiladores
 */

#include "vectores.h"
#include "vectores.tab.h"

static struct {
    char *nombre;
    int (*funcion)(Vector *);
} funciones_predefinidas[] = {
    "norma", norma,
    "dimension", dimension,
    0, 0
};

void inicializar()
{
    int contador;
    Simbolo *nuevo_simbolo = NULL;

    for (contador = 0; funciones_predefinidas[contador].nombre; contador++) {
        nuevo_simbolo = instalar(funciones_predefinidas[contador].nombre, FUNPREDEF, NULL, 0);
        nuevo_simbolo->u.apuntador = funciones_predefinidas[contador].funcion;
    }
}
