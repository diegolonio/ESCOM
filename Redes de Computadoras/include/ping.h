#ifndef _PING_H_
#define _PING_H_

#include <stdbool.h>

void swap(int *, int *);
void selection_sort(int *, int);
void encabezado_icmp(unsigned char *, unsigned char *, int, unsigned char *);
bool filtro_icmp(unsigned char *, unsigned char *);

#endif /* _PING_H_ */