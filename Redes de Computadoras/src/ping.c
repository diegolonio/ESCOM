#include <stdlib.h>
#include <string.h>
#include "../include/ping.h"
#include "../include/redes.h"

/**
 * swap - intercambiar los valores de dos variables
 * 
 * @a: El elemento de un arreglo que contiene el valor entero A
 * @b: El elemento de un arreglo que contiene el valor entero B
 */
void swap(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

/**
 * selection_sort - ordenar un arreglo de enteros con el algoritmo
 * de selección
 * 
 * @arr: El arreglo de enteros
 * @n: La cantidad de elementos del arreglo
 */
void selection_sort(int *arr, int n) {
    int i, j, minimo;
 
    for (i = 0; i < n-1; i++) {
        minimo = i;

        for (j = i + 1; j < n; j++) {
            if (arr[j] < arr[minimo]) {
                minimo = j;
            }
        }

        swap(&arr[minimo], &arr[i]);
    }
}

/**
 * encabezado_icmp - estructurar una solicitud ICMP
 * 
 * @trama: La trama que se va a enviar
 * @datos: Los datos que contendrá la solicitud
 * @longitud: La longitud del encabezado ICMP con los datos
 * @identificador: El identificador del proceso actual
 */
void encabezado_icmp(unsigned char *trama, unsigned char *datos, int longitud, unsigned char *identificador)
{
    unsigned char tipo[1] = {0x08}; // Solicitud
    unsigned char codigo[1] = {0x00};
    unsigned char suma[2] = {0x00, 0x00};
    unsigned char secuencia[2] = {(int)random(), (int)random()};

    /* Tipo: solicitud de echo */
    memcpy(trama+34, tipo, 1);
    /* Código */
    memcpy(trama+35, codigo, 1);
    /* Suma de comprobación inicial */
    memcpy(trama+36, suma, 2);
    /* Identificador */
    memcpy(trama+38, identificador, 2);
    /* Número de secuencia */
    memcpy(trama+40, secuencia, 2);
    /* Datos del mensaje ICMP */
    memcpy(trama+42, datos, 32);
    /* Se hace la suma de comprobación */
    checksum(trama+34, longitud, suma);
    /* Suma de comprobación */
    memcpy(trama+36, suma, 2);
}

/**
 * filtro_icmp - determinar si una trama es una respuesta de ICMP
 * 
 * @trama_recibida: La trama que se recibió
 * @trama_enviada: La trama que se envió
 * 
 * La función compara la siguiente lista de datos contenidos en ambas
 * tramas para determinar si se trata de una solicitud y su respuesta:
 * 
 *  - Dirección MAC de la fuente
 *  - Ethertype = 0x0800 (IP)
 *  - Protocolo 0x01 (ICMP)
 *  - Suma de comprobación del protocolo IP
 *  - Dirección IP de la fuente
 *  - Tipo = 0 (respuesta ICMP)
 *  - Suma de comprobación del protocolo ICMP
 *  - Identificador
 *  - Secuencia
 * 
 * Si los datos coinciden, retorna true; en caso contrario, retorna false.
 */
bool filtro_icmp(unsigned char *trama_recibida, unsigned char *trama_enviada)
{
    unsigned char ethertype[2] = {0x08, 0x00};
    unsigned char tiempo_excedido[1] = {0x0b};
    unsigned char respuesta[1] = {0x00};
    unsigned char suma_ip[2], suma_icmp[2];
    int longitud_icmp = (trama_recibida+16)[0] * 100 + (trama_recibida+17)[0] - 20;

    /* Cálculo de la suma de comprobación del encabezado IP */
    memcpy(suma_ip, trama_recibida+24, 2);
    (trama_recibida+24)[0] = 0x00;
    (trama_recibida+24)[1] = 0x00;
    checksum(trama_recibida+14, 20, trama_recibida+24);

    /* Cálculo de la suma de comprobación del encabezado ICMP */
    memcpy(suma_icmp, trama_recibida+36, 2);
    (trama_recibida+36)[0] = 0x00;
    (trama_recibida+36)[1] = 0x00;
    checksum(trama_recibida+34, longitud_icmp, trama_recibida+36);

    /* La dirección MAC destino es la mía */
    if (memcmp(trama_recibida+0, trama_enviada+6, 6) == 0) {
        /* El ethertype es 0x0800 (IP) */
        if (memcmp(trama_recibida+12, ethertype, 2) == 0) {
            /* El protocolo al que se le está dando servicio es 0x01 (ICMP) */
            if (memcmp(trama_recibida+23, trama_enviada+23, 1) == 0) {
                /* La suma de comprobación del encabezado IP es correcta */
                if (memcmp(trama_recibida+24, suma_ip, 2) == 0) {
                    /* La dirección IP del destino de la respuesta es la mía */
                    if (memcmp(trama_recibida+30, trama_enviada+26, 4) == 0) {
                        /* La trama es una respuesta o un mensaje de TTL excedido de ICMP*/
                        if (memcmp(trama_recibida+34, tiempo_excedido, 1) == 0 || memcmp(trama_recibida+34, respuesta, 1) == 0) {
                            /* La suma de comprobación del encabezado ICMP */
                            if (memcmp(trama_recibida+36, suma_icmp, 2) == 0) {
                                /* El identificador es el mismo */
                                if (memcmp(trama_recibida+66, trama_enviada+38, 2) == 0 || memcmp(trama_recibida+38, trama_enviada+38, 2) == 0) {
                                    /* El número de secuencia es el mismo */
                                    if (memcmp(trama_recibida+68, trama_enviada+40, 2) == 0 || memcmp(trama_recibida+40, trama_enviada+40, 2) == 0) {
                                        return true;
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }

    return false; 
}
