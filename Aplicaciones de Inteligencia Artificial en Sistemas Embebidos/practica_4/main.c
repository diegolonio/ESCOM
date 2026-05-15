#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ============================================================
 * CALIBRACIÓN DE SERVOS
 * ============================================================ */

#define S1_MIN  1060
#define S1_MAX  4900
#define S1_RANGE (S1_MAX - S1_MIN)

#define S2_MIN  1035
#define S2_MAX  4900
#define S2_RANGE (S2_MAX - S2_MIN)

/* ============================================================
 * UART - 9600 baud
 * ============================================================ */

void uart_init(void) {
    uint16_t ubrr = F_CPU / 16 / 9600 - 1;
    UBRR0H = (uint8_t)(ubrr >> 8);
    UBRR0L = (uint8_t)(ubrr);
    UCSR0B = (1 << TXEN0) | (1 << RXEN0);  // TX y RX habilitados
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_putchar(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void uart_print(const char *str) {
    while (*str) uart_putchar(*str++);
}

void uart_println(const char *str) {
    uart_print(str);
    uart_print("\r\n");
}

char uart_getchar(void) {
    while (!(UCSR0A & (1 << RXC0)));
    return UDR0;
}

uint8_t uart_available(void) {
    return (UCSR0A & (1 << RXC0));
}

/* ============================================================
 * SERVOS - PWM con Timer1
 * ============================================================ */

void servo_init(void) {
    DDRB |= (1 << PB1) | (1 << PB2);
    TCCR1A = (1 << COM1A1) | (1 << COM1B1) | (1 << WGM11);
    TCCR1B = (1 << WGM13) | (1 << WGM12) | (1 << CS11);
    ICR1 = 39999;
}

void servo1_escribir(float angulo) {
    if (angulo < 0.0) angulo = 0.0;
    if (angulo > 180.0) angulo = 180.0;
    long pulso = S1_MIN + (long)(angulo * S1_RANGE / 180.0);
    OCR1A = (uint16_t)pulso;
}

void servo2_escribir(float angulo) {
    if (angulo < 0.0) angulo = 0.0;
    if (angulo > 180.0) angulo = 180.0;
    long pulso = S2_MIN + (long)(angulo * S2_RANGE / 180.0);
    OCR1B = (uint16_t)pulso;
}

/* ============================================================
 * MAIN
 * ============================================================ */

int main(void) {
    uart_init();
    servo_init();

    // Posición inicial: ambos servos a 90°
    servo1_escribir(90.0);
    servo2_escribir(90.0);

    uart_println("ROBOT_READY");

    char buf[32];
    uint8_t idx = 0;

    while (1) {
        if (uart_available()) {
            char c = uart_getchar();

            if (c == '\n' || c == '\r') {
                buf[idx] = '\0';

                if (idx > 0) {
                    // Handshake
                    if (strcmp(buf, "READY") == 0) {
                        uart_println("OK");
                    }
                    // Comando de servos: "S1:90.5 S2:120.3"
                    else if (buf[0] == 'S') {
                        char *s1_str = strstr(buf, "S1:");
                        char *s2_str = strstr(buf, "S2:");

                        if (s1_str && s2_str) {
                            float s1 = atof(s1_str + 3);
                            float s2 = atof(s2_str + 3);

                            servo1_escribir(s1);
                            servo2_escribir(s2);

                            uart_println("OK");
                        }
                    }
                }

                idx = 0;
            } else if (idx < 30) {
                buf[idx++] = c;
            }
        }
    }

    return 0;
}
