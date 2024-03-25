#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <linux/if_packet.h>
#include <sys/time.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <net/if.h>
#include <net/ethernet.h>
#include "../include/redes.h"

/**
 * touch - ejecuta el comando touch sobre un archivo
 * 
 * @archivo: El nombre del archivo 
 */
void touch(char *archivo)
{
    char comando[strlen(archivo)+7];

    strcpy(comando, "touch ");
    strcat(comando, archivo);
    system(comando);
}

/**
 * potencia - elevar un número a una potencia entera
 * 
 * @base: La base que será elevada
 * @potencia: La potencia a la que se elevará @base
 * 
 * Si @potencia no es un entero mayor o igual a cero,
 * la función retorna -1.
 */
long potencia(int base, int potencia)
{
    long resultado = (long)base;

    if (potencia > 0) {
        while (potencia > 1) {
            resultado = resultado * (long)base;
            potencia = potencia - 1;
        }
    } else if (potencia == 0) {
        resultado = 1;
    } else {
        resultado = -1;
    }

    return resultado;
}

/**
 * valor_numerico - obtener el valor numérico de una cadena
 * 
 * @cadena: El número expresado como una cadena
 * 
 * La función devuelve el valor numérico de una cadena que expresa
 * un número. Así, para la cadena "2017" devuelve el entero 2017.
 */
int valor_numerico(char *cadena)
{
    int i, valor = 0;

    for (i = 0; i < strlen(cadena); i++) {
        valor = valor * 10 + (cadena[i] - '0');
    }

    return valor;
}

/**
 * imprimir_mac - imprimir una dirección MAC en la forma xx:xx:xx:xx:xx:xx
 * 
 * @mac: El arreglo que contiene los dígitos de la dirección MAC
 */
void imprimir_mac(unsigned char *mac)
{
    int i;

    for (i = 0; i < 6; i++) {
        printf("%.2x", mac[i]);

        if (i < 5) {
            printf(":");
        }
    }
}

/**
 * imprimir_ip - imprimir una dirección IP en la forma xxx.xxx.xxx.xxx
 * 
 * @ip: El arreglo que contiene los octetos de la dirección IP
 */
void imprimir_ip(unsigned char *ip)
{
    int i;

    for (i = 0; i < 4; i++) {
        printf("%d", ip[i]);

        if (i < 3) {
            printf(".");
        }
    }   
}

/**
 * imprimir_trama - imprimir una trama
 * 
 * @trama: La trama que se va a imprimir
 * @longitud: El número de dígitos de @trama
 */
void imprimir_trama(unsigned char *trama, int longitud)
{
    int i;

    for (i = 0; i < longitud; i++) {
        printf("%.2x ", trama[i]);

        if ((i+1) % 16 == 0) {
            printf("\n");
        }
    }
}

/**
 * imprimir_datos - imprime los datos de una interfaz de red
 * 
 * @datos: La estructura que contiene los datos de la interfaz de red
 * 
 * Imprime los datos de una interfaz de red almacenados en una
 * estructura.
 */
void imprimir_datos(Interfaz interfaz)
{
    int i;

    printf("Nombre: %s\n", interfaz.nombre);
    printf("Indice: %d\n", interfaz.indice);
    printf("Direccion MAC: ");

    for (i = 0; i < 6; i++) {
        printf("%.2x", interfaz.direccion_mac[i]);

        if (i < 5) {
            printf(":");
        }
    }

    printf("\nDireccion IP: ");

    for (i = 0; i < 4; i++) {
        printf("%d", interfaz.direccion_ip[i]);

        if (i < 3) {
            printf(".");
        }
    }

    printf("\nMascara de subred: ");

    for (i = 0; i < 4; i++) {
        printf("%d", interfaz.mascara_subred[i]);

        if (i < 3) {
            printf(".");
        }
    }

    printf("\n");
}

/**
 * limpiar_trama - igualar todos los bytes de una trama con 0x00
 * 
 * @trama: La trama que será limpiada
 */
void limpiar_trama(unsigned char *trama)
{
    int i;

    for (i = 0; i < 1514; i++) {
        trama[i] = 0x00;
    }
}

/**
 * ip_arreglo - convertir una dirección IP expresada en una cadena
 * a un arreglo de 4 elementos
 * 
 * @arreglo: El arreglo de cuatro enteros para almacenar la IP destino
 * @cadena: La cadena de la dirección IP en la forma xxx.xxx.xxx.xxx
 * 
 * Se convierten cada uno de los octetos de @cadena en números
 * enteros que son almacenados en @arreglo.
 * Si la dirección IP de @cadena no es válida, retorna
 * false; si la dirección IP es válida, retorna true.
 */
