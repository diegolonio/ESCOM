#include <avr/io.h>
#include <util/delay.h>
#include "uart.h"
#include "twi.h"

#define MPU 0x68

static void mpu_write(uint8_t reg, uint8_t data)
{
    twi_start();
    twi_write((MPU << 1) | 0);
    twi_write(reg);
    twi_write(data);
    twi_stop();
}

static void mpu_read(uint8_t reg, uint8_t *buf, uint8_t n)
{
    twi_start();
    twi_write((MPU << 1) | 0);
    twi_write(reg);
    twi_start();
    twi_write((MPU << 1) | 1);

    for (uint8_t i = 0; i < n; i++) {
        buf[i] = (i < n - 1) ? twi_read_ack() : twi_read_nack();
    }

    twi_stop();
}

/* 
 * Timer1 en CTC como base de 50 Hz.
 *   Tick = F_CPU / (prescaler * (OCR1A+1))
 *   50 Hz @ 16 MHz, prescaler 8:  OCR1A = 16e6/(8*50) - 1 = 39999
 * El flag OCF1A se levanta por hardware cada 20 ms aunque no
 * se habiliten interrupciones, asi que se sondea.
 */
static void timer1_50hz_init(void)
{
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11); // CTC, prescaler 8
    OCR1A  = 39999;
    TCNT1  = 0;
}

static void wait_tick(void)
{
    while (!(TIFR1 & (1 << OCF1A))) ; // espera al compare match

    TIFR1 = (1 << OCF1A);            // limpia el flag (se escribe 1)
}

int main(void) {
    uart_init();
    twi_init();

    mpu_write(0x6B, 0x00); // PWR_MGMT_1: despertar
    mpu_write(0x1C, 0x08); // ACCEL_CONFIG: +/-4g

    DDRB  |= (1 << PB5);
    PORTB |= (1 << PB5);
    _delay_ms(3000);
    PORTB &= ~(1 << PB5); // apaga: empieza el volcado de datos

    timer1_50hz_init();

    uart_print("ax,ay,az\r\n"); // cabecera CSV

    uint8_t b[6];
    while (1) {
        wait_tick(); // sincroniza a 50 Hz exactos

        mpu_read(0x3B, b, 6);
        int16_t ax = (int16_t)(((uint16_t)b[0] << 8) | b[1]);
        int16_t ay = (int16_t)(((uint16_t)b[2] << 8) | b[3]);
        int16_t az = (int16_t)(((uint16_t)b[4] << 8) | b[5]);

        uart_print_i16(ax);
        uart_tx(',');
        uart_print_i16(ay);
        uart_tx(',');
        uart_print_i16(az); 
        uart_print("\r\n");
    }

    return 0;
}
