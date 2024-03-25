#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "../include/calculadora.h"
#include "../include/redes.h"

/**
 * entero - recibir un número entero de la entrada estándar
 * 
 * @variable: El apuntador a la variable donde se almacenará el
 * valor entero.
 * 
 * Si la entrada se compone de puros dígitos, la función retorna
 * true; en caso contrario, retorna false.
 */
bool entero(int *variable)
{
    bool es_negativo = true;
    int caracter, i = 0;
    char numero[10];

    caracter = fgetc(stdin);

    if (caracter != '-') {
        ungetc(caracter, stdin);
        es_negativo = false;
    }

    while ((caracter = fgetc(stdin)) != '\n') {
        if (isdigit(caracter) == 0 || i >= 10) {
            fprintf(stderr, "%s", "El dato ingresado no es valido\n");
            ungetc(caracter, stdin);
            return false;
        }

        numero[i] = caracter;
        i = i + 1;
    }

    ungetc(caracter, stdin);
    numero[i] = '\0';
    *variable = valor_numerico(numero);

    if (es_negativo == true) {
        *variable = (*variable) * -1;
    }

    return true;
}

/**
 * clase_teorica - saber la clasificación de una dirección IP
 * 
 * @ip: La dirección IP de la cual se quiere obtener su clasificación
 * 
 * La función clasifica a @ip de acuerdo con la Classful Network
 * Architecture:
 * 
 *      1.0.0.0 - 127.0.0.0: Clase A
 *      128.0.0.0 - 191.0.0.0: Clase B
 *      192.0.0.0 - 223.0.0.0: Clase C
 */
int clase_teorica(unsigned char *ip)
{
    if (ip[0] >= 1 && ip[0] <= 127) {
        return 1; // Clase A
    } else if (ip[0] >= 128 && ip[0] <= 191) {
        return 2; // Clase B
    } else if (ip[0] >= 192 && ip[0] <= 223) {
        return 3; // Clase C
    }

    return -1; // Who knows?
}

/**
 * clase_practica - obtener la clasificación de una dirección IP de
 * acuerdo con los octetos que sean distintos de cero
 * 
 * @ip: La dirección IP de la cual se quiere obtener su clasificación
 * 
 * Para explicar su funcionamiento se utilizará el siguiente ejemplo:
 * Se tiene la dirección IP 148.204.58.0. De acuerdo con la Classful
 * Network Architecture, la dirección IP se clasifica como Clase B.
 * Sin embargo, en la dirección IP está asignado el tercer octeto;
 * de modo que solo se podrá trabajar con el cuarto octeto. En este
 * sentido, la dirección IP está siendo tratada como si fuera de
 * Clase C. La tarea de esta función es determinar esa clase.
 */
int clase_practica(unsigned char *ip)
{
    int clase = clase_teorica(ip);

    if (ip[1] == 0 && ip[2] == 0 && ip[3] == 0) { // Clase A
        if (clase > 1) {
            return clase;
        } else {
            return 1;
        }
    } else if (ip[2] == 0 && ip[3] == 0) { // Clase B
        if (clase > 2) {
            return clase;
        } else {
            return 2;
        }
    } else if (ip[3] == 0) { // Clase C
        if (clase > 3) {
            return clase;
        } else {
            return 3;
        }
    }

    return -1;
}

int clase_privada_practica(unsigned char *ip)
{
    if (ip[0] == 10) {
        if (ip[1] == 0 && ip[2] == 0 && ip[3] == 0) {
            return 1;
        } else if (ip[2] == 0 && ip[3] == 0) {
            return 2;
        } else if (ip[3] == 0) {
            return 3;
        }
    } else if (ip[0] == 172) {
        if (ip[1] >= 16 && ip[1] <= 31) {
            if (ip[1] == 16 && ip[2] == 0 && ip[3] == 0) {
                return 1;
            } else if (ip[2] == 0 && ip[3] == 0) {
                return 2;
            } else if (ip[3] == 0) {
                return 3;
            }
        }
    } else if (ip[0] == 192) {
        if (ip[1] == 168) {
            if (ip[2] == 0 && ip[3] == 0) {
                return 2;
            } else if (ip[3] == 0) {
                return 3;
            }
        }
    }
}

