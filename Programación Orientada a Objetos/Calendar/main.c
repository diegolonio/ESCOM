#include <stdio.h>
#include "calendar.h"

int main ()
{
    Fecha mi_fecha;
    
    if (crear_fecha(&mi_fecha, 30, 1, 2020)) {
        mostrar_fecha(mi_fecha);
        siguiente_dia(&mi_fecha);
        mostrar_fecha(mi_fecha);

        printf ("Dias del mes %02d: %d\n", mi_fecha.mes, numero_dias_mes(mi_fecha));

        if (es_fin_de_mes(mi_fecha)) {
            printf ("Es fin de mes.\n");
        }
    } else {
        printf ("Algo salio mal, por favor intentelo de nuevo.\n");
    }
    
    return 0;
}