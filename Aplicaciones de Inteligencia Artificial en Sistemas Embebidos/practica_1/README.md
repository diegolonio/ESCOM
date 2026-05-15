# Práctica 1 — Graduación del rango de giro de un potenciómetro con ADC y USART

## ¿Qué hace?

Lee el voltaje de un potenciómetro conectado al ADC del ATmega328P y lo convierte a grados de giro mediante una función lineal calibrada. Los datos se mandan por USART cada 500 ms para verlos en el monitor serial.

## ¿Cómo funciona?

### USART
Se configura a 9600 baudios con la fórmula `UBRR = (16000000 / (16 * 9600)) - 1 = 103`. Se redirige `stdout` hacia el USART para poder usar `printf` directamente sin librerías externas.

### ADC
- Referencia de voltaje: 5V (bit REFS0 en ADMUX)
- Prescaler de 128 → frecuencia del ADC = 16MHz / 128 = **125 kHz**
- El resultado de 10 bits se convierte a milivoltios con:

```
mV = (ADC * 5000) / 1023
```

### Conversión a grados de giro
Con el voltaje se aplica la función lineal obtenida al calibrar el potenciómetro:

```
Grados = (Voltaje - 0.012376) / 0.01613
```

Donde `0.012376` es el voltaje mínimo medido (posición 0°) y `0.01613` es la sensibilidad en V/grado. Esto mapea el rango eléctrico del potenciómetro a su rango físico de giro.

Como `avr-libc` no siempre soporta bien `%f` en `printf`, se separan la parte entera y decimal manualmente para imprimirlas con `%ld`.

## Salida esperada

```
Voltaje: 0.512 V  |  Grados: 31.03°
Voltaje: 1.625 V  |  Grados: 99.51°
```

## Hardware

- Microcontrolador: **ATmega328P** (Arduino UNO / Nano)
- Potenciómetro conectado a **A0** (canal 0 del ADC)
- Comunicación serial a **9600 baud**
