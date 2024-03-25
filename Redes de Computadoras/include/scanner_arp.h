#ifndef _SCANNER_ARP_H_
#define _SCANNER_ARP_H_

#include <sqlite3.h>

int gestionar_error(sqlite3 *);
int mostrar_db(void *, int, char **, char **);
void ip_cadena(unsigned char *, char *);
void mac_cadena(unsigned char *, char *);

#endif /* _SCANNER_ARP_H_ */