/**
 * es_privada - saber si una red es privada
 * 
 * @id: El identificador de la red que se va a analizar
 * 
 * La función determina si el identificador de red @id corresponde
 * a una red privada basada en los siguientes rangos de direcciones
 * IP para redes privadas:
 * 
 *  - 10.0.0.0/8: 10.0.0.0 - 10.255.255.255
 *  - 172.16.0.0/12: 172.16.0.0 - 172.31.255.255
 *  - 192.168.0.0/16: 192.168.0.0 - 192.168.255.255
 */
bool es_privada(unsigned char *id)
{
    if (id[0] == 10) {
        return true;
    } else if (id[0] == 172) {
        if (id[1] >= 16 && id[1] <= 31) {
            return true;
        }
    } else if (id[0] == 192) {
        if (id[1] == 168) {
            return true;
        }
    }

    return false;
}

/**
 * valor_octeto - determinar el valor de un byte de acuerdo al número
 * de bits que se tomen de él para subredes
 * 
 * @bits_subredes: La cantidad de bits que serán tomados del byte
 * 
 * Un byte se compone de 8 bits, los bits que se toman para subredes
 * deben ser 1 y el resto 0; por ejemplo, si se toman 3 bits para
 * subredes, entonces los bits del byte son 11100000, que en decimal
 * es 224. La tarea de esta función es determinar ese valor decimal.
 */
int valor_octeto(int bits_subredes)
{
    int valor;

    if (bits_subredes >= 0) {
        valor = 8 - bits_subredes;

        if (valor > 0) {
            valor = (int)potencia(2, valor-1);
            valor = valor | (valor-1);
            valor = valor ^ 255;
        } else {
            valor = 255;
        }
    } else {
        valor = -1;
    }

    return valor;
}

/**
 * es_id_subred - determinar si una dirección IP es un identificador
 * de subred
 * 
 * @direccion: La dirección IP que se va a analizar
 * 
 * La función comprueba que al menos el cuarto octeto de @direccion
 * es 0. Si es así, retorna true; en caso contrario, retorna false.
 */
bool es_id_subred(unsigned char *direccion)
{
    int i;

    if (direccion[3] != 0) {
        return false;
    }

    return true;
}

/**
 * crear_ip - almacenar una dirección IP en un arreglo de 4 enteros
 * dados sus 4 valores
 * 
 * @a: El primer octeto
 * @b: El segundo octeto
 * @c: El tercer octeto
 * @d: El cuarto octeto
 * @ip: El arreglo de 4 enteros donde se almacenará la dirección IP
 */
void crear_ip(int a, int b, int c, int d, unsigned char *ip)
{
    ip[0] = a;
    ip[1] = b;
    ip[2] = c;
    ip[3] = d;
}

/**
 * copiar_ip - copiar una dirección IP en otro arreglo de 4 enteros
 * 
 * @a: El arreglo de 4 enteros donde se copiará la dirección IP
 * @b: La dirección IP que va a ser copiada
 */
void copiar_ip(unsigned char *a, unsigned char *b)
{
    int i;

    for (i = 0; i < 4; i++) {
        a[i] = b[i];
    }
}

/**
 * mascara_inversa - obtener el complemento de una máscara de subred
 * 
 * @mascara: La máscara de subred
 * @inversa: El arreglo donde se almacenará el resultado
 * 
 * El complemento se obtiene restando el valor de cada octeto a
 * 255, por ejemplo:
 * 
 *      Máscara de subred: 255.255.8.0
 *      Máscara inversa: 0.0.247.255
 */
void mascara_inversa(unsigned char *mascara, unsigned char *inversa)
{
    int i;

    for (i = 0; i < 4; i++) {
        inversa[i] = 255 - mascara[i];
    }
}

/**
 * especificaciones - determinar las características de una dirección IP
 * 
 * @bits_hosts: Los bits que serán utilizados para hosts
 * @bits_subredes: Los bits que serán utilizados para subredes
 * @clase: La clase de la dirección IP
 * @hosts: La variable para almacenar el número de hosts por cada
 * subred
 * @subredes: La variable para almacenar el número de subredes
 * @mascara: La variable para almacenar la máscara de las subredes
 * @prefijo: La variable para almacenar la máscara de las subredes
 * en notación prefija
 * 
 * Dados los argumentos @bits_hosts, @bits_subredes y @clase; se
 * determinan el número de subredes y de hosts por cada subred así
 * como la máscara de subred y su notación prefija de un identificador
 * de red dado. Estos datos son almacenados en @hosts, @subredes,
 * @mascara y @prefijo si se pasan como argumentos a la función.
 */
