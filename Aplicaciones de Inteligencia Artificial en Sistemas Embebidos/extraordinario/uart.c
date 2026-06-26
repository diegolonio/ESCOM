#include <avr/io.h>
#include "uart.h"

/*
 * 115200 baud @ 16 MHz usando doble velocidad (U2X0).
 *   UBRR = F_CPU/(8*BAUD) - 1 = 16e6/(8*115200) - 1 ≈ 16
 *   Baud real = 117647  ->  error +2.1% (dentro de tolerancia UART).
 */
void uart_init(void) {
    UBRR0H = 0;
    UBRR0L = 16;
    UCSR0A = (1 << U2X0); // doble velocidad
    UCSR0B = (1 << TXEN0) | (1 << RXEN0); // habilita TX y RX
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);  // 8 datos, 1 stop, sin paridad
}

void uart_tx(char c) {
    while (!(UCSR0A & (1 << UDRE0))); // espera buffer de TX libre
    UDR0 = c;
}

void uart_print(const char *s) {
    while (*s) uart_tx(*s++);
}

void uart_print_i16(int16_t v) {
    char tmp[6];
    int8_t i = 0;
    uint16_t u;

    if (v < 0) { uart_tx('-'); u = (uint16_t)(-v); }
    else       { u = (uint16_t)v; }

    if (u == 0) { uart_tx('0'); return; }

    while (u) { tmp[i++] = (char)('0' + (u % 10)); u /= 10; }
    while (i) uart_tx(tmp[--i]);
}

void uart_print_hex8(uint8_t v) {
    const char *h = "0123456789ABCDEF";
    uart_tx(h[(v >> 4) & 0x0F]);
    uart_tx(h[v & 0x0F]);
}
