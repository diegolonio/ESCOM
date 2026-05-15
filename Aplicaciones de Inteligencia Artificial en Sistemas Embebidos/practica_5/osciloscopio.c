#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdio.h>

#define BAUD 115200
#define UBRR_VAL ((F_CPU / 8 / BAUD) - 1)

void adc_init(void) {
    ADMUX = (1 << REFS0); 
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); 
}

uint16_t adc_read(void) {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

void uart_init(void) {
    UBRR0H = (uint8_t)(UBRR_VAL >> 8);
    UBRR0L = (uint8_t)(UBRR_VAL);
    UCSR0A = (1 << U2X0); 
    UCSR0B = (1 << TXEN0);
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00);
}

void uart_tx(char c) {
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

void uart_print_int(int v) {
    char buf[8];
    snprintf(buf, sizeof(buf), "%d", v);
    char *s = buf;
    while (*s) uart_tx(*s++);
}

// ── CONFIGURACIÓN DEL RELOJ (TIMER1) ──
void timer1_init(void) {
    // Modo CTC, Prescaler de 64
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
    // Para 4ms exactos a 16MHz: (16M / (64 * 250Hz)) - 1 = 999
    OCR1A = 999;
}

void wait_for_timer(void) {
    while (!(TIFR1 & (1 << OCF1A))); // Espera a la bandera del reloj
    TIFR1 = (1 << OCF1A); // Limpia la bandera
}

int main(void) {
    adc_init();
    uart_init();
    timer1_init(); // Encendemos el reloj

    while (1) {
        uint16_t valor = adc_read();
        uart_print_int(valor);
        uart_tx('\r'); uart_tx('\n');
        
        wait_for_timer();
    }
    return 0;
}