void especificaciones(unsigned char *ip, int bits_hosts, int bits_subredes, int clase, int *hosts, int *subredes, unsigned char *mascara, int *prefijo)
{
    int i;

    if (hosts != NULL) {
        *hosts = potencia(2, bits_hosts)-2;
    }

    printf("\n  Hosts: %ld\n", potencia(2, bits_hosts)-2);

    if (bits_subredes == 0) {
        if (subredes != NULL) {
            *subredes = 0;
        }

        printf("  Subredes: %d\n", 0);
    } else {
        if (subredes != NULL) {
            *subredes = potencia(2, bits_subredes)-2;
        }

        printf("  Subredes: %ld\n", potencia(2, bits_subredes)-2);
    }
    
    switch (clase) {
        case 1:
            if (ip[0] == 172 && ip[1] == 16) {
                if (bits_subredes <= 4) {
                    if (mascara != NULL) {
                        for (i = 0; i < 4; i++) {
                            if (i < 1) {
                                mascara[i] = 255;
                            } else if (i == 1) {
                                mascara[i] = valor_octeto(bits_subredes+4);
                            } else {
                                mascara[i] = 0;
                            }
                        }
                    }

                    printf("  Mascara de subred: 255.%d.0.0 (/%d)\n", valor_octeto(bits_subredes+4), 8+bits_subredes+4);
                } else if (bits_subredes >= 5 && bits_subredes <= 12) {
                    if (mascara != NULL) {
                        for (i = 0; i < 4; i++) {
                            if (i < 2) {
                                mascara[i] = 255;
                            } else if (i == 2) {
                                mascara[i] = valor_octeto(bits_subredes+4);
                            } else {
                                mascara[i] = 0;
                            }
                        }
                    }

                    printf("  Mascara de subred: 255.255.%d.0 (/%d)\n", valor_octeto(bits_subredes-8+4), 8+bits_subredes+4);
                } else if (bits_subredes >= 13) {
                    if (mascara != NULL) {
                        for (i = 0; i < 3; i++) {
                            mascara[i] = 255;
                        }

                        mascara[i] = valor_octeto(bits_subredes+4-16);
                    }
                    
                    printf("  Mascara de subred: 255.255.255.%d (/%d)\n", valor_octeto(bits_subredes+4-16), 8+bits_subredes+4);
                }
            } else { 
                if (bits_subredes <= 8) {
                    if (mascara != NULL) {
                        for (i = 0; i < 4; i++) {
                            if (i < 1) {
                                mascara[i] = 255;
                            } else if (i == 1) {
                                mascara[i] = valor_octeto(bits_subredes);

                                if (ip[0] == 172 && ip[1] == 16) {
                                    mascara[i] = valor_octeto(bits_subredes+4);
                                }
                            } else {
                                mascara[i] = 0;
                            }
                        }
                    }

                    printf("  Mascara de subred: 255.%d.0.0 (/%d)\n", valor_octeto(bits_subredes), 8+bits_subredes);
                } else if (bits_subredes >= 9 && bits_subredes <= 16) {
                    if (mascara != NULL) {
                        for (i = 0; i < 4; i++) {
                            if (i < 2) {
                                mascara[i] = 255;
                            } else if (i == 2) {
                                mascara[i] = valor_octeto(bits_subredes-8);
                            } else {
                                mascara[i] = 0;
                            }
                        }
                    }

                    printf("  Mascara de subred: 255.255.%d.0 (/%d)\n", valor_octeto(bits_subredes-8), 8+bits_subredes);
                } else if (bits_subredes >= 17) {
                    if (mascara != NULL) {
                        for (i = 0; i < 3; i++) {
                            mascara[i] = 255;
                        }

                        mascara[i] = valor_octeto(bits_subredes-16);
                    }

                    printf("  Mascara de subred: 255.255.255.%d (/%d)\n", valor_octeto(bits_subredes-16), 8+bits_subredes);
                }
            }
            break;
        case 2:
            if (bits_subredes <= 8) {
                if (mascara != NULL) {
                    for (i = 0; i < 4; i++) {
                        if (i < 2) {
                            mascara[i] = 255;
                        } else if (i == 2) {
                            mascara[i] = valor_octeto(bits_subredes);
                        } else {
                            mascara[i] = 0;
                        }
                    }
                }

                printf("  Mascara de subred: 255.255.%d.0 (/%d)\n", valor_octeto(bits_subredes), 16+bits_subredes);
            } else if (bits_subredes >= 9) {
                if (mascara != NULL) {
                    for (i = 0; i < 3; i++) {
                        mascara[i] = 255;
                    }

                    mascara[i] = valor_octeto(bits_subredes-8);
                }

                printf("  Mascara de subred: 255.255.255.%d (/%d)\n", valor_octeto(bits_subredes-8), 16+bits_subredes);
            }
            break;
        case 3:
            if (bits_subredes <= 8) {
                if (mascara != NULL) {
                    for (i = 0; i < 3; i++) {
                        mascara[i] = 255;
                    }

                    mascara[i] = valor_octeto(bits_subredes);
                }

                printf("  Mascara de subred: 255.255.255.%d (/%d)\n", valor_octeto(bits_subredes), 24+bits_subredes);
            }
            break;    
    }

    if (prefijo != NULL) {
        if (clase == 1) {
            if (ip[0] == 172 && ip[1] == 16) {
                *prefijo = 12 + bits_subredes;
            } else {
                *prefijo = 8 + bits_subredes;
            }
        } else if (clase == 2) {
            *prefijo = 16 + bits_subredes;
        } else if (clase == 3) {
            *prefijo = 24 + bits_subredes;
        }
    }
}

