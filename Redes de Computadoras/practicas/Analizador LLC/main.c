#include <stdio.h>
#include "../../include/llc.h"
#include "../../include/redes.h"

int main()
{
    Tramas tramas;
    int m;

    /* Si el archivo no existe, es creado para evitar errores */
    touch("tramas.txt");

    /* Obtención de las tramas del archivo .txt */
    tramas = obtener_tramas("tramas.txt");

    /* Análisis de cada una de las tramas */
    for (m = 0; m < 45; m++) {
        printf("******************** Trama ********************\n");
        imprimir_trama(tramas.tramas[m], tramas.longitud_tramas[m]);
        printf("\n******************* Análisis ******************\n");

        /* Análisis del encabezado MAC de la trama actual */
        analizar_encabezado_mac(tramas.tramas[m]);
        /* Análisis del encabezado LLC de la trama actual */
        analizar_encabezado_llc(tramas.tramas[m]);

        if (m < 44) {
            printf("-----------------------------------------------\n");
        }
    }

    return 0;
}
