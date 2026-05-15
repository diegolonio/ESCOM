#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <math.h>

#define MCP4725_ADDR 0x60
#define PI 3.1415926535

// Tipos de onda
typedef enum { SENOIDAL, CUADRADA, SIERRA, TRIANGULAR } TipoOnda;

// ── Configuración I2C ────────────────────────────────────────
void i2c_init(void) {
    PORTC |= (1 << PC4) | (1 << PC5); 
    TWSR = 0x00;
    TWBR = 12; // 400kHz
    TWCR = (1 << TWEN);
}

void i2c_start(void) {
    TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void i2c_stop(void) {
    TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);
}

void i2c_write(uint8_t data) {
    TWDR = data;
    TWCR = (1 << TWINT) | (1 << TWEN);
    while (!(TWCR & (1 << TWINT)));
}

void dac_write(uint16_t value) {
    uint8_t high = (uint8_t)((value >> 8) & 0x0F);
    uint8_t low  = (uint8_t)(value & 0xFF);
    i2c_start();
    i2c_write(MCP4725_ADDR << 1);
    i2c_write(high);
    i2c_write(low);
    i2c_stop();
}

// ── Lógica Principal ─────────────────────────────────────────
int main(void) {
    i2c_init();
    DDRB |= (1 << PB5); // LED Pin 13, enciende en cada cresta de la onda
    
    TipoOnda modoActual = SENOIDAL;
    
    float fase = 0;
    const float incremento = (2.0 * PI) / 100.0; // Basado en 10Hz con delay de 1ms
    uint16_t dac_val = 0;

    while (1) {
        switch (modoActual) {
            case SENOIDAL:
                dac_val = (uint16_t)((sin(fase) + 1.0) * 2047.5);
                break;

            case CUADRADA:
                dac_val = (fase < PI) ? 4095 : 0;
                break;

            case SIERRA:
                dac_val = (uint16_t)((fase / (2.0 * PI)) * 4095.0);
                break;

            case TRIANGULAR:
                if (fase < PI) {
                    dac_val = (uint16_t)((fase / PI) * 4095.0);
                } else {
                    dac_val = (uint16_t)(((2.0 * PI - fase) / PI) * 4095.0);
                }
                break;
        }

        dac_write(dac_val);

        fase += incremento;
        if (fase >= 2.0 * PI) {
            fase -= 2.0 * PI;
            PORTB ^= (1 << PB5);
        }

        _delay_ms(1); 
    }
    return 0;
}
