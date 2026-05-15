# Práctica 3 — Sensor de proximidad de luz con fotorresistencia y PWM

## ¿Qué hace?

Usa una fotorresistencia (LDR) conectada al ADC para controlar la intensidad de un LED mediante PWM. El comportamiento es inverso: cuando hay mucha luz cerca del sensor el LED se atenúa, y cuando hay poca luz el LED aumenta su brillo. También reporta los valores por USART para monitorearlos.

## ¿Cómo funciona?

### USART
Configurado a 9600 baudios (`UBRR = 103`), con `stdout` redirigido al USART.

### ADC — lectura de la fotorresistencia
- Referencia de 5V, prescaler de 128 → ADC a 125 kHz
- La fotorresistencia forma un divisor de voltaje con una resistencia fija. A más luz, menor resistencia → mayor voltaje en A0 → valor ADC más alto.

### Mapeo invertido al PWM
El valor ADC de 10 bits se escala a 8 bits y se invierte antes de escribirlo al registro de comparación:

```c
uint8_t escala = (uint8_t)((uint32_t)ADC * 255UL / 1023UL);
OCR0A = 255 - escala;
```

Esto produce el comportamiento deseado:

| Condición | ADC | OCR0A | LED |
|---|---|---|---|
| Mucha luz cerca | Alto (~1023) | Bajo (~0) | Apagado / tenue |
| Poca luz | Bajo (~0) | Alto (~255) | Máximo brillo |

### PWM — Timer0 en Fast PWM
- Pin de salida: **PD6 (OC0A)**
- Modo: Fast PWM, non-inverting (COM0A1)
- Prescaler: 64 → frecuencia del PWM ≈ **976 Hz**

El delay es de 50 ms para una respuesta más ágil al cambio de luz.

## Salida esperada

```
Voltaje: 3.200 V | Escalado: 91   ← mucha luz, LED tenue
Voltaje: 0.410 V | Escalado: 234  ← poca luz, LED brillante
```

## Hardware

- Microcontrolador: **ATmega328P** (Arduino UNO / Nano)
- Fotorresistencia (LDR) en divisor de voltaje conectado a **A0**
- LED conectado a **PD6 (pin 6)**
- Comunicación serial a **9600 baud**
