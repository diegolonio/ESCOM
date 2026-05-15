# Práctica 5 — Detector de Anomalías con Perceptrón

## ¿Qué hace?

Se entrena un modelo de regresión lineal (perceptrón de una capa) en Python con datos reales capturados del Arduino, y luego se despliega ese modelo directamente en el ATmega328P. El micro predice el siguiente valor de la señal y si el error de predicción supera un umbral, enciende un LED de advertencia o alerta.

## Archivos

| Archivo | Descripción |
|---|---|
| `generador.c` | Genera señales periódicas (senoidal, cuadrada, etc.) via DAC I2C |
| `entrenamiento_modelo.py` | Captura datos y entrena el modelo en Python |
| `main.c` | Detector de anomalías corriendo en el ATmega con pesos fijos |
| `osciloscopio.py` | Visualizador en tiempo real de la señal |
| `visualizador.py` | Visualizador con la predicción del modelo superpuesta |

---

## Flujo del proyecto

```
generador.c  →  señal analógica  →  entrenamiento_modelo.py  →  pesos W[]
                                                                     ↓
                                                               main.c (AVR)
```

### 1. Generador de señales (`generador.c`)

Un segundo Arduino genera señales periódicas a 10Hz usando un DAC externo **MCP4725** por I2C (400kHz). Soporta: senoidal, cuadrada, sierra y triangular. La señal sale al canal analógico del Arduino detector.

### 2. Entrenamiento (`entrenamiento_modelo.py`)

- Se conecta al Arduino por serial (115200 baud) y captura muestras durante 40 segundos
- Construye ventanas deslizantes de tamaño N=25 muestras
- Entrena un `LinearRegression` de scikit-learn (perceptrón sin capa oculta)
- Calcula el umbral de error como: `media + 3 * desviación_estándar`
- Imprime los pesos W[], el bias y el umbral para copiarlos al firmware

```bash
pip install pyserial scikit-learn numpy
python3 entrenamiento_modelo.py
```

### 3. Detector en el AVR (`main.c`)

Los pesos entrenados se graban directo en el código como constantes en **PROGMEM** (memoria de programa) para no gastar RAM:

```c
const float W[N] PROGMEM = { -0.071289f, 0.174843f, ... };
const float BIAS      = 1393.227666f;
const float THRESHOLD = 14.1507f;
```

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

El muestreo está sincronizado con el **Timer1** (CTC mode, OCR1A=999, prescaler 64) para un período fijo de **4ms por muestra** sin usar delays bloqueantes.

### 4. Visualizadores

**`osciloscopio.py`** — muestra solo la señal cruda en tiempo real con matplotlib animado.

**`visualizador.py`** — muestra la señal real junto con la predicción del modelo para ver qué tan bien sigue la señal normal y cuándo diverge.

```bash
python3 osciloscopio.py
python3 visualizador.py
```

---

## Hardware

- **Arduino 1 (generador):** genera la señal con MCP4725 por I2C
- **Arduino 2 (detector):** ATmega328P corriendo `main.c`, lee la señal en A0
- Comunicación serial a **115200 baud**
- 3 LEDs en PD2, PD3, PD4
