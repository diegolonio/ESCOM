#ifndef UART_H
#define UART_H

#include <stdint.h>

void uart_init(void);
void uart_tx(char c);
void uart_print(const char *s);
void uart_print_i16(int16_t v); // entero con signo en decimal
void uart_print_hex8(uint8_t v); // byte en hex de 2 digitos

#endif // UART_H
