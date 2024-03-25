#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "../include/llc.h"
#include "../include/redes.h"

/**
 * obtener_tramas - obtener las tramas LLC de un archivo
 * 
 * @nombre: El nombre del archivo que contiene las tramas
 * 
 * Se obtienen las tramas LLC del archivo con nombre @nombre y son
 * almacendas en una estructura Tramas junto con su longitud.
 */
Tramas obtener_tramas(char *nombre)
{
    Tramas tramas;
    int i = 0, j = 0, k = 0, m = 0;
    char linea[500], numero[3];
    FILE *archivo = NULL;

    /**
     * Borrado de los datos contenidos en el espacio de memoria donde
     * se encuentra @trama
     */
    memset(&tramas, 0x00, sizeof(tramas));

    /* Apertura del archivo con las tramas */
    archivo = fopen(nombre, "r");

    /* Obtención de cada una de las líneas del archivo */
    while (fgets(linea, sizeof(linea), archivo) != NULL) {
        /* Para asegurar que los caracteres de la línea son solo de trama */
        linea[strlen(linea)-2] = '\0';

        /* Guardado de la longitud de la trama actual */
        tramas.longitud_tramas[m] = strlen(linea) / 2;

        for (i = 0; i < strlen(linea); i++) {
            if (i == 0) {
                k = 0; // Primera posición de la trama actual
            }

            /**
             * @numero permite almacenar cada número hexadecimal de
             * la trama como una cadena
             */
            if (j < 2) {
                numero[j] = linea[i];
                j = j + 1;
            }

            if (j == 2) {
                /**
                 * Se agrega el caracter nulo para completar la cadena
                 * del digito en hexadecimal
                 */
                numero[j] = '\0';

                /**
                 * Se obtiene el valor numérico en decimal de la cadena
                 * del número en hexadecimal y es almacenado en la
                 * k-ésima posición de la m-ésima trama
                 */
                tramas.tramas[m][k] = (int)strtol(numero, NULL, 16);

                /**
                 * Reinicio de la j-ésima posición para poder obtener
                 * el siguiente número en hexadecimal de la trama 
                 */
                j = 0;
                k = k + 1; // Siguiente posición de la trama actual
            }
        }

        m = m + 1; // Siguiente trama
    }

    fclose(archivo); // Cerrado del archivo con las tramas

    return tramas;
}

/**
 * analizar_encabezado_mac - presentar el encabezado MAC de una trama
 * 
 * @trama: La trama LLC que se va a analizar
 */
void analizar_encabezado_mac(unsigned char *trama)
{
    int i;

    printf("Encabezado MAC\n");
    printf("  Direccion destino: ");

    for (i = 0; i < 6; i++) {
        printf("%.2x", trama[i]);

        if (i < 5) {
            printf(":");
        }
    }

    printf("\n  Direccion fuente: ");

    for (i = 0; i < 6; i++) {
        printf("%.2x", (trama+6)[i]);

        if (i < 5) {
            printf(":");
        }
    }

    printf("\n  Longitud: 0x");

    for (i = 0; i < 2; i++) {
        printf("%.2x", (trama+12)[i]);
    }

    printf("\n");
}

/**
 * sap - obtener el SAP de la trama
 * 
 * @bit: El bit que se va a analizar 
 */
