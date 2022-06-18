#include <stdio.h>
#include <string.h>
#include "usb.h"

USB crear_usb(char nombre[], int capacidad, int almacenamiento_usado, char sistema_de_archivos[])
{   
    USB nueva_usb;

    strcpy(nueva_usb.nombre, nombre);
    nueva_usb.capacidad = capacidad;
    nueva_usb.almacenamiento_usado = almacenamiento_usado;
    nueva_usb.almacenamiento_libre = nueva_usb.capacidad - nueva_usb.almacenamiento_usado;
    strcpy(nueva_usb.sistema_de_archivos, sistema_de_archivos);

    return nueva_usb;
}

void formatear(USB *usb, char nuevo_sistema_de_archivos[], char nuevo_nombre[])
{
    usb->almacenamiento_libre = usb->capacidad;
    usb->almacenamiento_usado = 0;
    strcpy(usb->nombre, nuevo_nombre);
    strcpy(usb->sistema_de_archivos, nuevo_sistema_de_archivos);
}

void mostrar_propiedades(USB usb)
{
    printf("Nombre: %s\n", usb.nombre);
    printf("Capacidad: %d\n", usb.capacidad);
    printf("Almacenamiento libre: %d\n", usb.almacenamiento_libre);
    printf("Almacenamiento usado: %d\n", usb.almacenamiento_usado);
    printf("Sistema de archivos: %s\n", usb.sistema_de_archivos);
}

void cambiar_nombre(USB *usb, char nuevo_nombre[])
{
    strcpy(usb->nombre, nuevo_nombre);
}
