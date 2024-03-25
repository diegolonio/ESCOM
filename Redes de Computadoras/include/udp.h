#ifndef _UDP_H_
#define _UDP_H_

#include <stdbool.h>

void encabezado_udp(unsigned char *, int, int, int, unsigned char *);
bool filtro_udp(unsigned char *, unsigned char *);

#endif /* _UDP_H_ */