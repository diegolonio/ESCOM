# Extraordinario — Detector de Caídas y Golpes con Regresión Logística

## ¿Qué hace?

Se entrena un clasificador de **regresión logística multinomial** en Python con datos reales de un acelerómetro **MPU6050**, y luego se despliega ese modelo directamente en el ATmega328P. El micro lee la aceleración a 50 Hz, calcula features sobre una ventana de 1 segundo y clasifica el movimiento en una de tres clases, encendiendo un LED distinto para cada una:

- **Pasivo** (LED verde) — reposo o movimiento normal
- **Golpe** (LED amarillo) — un pico de aceleración aislado
- **Caída** (LED azul) — caída libre (valle ~0g) seguida de un impacto

La diferencia clave entre golpe y caída es el **tiempo en valle**: una caída tiene un vuelo sostenido cerca de 0g antes del impacto, mientras que un golpe es solo un pico.

## Archivos

Los `.c` son firmware para AVR (ATmega328P); los `.py` corren en la PC.

| Archivo | Lado | Descripción |
|---|---|---|
| `escaner_i2c.c` | AVR | Escáner I2C: verifica el estado del bus y lista direcciones de dispositivos |
| `captura.c` | AVR | Lee el MPU6050 a 50 Hz y vuelca `ax,ay,az` por serial en formato CSV |
| `entrenar.py` | PC | Entrena la regresión logística e imprime la matriz de confusión, la precisión y los valores del modelo |
| `model.h` | — | Constantes del modelo (`MU`, `SD`, `W`, `B`, umbrales) usadas por la inferencia |
| `inferencia.c` | AVR | Clasificador en tiempo real con confirmación de golpe (descarta el tirón al soltar el sensor) |
| `inferencia_.c` | AVR | Versión más simple del clasificador, sin ventana de confirmación |
| `twi.c` / `twi.h` | AVR | Driver del bus I2C (TWI) por sondeo |
| `uart.c` / `uart.h` | AVR | Driver del UART (115200 baud) |
| `caidas.csv` | datos | Capturas de caídas |
| `golpes.csv` | datos | Capturas de golpes |
| `reposo.csv` | datos | Capturas de movimiento pasivo / reposo |
| `Makefile` | — | Compila y graba cada firmware con `avr-gcc` / `avrdude` |

---

## Flujo del proyecto

```
escaner_i2c.c (AVR) ──→ confirma que el MPU6050 responde en 0x68
        │
        ▼
captura.c (AVR) ──→ serial CSV ──→ caidas.csv / golpes.csv / reposo.csv
                                          │
                                          ▼
                              entrenar.py (PC) ──→ MU, SD, W[], B[]
                                          │
                                          ▼
                              model.h ──→ inferencia.c (AVR) ──→ LEDs
```

### 1. Escáner I2C (`escaner_i2c.c`)

Antes de nada, verifica el cableado. Reporta por serial el nivel de reposo de las líneas SDA (A4/PC4) y SCL (A5/PC5), detecta un bus colgado y recorre las direcciones 1–126 buscando dispositivos. El MPU6050 debe aparecer en `0x68`.

### 2. Captura de datos (`captura.c`)

Inicializa el MPU6050 (lo despierta y lo configura a ±4g) y, tras un parpadeo de 3 s del LED interno (PB5) para preparar el gesto, vuelca las lecturas crudas del acelerómetro por serial sincronizadas a **50 Hz exactos** con el Timer1 (CTC, prescaler 8, `OCR1A=39999`). La salida es CSV (`ax,ay,az`) lista para guardarse en un archivo.

Se graba una vez por cada clase y se registra la salida del serial directamente a un archivo con `tio`:

```bash
tio /dev/ttyUSB0 -b 115200 --log --log-file caidas.csv
```

Repite el volcado cambiando el nombre del archivo (`golpes.csv`, `reposo.csv`) para cada clase. Cada clase se capturó durante **2 minutos**:

