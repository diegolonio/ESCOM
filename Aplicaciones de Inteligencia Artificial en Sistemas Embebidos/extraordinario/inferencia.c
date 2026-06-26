#include <avr/io.h>
#include <util/delay.h>
#include <math.h>
#include "uart.h"
#include "twi.h"
#include "model.h"

#define MPU 0x68

/* LEDs en PORTD */
#define LED_VERDE PD5 // pasivo
#define LED_AMARILLO PD6 // golpe
#define LED_AZUL PD7 // caída

/* índices de clase: 0 = caida, 1 = golpe, 2 = pasivo */
#define CLASE_CAIDA 0
#define CLASE_GOLPE 1
#define CLASE_PASIVO 2

/* Umbral de impacto */
#define TH_PICO 11120L

#define HOLD_MS 1500
#define HOLD_TICKS (HOLD_MS / 20) // 75 ticks = 1.5s

/*
 * Tiempo de espera de 0.2s para descartar el tirón
 * inicial al soltar el sensor para que caiga que es
 * detectado como un golpe
 */
#define CONFIRM_TICKS 10
#define VALLE_MIN 3

/* ---------- MPU6050 ---------- */
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

static float leer_magnitud(void)
{
    uint8_t b[6];
    mpu_read(0x3B, b, 6);
    int16_t ax = (int16_t)(((uint16_t)b[0] << 8) | b[1]);
    int16_t ay = (int16_t)(((uint16_t)b[2] << 8) | b[3]);
    int16_t az = (int16_t)(((uint16_t)b[4] << 8) | b[5]);
    float fx = ax, fy = ay, fz = az;

    return sqrtf(fx*fx + fy*fy + fz*fz);
}

/* ---------- Timer1 a 50 Hz ---------- */
static void timer1_50hz_init(void)
{
    TCCR1A = 0;
    TCCR1B = (1 << WGM12) | (1 << CS11);
    OCR1A  = 39999;
    TCNT1  = 0;
}

static void wait_tick(void)
{
    while (!(TIFR1 & (1 << OCF1A))) ;

    TIFR1 = (1 << OCF1A);
}

/* ---------- Clasificacion ---------- */
static uint8_t clasificar(const float *buf)
{
    float media = 0, mn = 1e30f, mx = 0;
    uint8_t tvalle = 0;

    for (uint8_t i = 0; i < VENTANA; i++) {
        float v = buf[i];
        media += v;

        if (v < mn) {
            mn = v;
        }

        if (v > mx) {
            mx = v;
        }

        if (v < (float)UMBRAL_VALLE) {
            tvalle++;
        }
    }

    media /= VENTANA;

    float var = 0;

    for (uint8_t i = 0; i < VENTANA; i++) {
        float d = buf[i] - media;
        var += d * d;
    }

    float desv = sqrtf(var / VENTANA);

    float f[N_FEATURES] = {media, desv, mn, mx, (float)tvalle};

    int8_t arg = 0;
    float best = -1e30f;

    for (uint8_t k = 0; k < N_CLASES; k++) {
        float logit = B[k];

        for (uint8_t i = 0; i < N_FEATURES; i++) {
            logit += W[k][i] * ((f[i] - MU[i]) / SD[i]);
        }

        if (logit > best) {
            best = logit; arg = k;
        }
    }

    return (uint8_t)arg;
}

static void leds_off(void)
{
    PORTD &= ~((1 << LED_VERDE) | (1 << LED_AMARILLO) | (1 << LED_AZUL));
}

int main(void)
{
    uart_init();
    twi_init();
    mpu_write(0x6B, 0x00);
    mpu_write(0x1C, 0x08);

    DDRD |= (1 << LED_VERDE) | (1 << LED_AMARILLO) | (1 << LED_AZUL);
    leds_off();
    PORTD |= (1 << LED_VERDE);

    float buf[VENTANA];
    timer1_50hz_init();

    for (uint8_t i = 0; i < VENTANA; i++) {
        wait_tick(); 
        buf[i] = leer_magnitud();
    }

    uint8_t idx = 0;
    uint8_t hold = 0;
    uint8_t hold_clase = CLASE_PASIVO;
    uint8_t golpe_pend = 0; // ticks restantes para confirmar un golpe
    uint8_t valle_cnt  = 0; // muestras en valle durante la confirmacion

    while (1) {
        wait_tick();
        float m = leer_magnitud();
        buf[idx] = m;
        idx = (idx + 1) % VENTANA;

        /* En cada impacto clasificamos. */
        if (m > (float)TH_PICO) {
            uint8_t clase = clasificar(buf);

            if (clase == CLASE_CAIDA) {
                hold = HOLD_TICKS;
                hold_clase = CLASE_CAIDA;
                golpe_pend = 0;
            } else if (clase == CLASE_GOLPE) {
                if (!(hold > 0 && hold_clase == CLASE_CAIDA)) {
                    golpe_pend = CONFIRM_TICKS;
                    valle_cnt  = 0;
                }
            }
        }

        /* Ventana de confirmacion del golpe */
        if (golpe_pend > 0) {
            if (m < (float)UMBRAL_VALLE) {
                valle_cnt++;
            }

            if (valle_cnt >= VALLE_MIN) {
                golpe_pend = 0; // hubo caida libre: era gesto de soltar
            } else {
                golpe_pend--;

                if (golpe_pend == 0) { // sin caida libre: golpe confirmado
                    hold = HOLD_TICKS;
                    hold_clase = CLASE_GOLPE;
                }
            }
        }

        leds_off();

        if (hold > 0) {
            if (hold_clase == CLASE_CAIDA) {
                PORTD |= (1 << LED_AZUL);
            } else {
                PORTD |= (1 << LED_AMARILLO);
            }

            hold--;
        } else {
            PORTD |= (1 << LED_VERDE);
        }
    }

    return 0;
}