/**
 * hosts - determinar las especificaciones de una red dado el número
 * de hosts que se requieren por subred
 * 
 * @ip: El identificador de red
 * @clase: La clase de la dirección IP de la red
 * @cantidad: La cantidad de hosts que se requieren por cada subred
 * @hosts: La variable para almacenar el número de hosts por cada
 * subred
 * @subredes: La variable para almacenar el número de subredes
 * @mascara: La variable para almacenar la máscara de las subredes
 * @prefijo: La variable para almacenar la máscara de las subredes
 * en notación prefija
 * 
 * Dados los datos @ip, @clase y @cantidad; se calcula el número de
 * bits a tomar de @ip para direccionar subredes y hosts y determinar
 * las especificaciones de la red con la función especificaciones().
 * Si los datos @ip, @clase y @cantidad son correctos y las
 * especificaciones de la red pudieron ser calculadas correctamente,
 * entonces la función retorna true; en caso contrario, retorna false.
 */
bool hosts(unsigned char *ip, int clase, int cantidad, int *hosts, int *subredes, unsigned char *mascara, int *prefijo)
{
    if (cantidad <= 0) {
        fprintf(stderr, "%s", "Cantidad de hosts no valida\n");
        return false;
    }

    int bits_hosts, bits_subredes;
    
    for (bits_hosts = 1; ; bits_hosts++) {
        if ((int)potencia(2, bits_hosts)-2 >= cantidad) {
            break;
        }
    }

    if (clase == 1) {
        bits_subredes = 24 - bits_hosts;
    } else if (clase == 2) {
        bits_subredes = 16 - bits_hosts;
    } else if (clase == 3) {
        bits_subredes = 8 - bits_hosts;
    }

    if (es_privada(ip) == true) {
        if ((clase = clase_privada_practica(ip)) == 1) {
            if (ip[0] == 172 && ip[1] == 16) {
                bits_subredes = 20 - bits_hosts;
            }
        } else if ((clase = clase_privada_practica(ip)) == 2) {   
            if (ip[0] == 192 && ip[1] == 168) {
                bits_subredes = 16 - bits_hosts;
            }
        }
    }

    if (bits_subredes < 0) {
        fprintf(stderr, "%s", "Cantidad de hosts no valida\n");
        return false;
    }

    especificaciones(ip, bits_hosts, bits_subredes, clase, hosts, subredes, mascara, prefijo);

    return true;
}