void sap(unsigned char *bit)
{
    switch (bit[0] & 0xfe) {
        case 0x00: printf("Null LSAP\n"); break;
        case 0x02: printf("Individual LLC Sublayer Management Function\n"); break;
        case 0x03: printf("Group LLC Sublayer Management Function\n"); break;
        case 0x04: printf("IBM SNA Path Control (individual)\n"); break;
        case 0x05: printf("IBM SNA Path Control (group)\n"); break;
        case 0x06: printf("ARPANET Internet Protocol (IP)\n"); break;
        case 0x08: printf("SNA\n"); break;
        case 0x0c: printf("SNA\n"); break;
        case 0x0e: printf("PROWAY (IEC955) Network Management & Initialization\n"); break;
        case 0x18: printf("Texas Instruments\n"); break;
        case 0x42: printf("IEEE 802.1 Bridge Spanning Tree Protocol\n"); break;
        case 0x4e: printf("EIA RS-511 Manufacturing Message Service\n"); break;
        case 0x7e: printf("ISO 8208 (X.25 over IEEE 802.2 Type 2 LLC)\n"); break;
        case 0x80: printf("Xerox Network Systems (XNS)\n"); break;
        case 0x86: printf("Nestar\n"); break;
        case 0x8e: printf("PROWAY (IEC 955) Active Station List Maintenance\n"); break;
        case 0x98: printf("ARPANET Address Resolution Protocol (ARP)\n"); break;
        case 0xbc: printf("Banyan VINES\n"); break;
        case 0xaa: printf("SubNetwork Access Protocol (SNAP)\n"); break;
        case 0xe0: printf("Novell NetWare\n"); break;
        case 0xf0: printf("IBM NetBIOS\n"); break;
        case 0xf4: printf("IBM LAN Management (individual)\n"); break;
        case 0xf5: printf("IBM LAN Management (group)\n"); break;
        case 0xf8: printf("IBM Remote Program Load (RPL)\n"); break;
        case 0xfa: printf("Ungermann-Bass\n"); break;
        case 0xfe: printf("ISO Network Layer Protocol\n"); break;
        case 0xff: printf("Global LSAP\n"); break;
        default: printf("Who knows?\n"); break;
    }
}

/**
 * dsap - analizar el byte DSAP de una trama LLC
 * 
 * @trama: La trama LLC que se va a analizar
 */
void dsap(unsigned char *trama)
{
    sap(trama+14);

    switch ((trama+14)[0] & 0x01) {
        case 0:
            printf("    Individual\n");
            break;
        case 1:
            printf("    Grupo\n");
            break;
        default:
            printf("Who knows?");
            break;
    } 
}

/**
 * ssap - analizar el byte SSAP de una trama LLC
 * 
 * @trama: La trama LLC que se va a analizar
 */
void ssap(unsigned char *trama)
{
    sap(trama+15);

    switch ((trama+15)[0] & 0x01) {
        case 0:
            printf("    Comando\n");
            break;
        case 1:
            printf("    Respuesta\n");
            break;
        default:
            printf("Who knows?");
            break;
    } 
}

/**
 * tipo_trama - obtener el tipo de una trama LLC
 * 
 * @trama: La trama LLC que se va a analizar
 * 
 * La función retorna los siguientes valores:
 *  1 - si la trama es de información
 *  2 - si la trama es de supervisión
 *  3 - si la trama es no numerada
 * En caso de que la trama no coincida con ninguno de los valores
 * anteriores, retorna 0.
 */
int tipo_trama(unsigned char *trama)
{
    if (((trama+16)[0] & 0x01) == 0) {
        return 1; // Trama de información
    } else if (((trama+16)[0] & 0x03) == 1) {
        return 2; // Trama de supervisión
    } else if (((trama+16)[0] & 0x03) == 3) {
        return 3; // Trama no numerada
    }

    return 0;
}

/**
 * poll_final - analizar el bit Poll/Final de una trama LLC de
 * información o de supervisión
 * 
 * @trama: La trama LLC que se va a analizar
 */
void poll_final(unsigned char *trama)
{
    printf("    P/F = ");

    if (((trama+17)[0] & 0x01) == 0) { // No tiene significado
        printf("0\n");
    } else {
        if (((trama+15)[0] & 0x01) == 0) { // Es Comando
            printf("1 - Poll\n");
        } else { // Es Respuesta
            printf("1 - Final\n");
        }
    }
}

/**
 * ss - saber la instrucción SS de una trama LLC de supervisión
 * 
 * @trama: La trama LLC que se va a analizar
 */
void ss(unsigned char *trama)
{
    switch (((trama+16)[0] & 0x0c) >> 2) {
        case 0:
            printf("RR");
            break;
        case 1:
            printf("RNR");
            break;
        case 2:
            printf("REJ");
            break;
        case 3:
            printf("SREJ");
            break;
        default:
            printf("Who knows?");
            break;
    }    
}

/**
 * trama_informacion - analizar los bytes de control de una trama LLC
 * de información
 * 
 * @trama: La trama LLC que se va a analizar 
 */
void trama_informacion(unsigned char *trama)
{
    poll_final(trama);
    printf("    N(S),N(R) = %d,%d\n", (trama+16)[0] >> 1, (trama+17)[0] >> 1);
}

