#ifndef _CALENDAR_H_
#define _CALENDAR_H_

typedef struct Mes {
    char nombre[10];
    int dias;
} Mes;

typedef struct Fecha {
    int dia, mes, agno;
} Fecha;

int crear_fecha(Fecha *, int, int, int);
void mostrar_fecha(Fecha);
void siguiente_dia(Fecha *);
int numero_dias_mes(Fecha);
int es_fin_de_mes(Fecha);

#endif /* _CALENDAR_H_ */
