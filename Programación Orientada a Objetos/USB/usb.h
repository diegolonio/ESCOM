#ifndef _USB_H_
#define _USB_H_

typedef struct Memoria {
    char nombre[22];
    int capacidad;
    int almacenamiento_usado;
    int almacenamiento_libre;
    char sistema_de_archivos[6];
} USB;

USB crear_usb(char [], int, int, char []);
void formatear(USB *, char [], char []);
void mostrar_propiedades (USB);
void cambiar_nombre (USB *, char []);

#endif /* _USB_H_ */
