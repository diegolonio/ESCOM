#ifndef _TRACERT_H_
#define _TRACERT_H_

#include <stdbool.h>

void encabezado_icmp(unsigned char *, unsigned char *, int, unsigned char *);
bool filtro_icmp(unsigned char *, unsigned char *);

#endif /* _TRACERT_H_ */