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

static struct {
    char *nombre;
    int valor;
} palabras_reservadas[] = {
    "imprimir", IMPRIMIR,
    "si", SI,
    "sino", SINO,
    "mientras", MIENTRAS,
    "para", PARA,
    0, 0
};

void inicializar()
{
    int contador;
    Simbolo *nuevo_simbolo = NULL;

    for (contador = 0; palabras_reservadas[contador].nombre; contador++)
        instalar(palabras_reservadas[contador].nombre, palabras_reservadas[contador].valor, NULL, 0);

    for (contador = 0; funciones_predefinidas[contador].nombre; contador++) {
        nuevo_simbolo = instalar(funciones_predefinidas[contador].nombre, FUNPREDEF, NULL, 0);
        nuevo_simbolo->u.apuntador = funciones_predefinidas[contador].funcion;
    }
}
