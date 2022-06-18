#include "usb.h"

int main ()
{
    USB mi_usb = crear_usb("APOLONIO", 32768, 15879, "NTFS");

    mostrar_propiedades(mi_usb);

    return 0;
}