bool ip_arreglo(unsigned char *arreglo, char *cadena)
{
    int i = 0, j = 0;
    char numero[4], *caracter;
    struct in_addr direccion;

    if (inet_aton(cadena, &direccion) == 0) { // ¿La dirección IP ingresada es válida?
        return false;
    }

    caracter = cadena;

    while (*caracter != '\0') {
        if (*caracter != '.') {
            numero[i] = *caracter;
            i = i + 1;
        } else {
            numero[i] = '\0';
            arreglo[j] = valor_numerico(numero);
            i = 0;
            j = j + 1;
        }

        caracter = caracter + 1;
    }

    numero[i] = '\0';
    arreglo[j] = valor_numerico(numero);

    return true;
}

/**
 * obtener_datos - obtener los datos de una interfaz de red
 * 
 * @descriptor_socket: El descriptor del socket
 * @nombre: El nombre de la interfaz de red
 * @interfaz: La estructura donde se alamcenarán los datos
 * 
 * La función obtiene los datos de la interfaz de red que tiene por
 * nombre @nombre y los almacena en @interfaz. Los datos que se
 * obtienen son:
 *  - Nombre
 *  - índice
 *  - Dirección MAC
 *  - Dirección IP
 *  - Máscara de subred
 * 
 * Si se logra obtener todos los datos de la interfaz de red, la
 * función retorna true; en caso contrario, retorna false.
 */
bool obtener_datos(int descriptor_socket, char *nombre, Interfaz *interfaz)
{
    struct ifreq nic;
    
    strcpy(nic.ifr_name, nombre);

    /* Obtener el índice de la interfaz */
    if (ioctl(descriptor_socket, SIOCGIFINDEX, &nic) == -1) {
        return false;
    }

    strcpy(interfaz->nombre, nombre);
    interfaz->indice = nic.ifr_ifindex;

    /* Obtener la direccion MAC */
    if (ioctl(descriptor_socket, SIOCGIFHWADDR, &nic) == -1) {
        return false;
    }

    memcpy(interfaz->direccion_mac, nic.ifr_hwaddr.sa_data, 6);

    /* Obtener la direccion IP */
    if (ioctl(descriptor_socket, SIOCGIFADDR, &nic) == -1) {
        return false;
    }

    memcpy(interfaz->direccion_ip, nic.ifr_addr.sa_data+2, 4);

    /* Obtener la máscara de subred */
    if (ioctl(descriptor_socket, SIOCGIFNETMASK, &nic) == -1) {
        return false;
    }

    memcpy(interfaz->mascara_subred, nic.ifr_netmask.sa_data+2, 4);

    return true;
}

/**
 * obtener_gateway - obtener la dirección IP de la puerta de enlace
 * de la red a la que se está conectada/o
 * 
 * @gateway: El arreglo de cuatro enteros para almacenar la IP de
 * la puerta de enlace
 * 
 * Con la ayuda de la función system(), se consulta la dirección IP
 * de la puerta de enlace en la terminal; luego se almacena la cadena
 * de la dirección IP en un archivo llamado "gateway".
 * Posteriormente, la cadena es obtenida del archivo para obtener el
 * valor numérico de cada octeto y almacenarlos en @gateway.
 * Si la dirección IP es obtenida con éxito, retorna true; de lo
 * contrario, retorna false.
 */
bool obtener_gateway(unsigned char *gateway)
{
    if (system("route -n | grep UG | grep -Eo \"[1-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}.[0-9]{1,3}\" > gateway") == -1) {
        return false;
    }

    int i = 0, j = 0;
    FILE *archivo = NULL;
    char linea[17], numero[4], *caracter;
    struct in_addr direccion;

    archivo = fopen("gateway", "r");

    if (fgets(linea, sizeof(linea), archivo) != NULL) {
        linea[strlen(linea)-1] = '\0';

        if (inet_aton(linea, &direccion) != 0) {
            caracter = linea;

            while (*caracter != '\0') {
                if (*caracter != '.') {
                    numero[i] = *caracter;
                    i = i + 1;
                } else {
                    numero[i] = '\0';
                    gateway[j] = valor_numerico(numero);
                    i = 0;
                    j = j + 1;
                }

                caracter = caracter + 1;
            }

            numero[i] = '\0';
            gateway[j] = valor_numerico(numero);

            fclose(archivo);
            system("rm -f gateway");
        }
    }

    return true;
}

/**
 * obtener_puerto_efimero - obtener un número de puerto efimero
 * 
 * La función obtiene un número pseudoaleatorio en el rango de
 * puertos efímeros.
 */
