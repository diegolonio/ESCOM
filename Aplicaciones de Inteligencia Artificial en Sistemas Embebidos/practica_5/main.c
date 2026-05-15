#define F_CPU 16000000UL
#include <avr/io.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <avr/pgmspace.h>

#define N 25
const float W[N] PROGMEM = {
     -0.071289f,
      0.174843f,
     -0.070229f,
     -0.081458f,
     -0.097000f,
     -0.121959f,
     -0.077191f,
     -0.052925f,
     -0.327065f,
     -0.039282f,
     -0.054737f,
     -0.406582f,
     -0.065124f,
     -0.017902f,
     -0.041104f,
     -0.101886f,
     -0.028564f,
     -0.060785f,
      0.082030f,
     -0.104902f,
     -0.077841f,
     -0.013471f,
      0.093754f,
     -0.100524f,
     -0.088673f
};
const float BIAS = 1393.227666f;
const float THRESHOLD = 14.1507f;

#define CONSEC_REQ 3
#define PIN_VERDE PD2
#define PIN_AMARILLO PD3
#define PIN_AZUL PD4

void led_init(void) { DDRD |= (1<<PIN_VERDE)|(1<<PIN_AMARILLO)|(1<<PIN_AZUL); }
typedef enum { NORMAL, WARNING, ANOMALY } Estado;
void led_set_state(Estado estado) {
    PORTD &= ~((1<<PIN_VERDE)|(1<<PIN_AMARILLO)|(1<<PIN_AZUL));
    if (estado == NORMAL) PORTD |= (1<<PIN_VERDE);
    else if (estado == WARNING) PORTD |= (1<<PIN_AMARILLO);
    else if (estado == ANOMALY) PORTD |= (1<<PIN_AZUL);
}

void adc_init(void) {
    ADMUX  = (1 << REFS0);
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0);
}
uint16_t adc_read(void) {
    ADCSRA |= (1 << ADSC);
    while (ADCSRA & (1 << ADSC));
    return ADC;
}

#define BAUD 115200
#define UBRR_VAL ((F_CPU / 8 / BAUD) - 1)
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
void uart_print(const char *s) { while (*s) uart_tx(*s++); }
void uart_print_float(float v) {
    char buf[10];
    dtostrf(v, 6, 2, buf); 
    uart_print(buf);
}

// ── CONFIGURACIÓN DEL RELOJ ──
void timer1_init(void) {
    TCCR1B = (1 << WGM12) | (1 << CS11) | (1 << CS10);
    OCR1A = 999;
}
void wait_for_timer(void) {
    while (!(TIFR1 & (1 << OCF1A)));
    TIFR1 = (1 << OCF1A);
}

float window[N] = {0};
uint8_t idx = 0;
int consec = 0;

int main(void) {
    adc_init();
    uart_init();
    led_init();
    timer1_init(); // Encender reloj

    led_set_state(NORMAL);

    for (uint8_t i = 0; i < N; i++) {
        window[i] = (float)adc_read();
        wait_for_timer(); // Espera perfecta
    }

    int print_counter = 0;

    while (1) {
        float y_hat = BIAS;
        for (uint8_t i = 0; i < N; i++) {
            uint8_t pos = (idx + i) % N;
            y_hat += pgm_read_float(&W[i]) * window[pos]; 
        }

        float y_real = (float)adc_read();
        float error = fabs(y_real - y_hat);
        float accuracy = 100.0f - ((error / 1023.0f) * 100.0f);
        if (accuracy < 0.0f) accuracy = 0.0f; 

        Estado estado_actual = NORMAL;
        if (error > THRESHOLD) {
            consec++;
            if (consec >= CONSEC_REQ) estado_actual = ANOMALY;
            else estado_actual = WARNING;
        } else {
            consec = 0;
            estado_actual = NORMAL;
        }
        led_set_state(estado_actual);

        // Imprimir cada 50 ciclos para no bloquear el sistema demasiado tiempo
        print_counter++;
        if (print_counter >= 50) {
            print_counter = 0;
            uart_print("Real: ");      uart_print_float(y_real);
            uart_print(" | Esp: ");    uart_print_float(y_hat);
            uart_print(" | Err: ");    uart_print_float(error);
            uart_print(" | Acc: ");    uart_print_float(accuracy);
            uart_print("\r\n");
        }

        window[idx] = y_real;
        idx = (idx + 1) % N;

        wait_for_timer();
    }
    return 0;
}
