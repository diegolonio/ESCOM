#include <avr/io.h>
#include <util/delay.h>
#include <stdio.h>

void usart_enviar(char c)
{
    /*
    UDRE0 = 1 cuando el buffer de transmisión está vacío, es decir,
    UCSR0A = 0b00100000 que negándolo da 0, lo que termina el ciclo
    y nos permite colocar otro dato en el data register
    */
    while (!(UCSR0A & (1 << UDRE0)));
    UDR0 = c;
}

int usart_redirigir(char c, FILE *stream)
{
    usart_enviar(c);
    return 0;
}

static FILE usart_stdout = FDEV_SETUP_STREAM(usart_redirigir, NULL, _FDEV_SETUP_WRITE);

int main(void) {
    // ********** Configuración de USART ********** 
    // Baud rate con 9600 de baud 103 = (16000000 / (16 * 9600)) - 1
    UBRR0H = 0;
    UBRR0L = 103;
    UCSR0B = (1 << TXEN0); // Habilitar transmisión: 0b00001000
    UCSR0C = (1 << UCSZ01) | (1 << UCSZ00); // 0b00000110 8N1
    stdout = &usart_stdout; // printf -> USART

    // ********** Configuración del ADC **********
    ADMUX = (1 << REFS0); // 0b01000000
    /*
    Habilitamos el ADC y configuramos el factor de división con los 3 bits prendidos,
    es decir, 128 de modo que la frecuencia del ADC será 16MHz/128 = 125kHz.
    */
    ADCSRA = (1 << ADEN) | (1 << ADPS2) | (1 << ADPS1) | (1 << ADPS0); // 0b10000111

    // ********** Configuración de la señal de PWM ********** 
    // PD6 (OC0A) como salida
    DDRD |= (1 << PD6);

    // Fast PWM, non-inverting, prescaler 64
    TCCR0A = (1 << COM0A1) | (1 << WGM01) | (1 << WGM00);
    TCCR0B = (1 << CS01) | (1 << CS00);
    OCR0A = 0;

    
    while (1) {
        // Hacemos una conversión
        ADCSRA |= (1 << ADSC); // 0b11000111

        /* Esperamos a que termine la conversión con un while vacío,
        el bit ADSC se coloca en 0 cuando termina la conversión */
        while (ADCSRA & (1 << ADSC));

        uint32_t mv = ((uint32_t)ADC * 5000UL) / 1023UL;
        uint8_t escala = (uint8_t)((uint32_t)ADC * 255UL / 1023UL);
        OCR0A = 255 - escala;

        printf("Voltaje: %lu.%03lu V | Escalado: %u\r\n", mv / 1000, mv % 1000, 255 - escala);

        _delay_ms(50);
    }
}
