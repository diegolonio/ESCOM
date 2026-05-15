import serial
import numpy as np
from sklearn.linear_model import LinearRegression
import time

# ── Configuración ──────────────────────────────────────────
PORT = '/dev/ttyUSB0' # puerto del Nano
BAUD = 115200
DURACION = 40 # segundos de captura
N = 25 # tamaño de ventana del perceptrón

# ── Captura de datos ───────────────────────────────────────
print(f"Conectando a puerto {PORT}...")

try:
    ser = serial.Serial(PORT, BAUD, timeout=2)
except serial.SerialException as e:
    print(f"Error: {e}")
    exit(1)

# Esperar a que el Arduino se estabilice
time.sleep(2)
ser.reset_input_buffer()

print(f"Capturando muestras de la señal...")

muestras = []
t_inicio = time.time()

while (time.time() - t_inicio) < DURACION:
    try:
        linea = ser.readline().decode('utf-8', errors='ignore').strip()
        if not linea:
            continue
        valor = float(linea.split('\t')[0])
        muestras.append(valor)

        # Mostrar progreso cada segundo
        transcurrido = time.time() - t_inicio
        if len(muestras) % 1000 == 0:
            print(f"  {transcurrido:.1f}s — {len(muestras)} muestras capturadas")

    except (ValueError, UnicodeDecodeError):
        continue

ser.close()

muestras = np.array(muestras)
print(f"\nCaptura completa: {len(muestras)} muestras")
print(f"  Rango: {muestras.min():.1f} — {muestras.max():.1f}")
print(f"  Media: {muestras.mean():.1f}")
print(f"  Desv. estándar: {muestras.std():.1f}")

# ── Construir ventanas deslizantes ─────────────────────────
print(f"\nConstruyendo ventanas de tamaño N={N}...")

X, y = [], []
for i in range(N, len(muestras)):
    X.append(muestras[i-N:i])
    y.append(muestras[i])

X = np.array(X)
y = np.array(y)

# ── Entrenar el perceptrón ─────────────────────────────────
print("\nEntrenando el modelo...")

model = LinearRegression()
model.fit(X, y)

print(f"  R²: {model.score(X, y):.6f}")

# ── Calcular umbral ────────────────────────────────────────
y_pred = model.predict(X)
errors = np.abs(y - y_pred)

print(f"\nEstadísticas del error en señal:")
print(f"  Error medio:    {errors.mean():.3f}")
print(f"  Error máximo:   {errors.max():.3f}")
print(f"  Desv. estándar: {errors.std():.3f}")

threshold = errors.mean() + 3 * errors.std()
print(f"  Umbral: {threshold:.4f}")

# ── Imprimir pesos del modelo ──────────
print(f"Tamaño de ventana: {N} muestras")
print(f"Pesos del modelo:")
print("\n".join(f"{w:.6f}" for w in model.coef_))
print(f"Sezgo: {model.intercept_:.6f}")
print(f"Umbral de error: {threshold:.4f}")
