# Práctica 2 — Goniómetro digital con ADC y USART

## ¿Qué hace?

Usando la calibración obtenida en la práctica 1, el potenciómetro funciona ahora como un **goniómetro**: un sensor que mide ángulos. El ATmega328P lee el voltaje del potenciómetro y lo reporta por USART cada 500 ms, sirviendo como la etapa de adquisición del goniómetro digital.

## ¿Cómo funciona?

### USART
Configurado a 9600 baudios (`UBRR = 103`). Se redirige `stdout` al USART para usar `printf` sin librerías externas.

### ADC
- Referencia de voltaje: 5V (bit REFS0 en ADMUX)
- Prescaler de 128 → frecuencia del ADC = 16MHz / 128 = **125 kHz**
- Conversión del resultado de 10 bits a milivoltios:

```
mV = (ADC * 5000) / 1023
```

El voltaje medido corresponde directamente a una posición angular del potenciómetro, que con la función lineal de la práctica 1 se traduce a grados. La parte entera y decimal se imprimen por separado con `%lu` para evitar problemas con `%f` en avr-libc.

## Salida esperada

```
Voltaje: 0.512 V
Voltaje: 1.625 V
```

## Hardware

- Microcontrolador: **ATmega328P** (Arduino UNO / Nano)
- Potenciómetro (goniómetro) conectado a **A0**
- Comunicación serial a **9600 baud**