/**
 * trama_supervision - analizar los bytes de control de una trama LLC
 * de supervisión
 * 
 * @trama: La trama que se va a analizar
 */
void trama_supervision(unsigned char *trama)
{
    poll_final(trama);
    printf("    SS,N(R) = ");
    ss(trama);
    printf(",%d\n", (trama+17)[0] >> 1);
}

/**
 * trama_nn - analizar el byte de control de una trama LLC no numerada
 * 
 * @trama: La trama LLC que se va a analizar
 */
void trama_nn(unsigned char *trama)
{
    unsigned char MMM_MM;

    /**
     * Obtención de los bits del código de la trama no numerada en la
     * forma 000MMMMM
     */
    MMM_MM = (((((trama+16)[0] & 0xec) >> 2) & 0x38) >> 1) | ((((trama+16)[0] & 0xec) >> 2) & 0x03);

    if ((((trama+16)[0] & 0x10) >> 4) == 0) {
        printf("    No se requiere respuesta inmediata\n");
    }

    if (((trama+15)[0] & 0x01) == 0) { // si es Comando
        switch (MMM_MM) {
            case 0b10000:
                printf("    Set normal response (SNRM)\n");
                break;
            case 0b11011:
                printf("    Set normal response extended mode (SNRME)\n");
                break;
            case 0b00011:
                printf("    Set asynchronous response (SARM)\n");
                break;
            case 0b01011:
                printf("    Set asynchronous response extended mode (SARME)\n");
                break;
            case 0b00111:
                printf("    Set asynchronous balanced mode (SABM)\n");
                break;
            case 0b01111:
                printf("    Set asynchronous balanced extended mode (SABME)\n");
                break;
            case 0b00001:
                printf("    Set initialization mode (SIM)\n");
                break;
            case 0b01000:
                printf("    Disconnect (DISC)\n");
                break;
            case 0b00000:
                printf("    Unnumbered Information (UI)\n");
                break;
            case 0b00100:
                printf("    Unnumbered Poll (UP)\n");
                break;
            case 0b10011:
                printf("    Reset (RSET)\n");
                break;
            case 0b10111:
                printf("    Exchange Identification (XID)\n");
                break;
            case 0b11100:
                printf("    Test (TEST)\n");
                break;
            default:
                printf("    Who knows?\n");
                break;   
        }
    } else { // si es Respuesta
        switch (MMM_MM) {
            case 0b01100:
                printf("    Unnumbered Acknowledgment (UA)\n");
                break;
            case 0b00011:
                printf("    Disconnect Mode (DM)\n");
                break;
            case 0b01000:
                printf("    Request Disconnect (RD)\n");
                break;
            case 0b00001:
                printf("    Request Initialization Mode (RIM)\n");
                break;
            case 0b00000:
                printf("    Unnumbered Information (UI)\n");
                break;
            case 0b10111:
                printf("    Exchange Identification (XID)\n");
                break;
            case 0b11100:
                printf("    Test (TEST)\n");
                break;
            default:
                printf("    Who knows?\n");
                break;
        }
    }
}

/**
 * analizar_encabezado_llc - analizar el encabezado LLC de una trama LLC
 * 
 * @trama: La trama LLC que se va a analizar
 * 
 * La función analiza el encabezado LLC de una trama LLC para obtener
 * los siguientes datos:
 *   DSAP - Individual/Grupal
 *   SSAP - Comando/Respuesta
 *   Tipo de trama
 *     Trama de información - P/F y N(S),N(R)
 *     Trama de supervisión - SS,N(R)
 *     Trama no numerada - código y descripción
 */
void analizar_encabezado_llc(unsigned char *trama)
{
    printf("Encabezado LLC\n");

    printf("  DSAP: ");
    dsap(trama);

    printf("  SSAP: ");
    ssap(trama);

    switch (tipo_trama(trama)) {
        case 1:
            printf("  Trama de información\n");
            trama_informacion(trama);
            break;
        case 2:
            printf("  Trama de supervisión\n");
            trama_supervision(trama);
            break;
        case 3:
            printf("  Trama no numerada\n");
            trama_nn(trama);
            break;
        default:
            printf("  Who knows?");
            break;
    }
}

