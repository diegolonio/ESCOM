#include <stdio.h>
#include "calendar.h"

int crear_fecha(Fecha *fecha, int dia, int mes, int agno)
{
    int dias_meses[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (dia < 1 || dia > 31) {
        return 0;
    }

    if (dia > dias_meses[mes-1]) {
        return 0;
    }

    if (mes < 0 || mes > 12) {
        return 0;
    }
        
    fecha->dia = dia;
    fecha->mes = mes;
    fecha->agno = agno;

    return 1;
}

void mostrar_fecha(Fecha fecha)
{
    char meses[12][10] = {"enero", "febrero", "marzo", "abril", "mayo", "junio", "julio", "agosto", "septiembre", "octubre", "noviembre", "diciembre"};
    printf("%02d de %s del %d\n", fecha.dia, meses[fecha.mes-1], fecha.agno);
}

void siguiente_dia(Fecha *fecha)
{
    int dias_meses[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (fecha->dia == dias_meses[fecha->mes-1]) {
        fecha->dia = 1;
        fecha->mes++;
    } else {
        fecha->dia++;
    }

    if (fecha->mes > 12) {
            fecha->mes = fecha->mes % 12;
            fecha->agno++;
    }
}

int numero_dias_mes(Fecha fecha)
{
    int dias_meses[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    return dias_meses[fecha.mes-1];
}

int es_fin_de_mes(Fecha fecha)
{
    int dias_meses[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};

    if (fecha.dia != dias_meses[fecha.mes-1]) {
        return 0;
    }

    return 1;
}
