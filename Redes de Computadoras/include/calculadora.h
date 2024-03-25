#ifndef _CALCULADORA_H_
#define _CALCULADORA_H_

#include <stdbool.h>

bool entero(int *);
int clase_teorica(unsigned char *);
int clase_practica(unsigned char *);
int clase_privada_practica(unsigned char *);
bool es_privada(unsigned char *);
int valor_octeto(int);
bool es_id_subred(unsigned char *);
void crear_ip(int, int, int, int, unsigned char *);
void copiar_ip(unsigned char *, unsigned char *);
void mascara_inversa(unsigned char *, unsigned char *);
void especificaciones(unsigned char *, int, int, int, int *, int *, unsigned char *, int *);
bool hosts(unsigned char *, int, int, int *, int *, unsigned char *, int *);
bool subredes(unsigned char *, int, int, int *, int *, unsigned char *, int *);
bool pre(unsigned char *, int, int, int *, int *, unsigned char *, int *);
void calcular_subredes(unsigned char *, unsigned char *);
void calcular_hosts(unsigned char *, unsigned char *);
void listar_subredes(int);
void listar_hosts(void);

#endif /* _CALCULADORA_H_ */
