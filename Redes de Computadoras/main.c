#include <stdio.h>

int main()
{
    int opc, c;
    unsigned char buffer[100];

    do {
        printf("Ingresa tu nombre: ");
        fgets(buffer, sizeof(buffer), stdin);
        printf("Tu nombres es %s\n", buffer);
        printf("Otro? (s/n): ");
        opc = fgetc(stdin);
        fgetc(stdin);
    } while (opc == 's' || opc == 'S');

    return 0;
}

