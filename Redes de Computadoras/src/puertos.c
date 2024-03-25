#include <string.h>
#include "../include/puertos.h"
#include "../include/redes.h"

/**
 * modificar_indicadores - modificar los indicadores de una trama TCP
 * 
 * @urg: Valor del indicador Urgent (URG)
 * @ack: Valor del indicador Acknowledge (ACK)
 * @psh: Valor del indicador Push (PSH)
 * @rst: Valor del indicador Reset (RST)
 * @syn: Valor del indicador Syncronous (SYN)
 * @fin: Valor del indicador Final (FIN)
 * @indicadores: El arreglo de indicadores que se quiere modificar
 * 
 * La función modifica los valores de @indicadores; que es un arreglo
 * que almacena los valores de los indicadores de una trama TCP.
 */
void modificar_indicadores(int urg, int ack, int psh, int rst, int syn, int fin, unsigned char *indicadores)
{
    int i;

    for (i = 0; i < 6; i++) {
        indicadores[0] = 0x00;
    }

    indicadores[0] = urg;
    indicadores[1] = ack;
    indicadores[2] = psh;
    indicadores[3] = rst;
    indicadores[4] = syn;
    indicadores[5] = fin;
}

/**
 * traducir_indicadores - obtener el byte de indicadores de TCP
 * 
 * @indicadores: El arreglo que contiene los valores de los indicadores
 * @resultado: La variable donde se almacenará el byte resultante
 * 
 * La función convierte los valores almacenados en @indicadores de los
 * indicadores que se quieren configurar a una trama TCP en el byte
 * que ha de ser incluido en la trama. Así, para los valores:
 * 
 *      URG = 0; ACK = 1; PSH = 0; RST = 1; SYN = 0; FIN = 0
 * 
 * Almacenados en @indicadores como:
 * 
 *                      [0, 1, 0, 1, 0, 0]
 * 
 * Se genera como resultado el byte:
 * 
 *                          00010100
 */
void traducir_indicadores(unsigned char *indicadores, unsigned char *resultado)
{
    int i;
    
    for (i = 0; i < 6; i++) {
        if (indicadores[i] == 1) {
            resultado[0] = resultado[0] | (unsigned char)potencia(2, 5-i);
        }
    }
}

/**
 * encabezado_tcp - estructurar el encabezado de una trama TCP
 * 
 * @trama: La trama en la cual se incluirá el encabezado TCP
 * @origen: El puerto origen
 * @destino: El puerto destino
 * @sec: El número de secuencia que ha de incluir la trama
 * @rec: El número de reconocimiento que ha de incluir la trama
 * @int: La longitud del encabezado TCP más la longitud de los datos
 * @datos: Los datos que se enviarán a través del protocolo TCP
 */
void encabezado_tcp(unsigned char *trama, int origen, int destino, unsigned int sec, unsigned int rec, unsigned char *indc, int longitud, unsigned char *datos)
{
    unsigned char puerto_origen[2] = {origen/0x100, origen-origen/0x100*0x100};
    unsigned char puerto_destino[2] = {destino/0x100, destino-destino/0x100*0x100};
    unsigned char secuencia[4] = {sec/0x1000000, sec/0x10000-sec/0x1000000*0x100, sec/0x100-sec/0x10000*0x100, sec-sec/0x100*0x100};
    unsigned char reconocimiento[4] = {rec/0x1000000, rec/0x10000-rec/0x1000000*0x100, rec/0x100-rec/0x10000*0x100, rec-rec/0x100*0x100};
    unsigned char tamanio[1] = {0x50}; // El encabezado TCP mide 20 bytes
    unsigned char indicadores[1] = {0x00};
    unsigned char ventana[2] = {0x01, 0x00}; // Ventana de 256 bytes de datos
    unsigned char checksum[2] = {0x00, 0x00};
    unsigned char puntero_urgente[2] = {0x00, 0x00};

    /* Conversión de las banderas activas de @indc en un número */
    traducir_indicadores(indc, indicadores);

    /* Número de puerto origen */
    memcpy(trama+34, puerto_origen, 2);
    /* Número de puerto destino */
    memcpy(trama+36, puerto_destino, 2);
    /* Número de secuencia */
    memcpy(trama+38, secuencia, 4);
    /* Número de reconocimiento */
    memcpy(trama+42, reconocimiento, 4);
    /* Longitud del encabezado de TCP */
    memcpy(trama+46, tamanio, 1);
    /* Indicadores */
    memcpy(trama+47, indicadores, 1);
    /* Ventana */
    memcpy(trama+48, ventana, 2);
    /* Suma de comprobación inicial */
    memcpy(trama+50, checksum, 2);
    /* Puntero de urgente */
    memcpy(trama+52, puntero_urgente, 2);
    /* Datos */
    if (datos != NULL) {
        memcpy(trama+54, datos, longitud-20);
    }
    /* Cálculo de la suma de comprobación */
    checksum_pseudo(trama, longitud, checksum);
    /* Suma de comprobación */
    memcpy(trama+50, checksum, 2);
}

/**
 * filtro_tcp - determinar si una trama es una respuesta de TCP
 * 
 * @trama_recibida: La trama recibida del socket
 * @trama_enviada: La trama enviada de la solicitud de TCP
 * 
 * La función utiliza distintos filtros para determinar si
 * @trama_recibida es respuesta de @trama_enviada. Si es así, la
 * función retorna true; en caso contrario, retorna false.
 */
bool filtro_tcp(unsigned char *trama_recibida, unsigned char *trama_enviada)
{
    unsigned char ethertype[2] = {0x08, 0x00};
    unsigned char suma_ip[2], suma_tcp[2];
    int longitud_tcp = (trama_recibida + 16)[0] * 0x100 + (trama_recibida + 16)[1] - ((trama_recibida+14)[0] & 0x0f) * 4;

    /* Cálculo de la suma de comprobación del encabezado IP */
    memcpy(suma_ip, trama_recibida+24, 2);
    (trama_recibida+24)[0] = 0x00;
    (trama_recibida+24)[1] = 0x00;
    checksum(trama_recibida+14, 20, trama_recibida+24);

    // /* Cálculo de la suma de comprobación de la trama TCP */
    memcpy(suma_tcp, trama_recibida+50, 2);
    (trama_recibida+50)[0] = 0x00;
    (trama_recibida+50)[1] = 0x00;
    checksum_pseudo(trama_recibida, longitud_tcp, trama_recibida+50);

    /* La dirección MAC destino es la mía */
    if (memcmp(trama_recibida+0, trama_enviada+6, 6) == 0) {
        /* El ethertype es 0x0800 (IP) */
        if (memcmp(trama_recibida+12, ethertype, 2) == 0) {
            /* El protocolo al que se le está dando servicio es 0x06 (TCP) */
            if (memcmp(trama_recibida+23, trama_enviada+23, 1) == 0) {
                /* La suma de comprobación del encabezado IP es correcta */
                if (memcmp(trama_recibida+24, suma_ip, 2) == 0) {
                    /* La dirección IP del destino de la respuesta es la mía */
                    if (memcmp(trama_recibida+30, trama_enviada+26, 4) == 0) {
                        /* El puerto origen de la respuesta es el puerto destino de la solicitud */
                        if (memcmp(trama_recibida+34, trama_enviada+36, 2) == 0) {
                            /* El puerto destino de la respuesta es el puerto origen de la solicitud */
                            if (memcmp(trama_recibida+36, trama_enviada+34, 2) == 0) {
                                /* La suma de comprobación de la trama TCP es correcta */
                                if (memcmp(trama_recibida+50, suma_tcp, 2) == 0) {
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