int obtener_puerto_efimero(void)
{
    int maximo, minimo, rango, puerto;
    struct timeval semilla;

    gettimeofday(&semilla, NULL);
    srandom((unsigned int)(semilla.tv_sec));

    minimo = 49152;
    maximo = 65535;
    rango = maximo - minimo + 1;
    puerto = ((int)(random()) % rango) + minimo;

    return puerto;
}

/**
 * aplicar_mascara - aplicar una máscara de subred a una dirección IP
 * 
 * @ip: La dirección IP a la cuál se le aplicará la máscara de subred
 * @mascara: La máscara de subred que se va a aplicar
 * @id: El arreglo de 4 enteros donde se almacenará el resultado
 */
void aplicar_mascara(unsigned char *ip, unsigned char *mascara, unsigned char *id)
{
    int i;

    for (i = 0; i < 4; i++) {
        id[i] = ip[i] & mascara[i];
    }
}

/**
 * misma_subred - determinar si dos identificadores de subred son iguales
 * 
 * @subred_a: El identificador de la subred A
 * @subred_b: El identificador de la subred B
 * 
 * La función compara de par en par los octetos de ambas subredes.
 * Si ambos identificadores son iguales, retorna true; de lo
 * contrario, retorna false.
 */
bool misma_subred(unsigned char *subred_a, unsigned char *subred_b)
{
    int i;

    for (i = 0; i < 4; i++) {
        if (subred_a[i] != subred_b[i]) {
            return false;
        }
    }

    return true;
}

/**
 * checksum - calcular la suma de comprobación de una trama
 * 
 * @datos: La trama que se va a verificar
 * @longitud: La longitud de la trama
 * @resultado: El arreglo de 2 bytes donde se alamcenará el cálculo
 * 
 * Los bytes son agrupados de dos en dos y sumados, por ejemplo:
 * 
 *       af 54 c0 78 00 ac -> 0xaf54 + 0xc078 + 0x00ac
 * 
 * Si el resultado tiene acarreo, este es sumado a los primeros 4
 * dígitos, por ejemplo:
 * 
 *                   0x78afd -> 0x8afd + 0x07
 * 
 * Si el resultado tiene acarreo, se realiza el procedimiento anterior
 * una vez más. Luego se saca el complemento del resultado, por ejemplo:
 * 
 *                  0xffff - 0xa1c0 -> 0x5e3f
 * 
 * El resultado es almacenado en @resultado.
 */
void checksum(unsigned char *datos, int longitud, unsigned char *resultado)
{
    int suma = 0, acarreo, i;

    for (i = 0; i < longitud; i = i + 2) {
        if (longitud % 2 != 0) {
            if (i == longitud-1) {
                suma = suma + datos[i] * 0x100;
                break;
            }
        }

        suma = suma + datos[i] * 0x100 + datos[i+1];
    }

    acarreo = suma / 0x10000;
    suma = suma - acarreo * 0x10000;

    /* Primera suma del acarreo */
    suma = suma + acarreo;
    acarreo = suma / 0x10000;
    suma = suma - acarreo * 0x10000;

    /* Segunda suma del acarreo */
    suma = suma + acarreo;
    acarreo = suma / 0x10000;
    suma = suma - acarreo * 0x10000;

    /* Complemento */
    suma = 0xffff - suma;

    resultado[0] = suma / 0x100;
    resultado[1] = suma - suma / 0x100 * 0x100;
}

/**
 * checksum_pseudo - calcular el checksum de una trama de un protocolo
 * de la capa de transporte
 * 
 * @trama: La trama del protocolo de la capa de transporte
 * @longitud: La longitud del encabezado y los datos
 * @resultado: El arreglo donde se almacenará el resultado de la suma
 * 
 * La función crea una trama que contiene el pseudoencabezado, el
 * encabezado del protocolo correspondiente (TCP/UDP) y los datos que
 * contiene (en ese orden) y calcula el checksum; que es copiado
 * en @resultado.
 */
void checksum_pseudo(unsigned char *trama, int longitud, unsigned char *resultado)
{
    unsigned char trama_auxiliar[longitud+12];
    unsigned char len[2] = {longitud/0x100*0x100, longitud-longitud/0x100*0x100};

    /* Dirección IP origen */
    memcpy(trama_auxiliar+0, trama+26, 4);
    /* Dirección IP destino */
    memcpy(trama_auxiliar+4, trama+30, 4);
    /* Byte en cero */
    trama_auxiliar[8] = 0x00;
    /* Protocolo */
    memcpy(trama_auxiliar+9, trama+23, 1);
    /* Longitud */
    memcpy(trama_auxiliar+10, len, 2);
    /* Encabezado y datos del protocolo */
    memcpy(trama_auxiliar+12, trama+34, longitud);
    /* Cálculo de la suma de comprobación */
    checksum(trama_auxiliar, longitud+12, resultado);
}