- **Pasivo** (`reposo.csv`): la protoboard se sostenía en la mano y se cambiaba de posición cada tanto, incluyendo dejarla posada en la mesa.
- **Golpe** (`golpes.csv`): se golpeaba la protoboard con diferentes intensidades y en diferentes partes.
- **Caída** (`caidas.csv`): se dejaba caer la protoboard desde distintas alturas y en diferentes posiciones.

### 3. Entrenamiento (`entrenar.py`)

- Lee los tres CSV de forma robusta (ignora basura del reset serial y cabeceras coladas).
- Calcula la **magnitud** del vector de aceleración `√(ax²+ay²+az²)` y la segmenta en ventanas de **50 muestras (1 s)**.
- Sobre cada ventana extrae **5 features**: media, desviación estándar, mínimo, máximo y *tiempo en valle* (número de muestras por debajo de `TH_VALLE`).
- Entrena un `LogisticRegression` de scikit-learn con normalización (estandarización) de las features.
- Evalúa con un hold-out del 30% e imprime la **matriz de confusión** y el **porcentaje de precisión**.
- Imprime los valores del modelo final (`MU`, `SD`, `W`, `B` y los umbrales) para copiarlos a `model.h`.

```bash
pip install scikit-learn numpy pandas
python3 entrenar.py
```

### 4. El modelo (`model.h`)

La regresión logística se reduce a un puñado de constantes: las medias/desviaciones para normalizar (`MU`, `SD`), la matriz de pesos por clase (`W`), los sesgos (`B`) y los parámetros del preprocesamiento (`VENTANA`, `UMBRAL_VALLE`). Estos valores se copian a `model.h` desde la salida de `entrenar.py`.

```c
#define N_CLASES 3
#define N_FEATURES 5
#define VENTANA 50
#define UMBRAL_VALLE 3475L
static const float MU[N_FEATURES] = { ... };
static const float SD[N_FEATURES] = { ... };
static const float W[N_CLASES][N_FEATURES] = { ... };
static const float B[N_CLASES] = { ... };
```

> Los valores de `model.h` cambian con cada entrenamiento; los del repo son los actuales.

### 5. Inferencia en el AVR (`inferencia.c` / `inferencia_.c`)

El micro mantiene una ventana circular de 50 magnitudes. En cada **pico** (magnitud > `TH_PICO`) recalcula las 5 features, las normaliza con `MU`/`SD` y evalúa el modelo:

```c
float logit = B[k];
for (uint8_t i = 0; i < N_FEATURES; i++)
    logit += W[k][i] * ((f[i] - MU[i]) / SD[i]);
// clase = argmax de logit
```

La clase detectada enciende su LED durante 1.5 s (`HOLD_TICKS`). La caída tiene prioridad sobre el golpe.

| LED | Pin | Clase |
|---|---|---|
| Verde | PD5 | Pasivo |
| Amarillo | PD6 | Golpe |
| Azul | PD7 | Caída |

- **`inferencia_.c`** — versión directa: en cada pico clasifica y enciende el LED correspondiente.
- **`inferencia.c`** — añade una **ventana de confirmación** del golpe: tras un posible golpe espera 0.2 s y, si detecta caída libre (varias muestras en valle), lo descarta como el "tirón" de soltar el sensor en lugar de marcarlo como golpe.

---

## Compilar y grabar

El `Makefile` compila cada firmware por separado. Ajusta `PORT` al puerto de tu Arduino.

```bash
make escaner      && make flash-escaner       # escáner I2C
make captura      && make flash-captura        # captura de datos
make inferencia   && make flash-inferencia     # clasificador
make clean                                     # borra *.elf y *.hex
```

## Hardware

- **Arduino / ATmega328P** a 16 MHz
- **MPU6050** por I2C en `0x68` (SDA=A4, SCL=A5), configurado a ±4g
- 3 LEDs en PD5 (verde), PD6 (amarillo), PD7 (azul)
- Comunicación serial a **115200 baud**
