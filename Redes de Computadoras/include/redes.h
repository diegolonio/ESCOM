#ifndef _REDES_H_
#define _REDES_H_

#include <stdbool.h>
#include <sys/types.h>

/* Datos de la interfaz de red */
typedef struct interfaz {
    int indice;
    unsigned char nombre[20];
    unsigned char direccion_mac[6];
    unsigned char direccion_ip[4];
    unsigned char mascara_subred[4];
} Interfaz;

void touch(char *);
long potencia(int, int);
int valor_numerico(char *);
void imprimir_ip(unsigned char *);
void imprimir_mac(unsigned char *);
void imprimir_trama(unsigned char *, int);
void imprimir_datos(Interfaz);
void limpiar_trama(unsigned char *);
bool ip_arreglo(unsigned char *, char *);
bool obtener_ip(unsigned char *, char *);
bool obtener_datos(int, char *, Interfaz *);
bool obtener_gateway(unsigned char *);
int obtener_puerto_efimero(void);
void aplicar_mascara(unsigned char *, unsigned char *, unsigned char *);
bool misma_subred(unsigned char *, unsigned char *);
void checksum(unsigned char *, int, unsigned char *);
void checksum_pseudo(unsigned char *, int, unsigned char *);
void encabezado_mac(unsigned char *, unsigned char *, unsigned char *, unsigned char *);
void mensaje_arp(unsigned char *, Interfaz, unsigned char *);
void encabezado_ip(unsigned char *, unsigned char *, unsigned char *, int, int, int);
bool filtro_arp(unsigned char *, unsigned char *);
bool enviar_trama(int, int, unsigned char *, int);
bool recibir_trama(int, unsigned char *, unsigned char *, bool (*)(unsigned char *, unsigned char *), long *, ssize_t *);

#endif /* _REDES_H_ */