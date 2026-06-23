#define F_CPU 16000000UL
#include <avr/io.h>
#include <math.h>
#include <avr/pgmspace.h>

#define N 25
const float W[N] PROGMEM = {
      0.749861f,
     -0.039724f,
     -0.035499f,
     -0.031663f,
      0.003203f,
     -0.022164f,
      0.021385f,
      0.022885f,
     -0.011997f,
      0.030757f,
     -0.005621f,
     -0.031893f,
      0.007769f,
     -0.104404f,
     -0.034948f,
     -0.054194f,
     -0.046131f,
     -0.023999f,
     -0.037935f,
      0.010280f,
      0.023988f,
      0.030936f,
      0.054829f,
      0.031216f,
      0.020850f
};
const float BIAS = 239.667968f;
const float THRESHOLD = 3.5884f;

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

// Promedio de 8 muestras (descarta la primera por asentamiento del S/H)
uint16_t adc_read(void) {
    ADCSRA |= (1 << ADSC); while (ADCSRA & (1 << ADSC)); // descarte
    uint32_t acc = 0;
    for (uint8_t k = 0; k < 8; k++) {
        ADCSRA |= (1 << ADSC); while (ADCSRA & (1 << ADSC));
        acc += ADC;
    }
    return (uint16_t)(acc >> 3);
}

// -------------------- Configuración Timer1 --------------------
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
    led_init();
    timer1_init(); // encender reloj

    led_set_state(NORMAL);

    for (uint8_t i = 0; i < N; i++) {
        window[i] = (float)adc_read();
        wait_for_timer();
    }

    while (1) {
        float y_hat = BIAS;
        for (uint8_t i = 0; i < N; i++) {
            uint8_t pos = (idx + i) % N;
            y_hat += pgm_read_float(&W[i]) * window[pos];
        }

        float y_real = (float)adc_read();
        float error = fabs(y_real - y_hat);

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

        window[idx] = y_real;
        idx = (idx + 1) % N;

        wait_for_timer();
    }
    
    return 0;
}
