#include <stdio.h>
#include <string.h>
#include "../include/udp.h"
#include "../include/redes.h"

/**
 * encabezado_udp - estructurar el encabezado de una trama de UDP
 * 
 * @trama: La trama con la que se va a trabajar
 * @origen: El puerto origen
 * @destino: El puerto destino
 * @longitud: La longitud del datagrama UDP
 * @datos: Los datos que contendrá el datagrama UDP
 */
void encabezado_udp(unsigned char *trama, int origen, int destino, int longitud, unsigned char *datos)
{
    unsigned char puerto_origen[2] = {origen/0x100, origen-origen/0x100*0x100};
    unsigned char puerto_destino[2] = {destino/0x100, destino-destino/0x100*0x100};
    unsigned char tamanio[2] = {longitud/0x100, longitud-longitud/0x100*0x100};
    unsigned char checksum[2] = {0x00, 0x00};

    /* Número de puerto origen */
    memcpy(trama+34, puerto_origen, 2);
    /* Número de puerto destino */
    memcpy(trama+36, puerto_destino, 2);
    /* Longitud de la trama de UDP */
    memcpy(trama+38, tamanio, 2);
    /* Suma de comprobación inicial */
    memcpy(trama+40, checksum, 2);
    /* Datos */
    memcpy(trama+42, datos, longitud);
    /* Cálculo de la suma de comprobación */
    checksum_pseudo(trama, longitud, checksum);
    /* Suma de comprobación */
    memcpy(trama+40, checksum, 2);
}

/**
 * filtro_udp - determinar si una trama es un datagrama UDP
 * 
 * @trama_recibida: La trama recibida del socket
 * @trama_enviada: La trama con el datagrama UDP enviado
 * 
 * La función utiliza distintos filtros para determinar si
 * @trama_recibida es respuesta de @trama_enviada. Si es así, la
 * función retorna true; en caso contrario, retorna false.
 */
bool filtro_udp(unsigned char *trama_recibida, unsigned char *trama_enviada)
{
    unsigned char ethertype[2] = {0x08, 0x00};
    unsigned char suma_ip[2], suma_udp[2];

    /* Cálculo de la suma de comprobación del encabezado IP */
    memcpy(suma_ip, trama_recibida+24, 2);
    (trama_recibida+24)[0] = 0x00;
    (trama_recibida+24)[1] = 0x00;
    checksum(trama_recibida+14, 20, trama_recibida+24);

    /* Cálculo de la suma de comprobación de la trama UDP */
    memcpy(suma_udp, trama_recibida+40, 2);
    (trama_recibida+40)[0] = 0x00;
    (trama_recibida+40)[1] = 0x00;
    checksum_pseudo(trama_recibida, (trama_recibida+38)[0]*0x100+(trama_recibida+38)[1], trama_recibida+40);

    /* La dirección MAC destino es la mía */
    if (memcmp(trama_recibida+0, trama_enviada+6, 6) == 0) {
        /* El ethertype es 0x0800 (IP) */
        if (memcmp(trama_recibida+12, ethertype, 2) == 0) {
            /* El protocolo al que se le está dando servicio es 0x11 (UDP) */
            if (memcmp(trama_recibida+23, trama_enviada+23, 1) == 0) {
                /* La suma de comprobación del encabezado IP es correcta */
                if (memcmp(trama_recibida+24, suma_ip, 2) == 0) {
                    /* La dirección IP del destino de la respuesta es la mía */
                    if (memcmp(trama_recibida+30, trama_enviada+26, 4) == 0) {
                        /* El puerto origen de la respuesta es el puerto destino de la solicitud */
                        if (memcmp(trama_recibida+34, trama_enviada+36, 2) == 0) {
                            /* El puerto destino de la respuesta es el puerto origen de la solicitud */
                            if (memcmp(trama_recibida+36, trama_enviada+34, 2) == 0) {
                                /* La suma de comprobación de la trama UDP es correcta */
                                if (memcmp(trama_recibida+40, suma_udp, 2) == 0) {
                                    return true;
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
