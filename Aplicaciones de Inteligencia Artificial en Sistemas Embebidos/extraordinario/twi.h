#ifndef TWI_H
#define TWI_H

#include <stdint.h>

void twi_init(void); // 100 kHz @ 16 MHz
uint8_t twi_start(void); // devuelve TWSR enmascarado
void twi_stop(void);
uint8_t twi_write(uint8_t data); // devuelve TWSR enmascarado
uint8_t twi_read_ack(void); // lee byte, responde ACK
uint8_t twi_read_nack(void); // lee byte, responde NACK
uint8_t twi_probe(uint8_t addr7); // 1 si la direccion responde, 0 si no

#endif // TWI_H