/**
 * subredes - determinar las especificaciones de una red dado el
 * número de subredes que se requieren
 * 
 * @ip: El identificador de red
 * @clase: La clase de la dirección IP de la red
 * @cantidad: La cantidad de subredes que se requieren
 * @hosts: La variable para almacenar el número de hosts por cada
 * subred
 * @subredes: La variable para almacenar el número de subredes
 * @mascara: La variable para almacenar la máscara de las subredes
 * @prefijo: La variable para almacenar la máscara de las subredes
 * en notación prefija
 * 
 * Dados los datos @ip, @clase y @cantidad; se calcula el número de
 * bits a tomar de @ip para direccionar subredes y hosts y determinar
 * las especificaciones de la red con la función especificaciones().
 * Si los datos @ip, @clase y @cantidad son correctos y las
 * especificaciones de la red pudieron ser calculadas correctamente,
 * entonces la función retorna true; en caso contrario, retorna false.
 */
bool subredes(unsigned char *ip, int clase, int cantidad, int *hosts, int *subredes, unsigned char *mascara, int *prefijo)
{
    if (cantidad < 0) {
        fprintf(stderr, "%s", "Cantidad de subredes no valida\n");
        return false;
    }

    int bits_hosts, bits_subredes;
    
    if (cantidad > 0) {
        for (bits_subredes = 1; ; bits_subredes++) {
            if ((int)potencia(2, bits_subredes)-2 >= cantidad) {
                break;
            }
        }
    } else {
        bits_subredes = 0;
    }

    if (clase == 1) {
        bits_hosts = 24 - bits_subredes;
    } else if (clase == 2) {
        bits_hosts = 16 - bits_subredes;
    } else if (clase == 3) {
        bits_hosts = 8 - bits_subredes;
    }
    
    if (es_privada(ip) == true) {
        if ((clase = clase_privada_practica(ip)) == 1) {
            if (ip[0] == 172 && ip[1] == 16) {
                bits_hosts = 20 - bits_subredes;
            }
        } else if ((clase = clase_privada_practica(ip)) == 2) {   
            if (ip[0] == 192 && ip[1] == 168) {
                bits_hosts = 16 - bits_subredes;
            }
        }
    }

    if (bits_hosts < 2) {
        fprintf(stderr, "%s", "Cantidad de subredes no valida\n");
        return false;
    }

    especificaciones(ip, bits_hosts, bits_subredes, clase, hosts, subredes, mascara, prefijo);

    return true;
}

/**
 * pre - determinar las especificaciones de una red dado el
 * prefijo de sus subredes
 * 
 * @ip: El identificador de red
 * @clase: La clase de la dirección IP de la red
 * @prefj: El prefijo que tendrán las subredes
 * @hosts: La variable para almacenar el número de hosts por cada
 * subred
 * @subredes: La variable para almacenar el número de subredes
 * @mascara: La variable para almacenar la máscara de las subredes
 * @prefijo: La variable para almacenar la máscara de las subredes
 * en notación prefija
 * 
 * Dados los datos @ip, @clase y @prefj; se calcula el número de bits
 * a tomar de @ip para direccionar subredes y hosts y determinar
 * las especificaciones de la red con la función especificaciones().
 * Si los datos @ip, @clase y @prefj son correctos y las
 * especificaciones de la red pudieron ser calculadas correctamente,
 * entonces la función retorna true; en caso contrario, retorna false.
 */
bool pre(unsigned char *ip, int clase, int prefj, int *hosts, int *subredes, unsigned char *mascara, int *prefijo)
{
    if (prefj < 0) {
        fprintf(stderr, "%s", "Prefijo no valido\n");
        return false;
    }

    int bits_hosts, bits_subredes;

    switch (clase) {
        case 1:
            bits_subredes = prefj - 8;
            bits_hosts = 24 - bits_subredes;
            break;
        case 2:
            bits_subredes = prefj - 16;
            bits_hosts = 16 - bits_subredes;
            break;
        case 3:
            bits_subredes = prefj - 24;
            bits_hosts = 8 - bits_subredes;
            break;
    }

    if (es_privada(ip) == true) {
        if ((clase = clase_privada_practica(ip)) == 1) {
            if (ip[0] == 172 && ip[1] == 16) {
                bits_subredes = prefj - 12;
                bits_hosts = 20 - bits_subredes;
            }
        } else if ((clase = clase_privada_practica(ip)) == 2) {   
            if (ip[0] == 192 && ip[1] == 168) {
                bits_subredes = prefj - 16;
                bits_hosts = 16 - bits_subredes;
            }
        }
    }

    if (bits_subredes < 0) {
        fprintf(stderr, "%s", "Prefijo no valido\n");
        return false;
    }

    especificaciones(ip, bits_hosts, bits_subredes, clase, hosts, subredes, mascara, prefijo);

    return true;
}

