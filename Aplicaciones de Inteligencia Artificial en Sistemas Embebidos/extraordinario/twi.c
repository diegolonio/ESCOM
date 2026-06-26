#include <avr/io.h>
#include "twi.h"

#define TWSR_MASK   0xF8
#define TWI_TIMEOUT 20000U // iteraciones antes de rendirse

/* Espera a que TWINT suba, pero con limite. Devuelve 1 si OK, 0 si expiró */
static uint8_t twi_wait(void) {
    uint16_t t = TWI_TIMEOUT;
    while (!(TWCR & (1 << TWINT))) {
        if (--t == 0) return 0; // el bus no respondió
    }
    return 1;
}

/* SCL = F_CPU/(16 + 2*TWBR); TWBR=72 -> ~100 kHz @ 16 MHz */
void twi_init(void) {
    TWSR = 0x00; // prescaler = 1
    TWBR = 72;
    TWCR = (1 << TWEN);
}

/* Devuelve TWSR enmascarado, o 0xFF si hubo timeout (bus colgado).
 * 0xFF nunca es un status valido porque estos terminan en 000 binario. */
uint8_t twi_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    if (!twi_wait()) return 0xFF;
    return TWSR & TWSR_MASK;
}

void twi_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
    uint16_t t = TWI_TIMEOUT;
    while ((TWCR & (1 << TWSTO)) && --t); // con limite, para no colgar aqui tampoco
}

uint8_t twi_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    if (!twi_wait()) return 0xFF;
    return TWSR & TWSR_MASK;
}

uint8_t twi_read_ack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN) | (1 << TWEA);
    twi_wait();
    return TWDR;
}

uint8_t twi_read_nack(void) {
    TWCR = (1 << TWINT) | (1 << TWEN);
    twi_wait();
    return TWDR;
}

/* Sondea una direccion: 1 si responde con ACK, 0 si no (o si el bus colgo). */
uint8_t twi_probe(uint8_t addr7) {
    uint8_t st = twi_start();
    if (st == 0xFF) { twi_stop(); return 0; }
    st = twi_write((uint8_t)((addr7 << 1) | 0)); // SLA+W
    twi_stop();
    return (st == 0x18); // 0x18 = SLA+W, ACK recibido
}
