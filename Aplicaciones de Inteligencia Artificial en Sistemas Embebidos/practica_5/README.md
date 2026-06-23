# Práctica 5 — Detector de Anomalías con Perceptrón

## ¿Qué hace?

Se entrena un modelo de regresión lineal (perceptrón de una capa) en Python con datos reales capturados del Arduino, y luego se despliega ese modelo directamente en el ATmega328P. El micro predice el siguiente valor de la señal y, si el error de predicción supera un umbral, enciende un LED de advertencia o de alerta.

El sistema usa **dos Arduinos**: uno genera una señal periódica (con anomalías inyectadas de forma controlada) y el otro la lee, predice y clasifica.

## Archivos

Los `.c` son firmware para AVR (ATmega328P); los `.py` corren en la PC.

| Archivo | Lado | Descripción |
|---|---|---|
| `segnales.py` | PC | Genera las tablas de onda (`PROGMEM`) que se pegan en `generador.c` |
| `generador.c` | AVR | Genera señales periódicas vía DAC I2C e inyecta anomalías controladas |
| `entrenamiento_modelo.py` | PC | Captura datos por serial y entrena el modelo |
| `main.c` | AVR | Detector de anomalías con pesos fijos (solo LEDs, sin serial) |
| `visualizador.c` | AVR | Detector con LEDs **+** telemetría por serial para Python |
| `osciloscopio.c` | AVR | Streamea la señal cruda del ADC por serial (sin modelo) |
| `osciloscopio.py` | PC | Osciloscopio en tiempo real de la señal cruda |
| `visualizador.py` | PC | Grafica señal real vs. predicción y el error vs. umbral |

---

## Flujo del proyecto

```
segnales.py ─→ tablas PROGMEM ─→ generador.c (AVR 1) ─→ señal analógica
                                                              │
                                                              ▼
                                       entrenamiento_modelo.py (PC) ─→ pesos W[], BIAS, THRESHOLD
                                                              │
                                                              ▼
                                              main.c / visualizador.c (AVR 2)
```

### 1. Tablas de onda (`segnales.py`)

Script de PC que precalcula 500 puntos de cada onda (senoidal, cuadrada, sierra, triangular) en el rango del DAC (0–4095) y los imprime como arreglos `const uint16_t ... PROGMEM` listos para copiar a `generador.c`.

```bash
python3 segnales.py
```

### 2. Generador de señales (`generador.c`)

Un Arduino genera señales periódicas usando un DAC externo **MCP4725** por I2C (400kHz). Las muestras provienen de las tablas en `PROGMEM` y se actualizan cada **200µs** vía Timer1 (CTC, prescaler 8, `OCR1A=399`), por lo que un ciclo completo de 500 puntos dura **100ms (10Hz)**. El LED interno (PB5) parpadea al completar cada ciclo.

**Inyección de anomalías (`CAOS`):** cuando está activo, de forma aleatoria el generador suma una desviación grande (±1200 cuentas) a la señal para simular fallas:

- **Perturbación corta** (25 puntos) → debe producir *warning* (LED amarillo) en el detector
- **Perturbación larga** (100 puntos) → debe producir *anomalía* (LED azul)

Tras cada perturbación se fuerza un periodo de "enfriamiento" con señal limpia para que el detector pueda recuperarse.

### 3. Entrenamiento (`entrenamiento_modelo.py`)

- Se conecta al Arduino por serial (115200 baud) y captura muestras durante 40 segundos
- Construye ventanas deslizantes de tamaño N=25 muestras
- Entrena un `LinearRegression` de scikit-learn (perceptrón sin capa oculta)
- Calcula el umbral por encima del peor error observado en la señal limpia para evitar falsos positivos:

  ```python
  threshold = max(errors.max() * 1.3, errors.mean() + 5 * errors.std())
  ```

- Imprime los pesos W[], el bias y el umbral para copiarlos al firmware

```bash
pip install pyserial scikit-learn numpy
python3 entrenamiento_modelo.py
```

### 4. Detector en el AVR (`main.c` y `visualizador.c`)

Los pesos entrenados se graban directo en el código como constantes en **PROGMEM** (memoria de programa) para no gastar RAM:

```c
const float W[N] PROGMEM = { 0.749861f, -0.039724f, ... };
const float BIAS      = 239.667968f;
const float THRESHOLD = 3.5884f;
```

> Los valores de `W[]`, `BIAS` y `THRESHOLD` cambian con cada entrenamiento; los de arriba son los actuales de `main.c`.

El modelo se evalúa cada ciclo con una ventana circular:

```c
float y_hat = BIAS;
for (uint8_t i = 0; i < N; i++) {
    uint8_t pos = (idx + i) % N;
    y_hat += pgm_read_float(&W[i]) * window[pos];
}
```

Si el error supera el umbral por 3 ciclos consecutivos (`CONSEC_REQ`), se declara anomalía.

**Estados de los LEDs:**

| LED | Pin | Estado |
|---|---|---|
| Verde | PD2 | Normal |
| Amarillo | PD3 | Warning (1-2 errores seguidos) |
| Azul | PD4 | Anomalía (3+ errores seguidos) |

El muestreo está sincronizado con el **Timer1** (CTC, OCR1A=999, prescaler 64) a **250Hz (4ms por muestra)** sin usar delays bloqueantes.

- **`main.c`** — versión mínima: solo enciende los LEDs según el estado.
- **`visualizador.c`** — además envía por serial 4 columnas separadas por TAB (`y_real`, `y_hat`, `error`, `flag`) para alimentar a `visualizador.py`.

### 5. Visualizadores (PC)

Para que estos scripts reciban datos, el AVR debe correr el firmware correspondiente:

| Firmware (AVR) | Script (PC) | Qué muestra |
|---|---|---|
| `osciloscopio.c` | `osciloscopio.py` | Señal cruda del ADC en tiempo real |
| `visualizador.c` | `visualizador.py` | Señal real vs. predicción + error vs. umbral, con la anomalía resaltada |

```bash
python3 osciloscopio.py
python3 visualizador.py
```

---

## Hardware

- **Arduino 1 (generador):** genera la señal con MCP4725 por I2C e inyecta anomalías
- **Arduino 2 (detector):** ATmega328P corriendo `main.c` o `visualizador.c`, lee la señal en A0
- Comunicación serial a **115200 baud**
- 3 LEDs en PD2 (verde), PD3 (amarillo), PD4 (azul)