/**
 * encabezado_mac - estructurar el encabezado MAC de una trama
 * 
 * @trama: La trama que se está estructurando
 * @destino: La direccion MAC destino
 * @fuente: La dirección MAC fuente
 * @ethertype: El protocolo al que se está brindando el servicio
 */
void encabezado_mac(unsigned char *trama, unsigned char *destino, unsigned char *fuente, unsigned char *ethertype)
{
    /* Dirección destino */
    memcpy(trama+0, destino, 6);
    /* Dirección fuente */
    memcpy(trama+6, fuente, 6);
    /* Ethertype */
    memcpy(trama+12, ethertype, 2);
}

/**
 * mensaje_arp - estructurar una solicitud de ARP
 * 
 * @trama: La trama que se va a estructurar
 * @origen: Los datos de la interfaz de red
 * @direccion_ip_destino: La dirección IP de destino
 * 
 * Se estructura @trama para contener una solicitud de ARP a la
 * dirección IP @destino.
 */
void mensaje_arp(unsigned char *trama, Interfaz interfaz, unsigned char *destino)
{
    unsigned char hardware[2] = {0x00, 0x01};
    unsigned char protocolo[2] = {0x08, 0x00};
    unsigned char long_dir_hw[1] = {0x06};
    unsigned char long_dir_prot[1] = {0x04};
    unsigned char codigo_operacion[2] = {0x00, 0x01};
    unsigned char mac_broadcast[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
    
    /* Tipo de hardware */
    memcpy(trama+14, hardware, 2);
    /* Protocolo al que se le presta el servicio */
    memcpy(trama+16, protocolo, 2);
    /* Longitud de direcciones de hardware */
    memcpy(trama+18, long_dir_hw, 1);
    /* Longitud de direcciones de protocolo */
    memcpy(trama+19, long_dir_prot, 1);
    /* Código de operación */
    memcpy(trama+20, codigo_operacion, 2);
    /* Dirección de hardware origen */
    memcpy(trama+22, interfaz.direccion_mac, 6);
    /* Dirección lógica origen */
    memcpy(trama+28, interfaz.direccion_ip, 4);
    /* Dirección de hardware destino */
    memcpy(trama+32, mac_broadcast, 6);
    /* Dirección lógica destino */
    memcpy(trama+38, destino, 4);
}

/**
 * encabezado_ip - estructurar el encabezado de un datagrama IP
 * 
 * @trama: La trama que se va a enviar
 * @origen: La dirección IP del origen
 * @destino: La dirección IP del destino
 * @longitud: La longitud del datagrama IP
 * @ttl: Tiempo de vida de la trama
 * @protocolo: El protocolo al que IP brindará el servicio
 */
void encabezado_ip(unsigned char *trama, unsigned char *origen, unsigned char *destino, int longitud, int ttl, int protocolo)
{
    unsigned char version_ihl[1] = {0x45};
    unsigned char tipo_servicio[1] = {0x00};
    unsigned char longitud_datagrama[2] = {longitud/0x100, longitud-longitud/0x100*0x100};
    unsigned char identificador[2] = {(int)random(), (int)random()};
    unsigned char banderas_desplazamiento[2] = {0x00, 0x00};
    unsigned char tiempo_vida[1] = {ttl};
    unsigned char protocol[1] = {protocolo};
    unsigned char suma[2] = {0x00, 0x00};

    /* versión = IPv4; IHL = 20 bytes */
    memcpy(trama+14, version_ihl, 1);
    /* Tipo de servicio */
    memcpy(trama+15, tipo_servicio, 1);
    /* Longitud de datagrama IP */
    memcpy(trama+16, longitud_datagrama, 2);
    /* Identificador del datagrama IP */
    memcpy(trama+18, identificador, 2);
    /* Banderas: DF = 0, MF = 0; Desplazamiento = 0 */
    memcpy(trama+20, banderas_desplazamiento, 2);
    /* Tiempo de vida */
    memcpy(trama+22, tiempo_vida, 1);
    /* Protocolo al que se le brinda servicio */
    memcpy(trama+23, protocol, 1);
    /* Suma de comprobación inicial */
    memcpy(trama+24, suma, 2);
    /* Dirección IP de origen */
    memcpy(trama+26, origen, 4);
    /* Dirección IP de destino */
    memcpy(trama+30, destino, 4);
    /* Se hace la suma de comprobación */
    checksum(trama+14, 20, suma);
    /* Suma de comprobación */
    memcpy(trama+24, suma, 2);
}

/**
 * filtro_arp - bandera para poder filtrar respuestas de ARP
 * 
 * @trama_recibida: Una trama recibida del socket
 * @trama_enviada: La trama que se envió
 * 
 * La función comprueba si @trama_recibida es la respuesta a la
 * solicitud de ARP de @trama_enviada. 
 */
bool filtro_arp(unsigned char *trama_recibida, unsigned char *trama_enviada)
{
    unsigned char ethertype[2] = {0x08, 0x06};
    unsigned char codigo_operacion[2] = {0x00, 0x02};

    /* La dirección MAC destino es la mía */
    if (memcmp(trama_recibida+0, trama_enviada+6, 6) == 0) {
        /* El ethertype es 0x0806 (ARP) */
        if (memcmp(trama_recibida+12, ethertype, 2) == 0) {
            /* El código de operación es 0x02 (respuesta ARP) */
            if (memcmp(trama_recibida+20, codigo_operacion, 2) == 0) {
                /* La dirección IP origen es la dirección IP buscada */
                if (memcmp(trama_recibida+28, trama_enviada+38, 4) == 0) {
                    /* La dirección IP destino es la mía */
                    if (memcmp(trama_recibida+38, trama_enviada+28, 4) == 0) {
                        return true;
                    }
                }
            }
        }
    }

    return false;
}

/**
 * enviar_trama - enviar una trama
 * 
 * @descriptor_socket: El descriptor del socket
 * @indice: Índice de la interfaz de red
 * @trama: La trama que se va a enviar
 * @longitud: La longitud de la trama
 */
bool enviar_trama(int descriptor_socket, int indice, unsigned char *trama, int longitud)
{
    int enviado;
    struct sockaddr_ll interfaz;

    /* Borrado de los datos de la estructura */
    memset(&interfaz, 0x00, sizeof(interfaz));

    /* Configuración de los datos de la estructura */
    interfaz.sll_family = AF_PACKET;
    interfaz.sll_protocol = htons(ETH_P_ALL);
    interfaz.sll_ifindex = indice;

    /* Envío de la trama */
    enviado = sendto(descriptor_socket, trama, longitud, 0, (struct sockaddr *)&interfaz, sizeof(interfaz));

    if (enviado == -1) {
        return false;
    }

    return true;
}

/**
 * recibir_trama - recibir una trama
 * 
 * @descriptor_socket: El descriptor del socket
 * @trama_recibida: Arreglo donde se almacenará la trama recibida
 * @trama_enviada: La trama enviada
 * @filtro: Una función para filtrar la trama que se recibe
 * 
 * Si no se requiere filtrar la trama que se recibe, entonces retorna
 * true apenas se recibe una trama.
 * Si se requiere filtrar la trama que se recibe, @trama_enviada y
 * @filtro deben ser distintos de NULL; si la trama que se recibe
 * cumple con el filtro, retorna true.
 * En ambos casos, el tiempo de espera para recibir una trama son 200
 * milisegundos.
 */
bool recibir_trama(int descriptor_socket, unsigned char *trama_recibida, unsigned char *trama_enviada, \
    bool (*filtro)(unsigned char *, unsigned char *), long *tiempo, ssize_t *longitud)
{
    bool bandera = false;
    int recibido;
    struct timeval inicio, final;
    long tiempo_espera = 0, segundos, microsegundos;    

    gettimeofday(&inicio, NULL);

    /* NOTA: si requiero conocer la información de la interfaz de red desde la
     * que me fueron enviados los datos entonces debo definir su estructura e
     * incluirla en los parámetros de la función recvfrom().
     */
    while (tiempo_espera < 200) {
        recibido = recvfrom(descriptor_socket, trama_recibida, 1514, MSG_DONTWAIT, NULL, 0);

        if (recibido != -1) {
            bandera = true;

            if (filtro != NULL && trama_enviada != NULL) {
                if (filtro(trama_recibida, trama_enviada) == false) {
                    bandera = false;
                }
            }
        }

        gettimeofday(&final, NULL);

        segundos = final.tv_sec - inicio.tv_sec;
        microsegundos = final.tv_usec - inicio.tv_usec;
        tiempo_espera = ((segundos) * 1000 + microsegundos/1000.0) + 0.5;

        if (bandera == true) {
            break;
        }
    }

    if (tiempo != NULL) {
        *tiempo = tiempo_espera;
    }

    if (longitud != NULL) {
        *longitud = recibido;
    }

    return bandera;
}
