#ifndef _PUERTOS_H_
#define _PUERTOS_H_

#include <stdbool.h>

void modificar_indicadores(int, int, int, int, int, int, unsigned char *);
void traducir_indicadores(unsigned char *, unsigned char *);
void encabezado_tcp(unsigned char *, int, int, unsigned int, unsigned int, unsigned char *, int, unsigned char *);
bool filtro_tcp(unsigned char *, unsigned char *);

#endif /* _PUERTOS_H_ */