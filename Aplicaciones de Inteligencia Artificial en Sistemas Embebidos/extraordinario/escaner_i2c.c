#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "twi.h"

/* 
 * Lee el nivel fisico de las dos lineas del bus. PINC refleja el pin
 * real aunque el periferico TWI tenga el control.
 */
static void reporta_lineas(void)
{
    uint8_t p   = PINC;
    uint8_t sda = (p >> 4) & 1; // A4 = PC4 = SDA
    uint8_t scl = (p >> 5) & 1; // A5 = PC5 = SCL

    uart_print("Reposo:  SDA=");
    uart_tx(sda ? '1' : '0');
    uart_print("  SCL=");
    uart_tx(scl ? '1' : '0');
    uart_print("\r\n");

    if (!sda || !scl) {
        uart_print("Linea en bajo...\r\n");
    }
}

int main(void)
{
    uart_init();
    twi_init();

    while (1) {
        reporta_lineas();

        /* Probar un START suelto: ¿el bus deja completar la operacion? */
        uint8_t st = twi_start();
        twi_stop();

        if (st == 0xFF) {
            uart_print("Bus colgado...\r\n");
            _delay_ms(2000);
            continue;
        }

        uint8_t encontrados = 0;
        for (uint8_t addr = 1; addr < 127; addr++) {
            if (twi_probe(addr)) {
                uart_print("Dispositivo en 0x");
                uart_print_hex8(addr);
                uart_print("\r\n");
                encontrados++;
            }
        }

        if (encontrados == 0)
            uart_print("Bus OK pero nadie respondio (modulo danado o sin alimentar)...\r\n");
        else {
            uart_print("Total: ");
            uart_print_i16(encontrados);
            uart_print("\r\n");
        }

        _delay_ms(2000);
    }

    return 0;
}
