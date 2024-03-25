#ifndef _LLC_H_
#define _LLC_H_

/* Estructura para almacenar las tramas LLC y su longitud */
typedef struct tramas {
    unsigned char tramas[45][1514];
    int longitud_tramas[45];
} Tramas;

Tramas obtener_tramas(char *);
void analizar_encabezado_mac(unsigned char *);
void sap(unsigned char *);
void dsap(unsigned char *);
void ssap(unsigned char *);
int tipo_trama(unsigned char *);
void poll_final(unsigned char *);
void ss(unsigned char *);
void trama_informacion(unsigned char *);
void trama_supervision(unsigned char *);
void trama_nn(unsigned char *);
void analizar_encabezado_llc(unsigned char *);

#endif /* _LLC_H_ */