/**
 * calcular_subredes - calcular los identificadores de las subredes
 * de una red
 * 
 * @id: El identificador de la red
 * @mascara: La máscara de subred de @id
 * 
 * La función calcula las direcciones IP de las subredes de una
 * red y las almacena en un archivo llamado "subredes".
 */
void calcular_subredes(unsigned char *id, unsigned char *mascara)
{
    int i, j, k, contador = 0, clase;
    unsigned char subred_actual[4], ip_actual[4], subred_anterior[4];
    FILE *subredes = NULL;

    system("rm -f subredes && touch subredes");
    subredes = fopen("subredes", "a");

    if ((id[0] == 172 && id[1] == 16) || (id[0] == 192 && id[1] == 168)) {
        clase = clase_privada_practica(id);
    } else {
        clase = clase_practica(id);
    }

    switch (clase) {
        case 1:
            for (i = 0; i < 256; i++) {
                for (j = 0; j < 256; j++) {
                    for (k = 0; k < 256; k++) {
                        crear_ip(id[0], i, j, k, ip_actual); // La dirección IP actual
                        aplicar_mascara(ip_actual, mascara, subred_actual); // Aplicar la máscara de subred a la dirección IP actual

                        if (id[0] == 172 && id[1] == 16) {
                            if (ip_actual[1] >= 16 && ip_actual[1] <= 31) {
                                if (misma_subred(id, subred_actual) == false) { // No incluir el id ingresado
                                    if (subred_actual[1] != 31 || subred_actual[2] != mascara[2] || subred_actual[3] != mascara[3]) {
                                        if (misma_subred(subred_anterior, subred_actual) == false) { // Solo incluir id's diferentes
                                            copiar_ip(subred_anterior, subred_actual);
                                            fprintf(subredes, "%d.%d.%d.%d\n", subred_actual[0], subred_actual[1], subred_actual[2], subred_actual[3]);
                                            contador = contador + 1;
                                        }
                                    }
                                }
                            }
                        } else {
                            if (misma_subred(id, subred_actual) == false) {
                                if (subred_actual[1] != mascara[1] || subred_actual[2] != mascara[2] || subred_actual[3] != mascara[3]) {
                                    if (misma_subred(subred_anterior, subred_actual) == false) {
                                        copiar_ip(subred_anterior, subred_actual);
                                        fprintf(subredes, "%d.%d.%d.%d\n", subred_actual[0], subred_actual[1], subred_actual[2], subred_actual[3]);
                                        contador = contador + 1;
                                    }
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 2:
            for (i = 0; i < 256; i++) {
                for (j = 0; j < 256; j++) {
                    crear_ip(id[0], id[1], i, j, ip_actual); // La dirección IP actual
                    aplicar_mascara(ip_actual, mascara, subred_actual); // Aplicar la máscara de subred a la dirección IP actual
                    
                    if (misma_subred(id, subred_actual) == false) {
                        if (subred_actual[2] != mascara[2] || subred_actual[3] != mascara[3]) {
                            if (misma_subred(subred_anterior, subred_actual) == false) {
                                copiar_ip(subred_anterior, subred_actual);
                                fprintf(subredes, "%d.%d.%d.%d\n", subred_actual[0], subred_actual[1], subred_actual[2], subred_actual[3]);
                                contador = contador + 1;
                            }
                        }
                    }
                }
            }
            break;
        case 3:
            for (i = 0; i < 256; i++) {
                crear_ip(id[0], id[1], id[2], i, ip_actual); // Cada una de las direcciones IP
                aplicar_mascara(ip_actual, mascara, subred_actual); // Aplicar la máscara de subred a la dirección IP actual

                if (misma_subred(id, subred_actual) == false) {
                    if (subred_actual[3] != mascara[3]) {
                        if (misma_subred(subred_anterior, subred_actual) == false) {
                            copiar_ip(subred_anterior, subred_actual);
                            fprintf(subredes, "%d.%d.%d.%d\n", subred_actual[0], subred_actual[1], subred_actual[2], subred_actual[3]);
                            contador = contador + 1;
                        }
                    }
                }
            }
            break;
    }
    
    fclose(subredes);
}

/**
 * calcular_hosts - calcular las direcciones IP de los hosts de una subred
 * 
 * @id: El identificador de la subred
 * @mascara: La máscara de @id
 * 
 * La función calcula las direcciones IP de los hosts de una subred
 * y las almacena en un archivo llamado "hosts".
 */
void calcular_hosts(unsigned char *id, unsigned char *mascara)
{
    int i, j, k;
    unsigned char ip[4], subred[4], inversa[4], aux[4];
    FILE *hosts = NULL;

    system("rm -f hosts && touch hosts");
    hosts = fopen("hosts", "a");

    mascara_inversa(mascara, inversa);

    switch (clase_teorica(id)) {
        case 1:
            for (i = 0; i < 256; i++) {
                for (j = 0; j < 256; j++) {
                    for (k = 0; k < 256; k++) {
                        crear_ip(id[0], i, j, k, ip);
                        aplicar_mascara(ip, mascara, subred);
                        aplicar_mascara(ip, inversa, aux);

                        if (misma_subred(id, subred) == true) {
                            if (misma_subred(id, ip) == false) {
                                if (misma_subred(aux, inversa) == false) {
                                    fprintf(hosts, "%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
                                }
                            }
                        }
                    }
                }
            }
            break;
        case 2:
            for (i = 0; i < 256; i++) {
                for (j = 0; j < 256; j++) {
                    crear_ip(id[0], id[1], i, j, ip);
                    aplicar_mascara(ip, mascara, subred);
                    aplicar_mascara(ip, inversa, aux);

                    if (misma_subred(id, subred) == true) {
                        if (misma_subred(id, ip) == false) {
                            if (misma_subred(aux, inversa) == false) {
                                fprintf(hosts, "%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
                            }
                        }
                    }
                }
            }
            break;
        case 3:
            for (i = 0; i < 256; i++) {
                crear_ip(id[0], id[1], id[2], i, ip);
                aplicar_mascara(ip, mascara, subred);
                aplicar_mascara(ip, inversa, aux);

                if (misma_subred(id, subred) == true) {
                    if (misma_subred(id, ip) == false) {
                        if (misma_subred(aux, inversa) == false) {
                            fprintf(hosts, "%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
                        }
                    }
                }
            }
            break;
        default:
            for (i = 0; i < 256; i++) {
                crear_ip(id[0], id[1], id[2], i, ip);
                aplicar_mascara(ip, mascara, subred);
                aplicar_mascara(ip, inversa, aux);

                if (misma_subred(id, subred) == true) {
                    if (misma_subred(id, ip) == false) {
                        if (misma_subred(aux, inversa) == false) {
                            fprintf(hosts, "%d.%d.%d.%d\n", ip[0], ip[1], ip[2], ip[3]);
                        }
                    }
                }
            }
            break;
    }

    fclose(hosts);
}

/**
 * listar_subredes - mostrar en pantalla los identificadores de subred
 * del archivo "subredes"
 * 
 * @prefj: La máscara de red de las subredes
 * 
 * La función muestra una lista numerada de las direcciones IP
 * almacenadas en el archivo "subredes".
 */
void listar_subredes(int prefj)
{
    int contador = 1;
    char subred[17];
    FILE *subredes = NULL;

    subredes = fopen("subredes", "r");

    while (fgets(subred, sizeof(subred), subredes) != NULL) {
        subred[strlen(subred)-1] = '\0';
        printf("%d. %s/%d\n", contador, subred, prefj);
        contador = contador + 1;
    }

    fclose(subredes);
}

/**
 * listar_hosts - mostrar en pantalla las direcciones IP de hosts del
 * archivo "hosts"
 * 
 * La función muestra una lista numerada de las direcciones IP
 * almacenadas en el archivo "hosts".
 */
void listar_hosts(void)
{
    int contador = 1;
    char host[17];
    FILE *hosts = NULL;

    hosts = fopen("hosts", "r");

    while (fgets(host, sizeof(host), hosts) != NULL) {
        host[strlen(host)-1] = '\0';
        printf("%d. %s\n", contador, host);
        contador = contador + 1;
    }

    fclose(hosts);
}
