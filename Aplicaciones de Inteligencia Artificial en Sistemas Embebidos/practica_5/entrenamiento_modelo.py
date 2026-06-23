import serial
import numpy as np
from sklearn.linear_model import LinearRegression
import time

# -------------------- Configuración --------------------
PORT = '/dev/ttyUSB0' # puerto del Arduino Nano
BAUD = 115200
DURACION = 40 # segundos de captura
N = 25 # tamaño de ventana del perceptrón

# -------------------- Captura de los datos --------------------
print(f"Conectando a {PORT} a {BAUD} baud...")

try:
    ser = serial.Serial(PORT, BAUD, timeout=2)
except serial.SerialException as e:
    print(f"Error: {e}")
    exit(1)

# Esperar a que el Arduino se estabilice
time.sleep(2)
ser.reset_input_buffer()

print(f"Capturando señal...")
print()

muestras = []
t_inicio = time.time()

while (time.time() - t_inicio) < DURACION:
    try:
        linea = ser.readline().decode('utf-8', errors='ignore').strip()
        if not linea:
            continue
        valor = float(linea.split('\t')[0])
        muestras.append(valor)
    except (ValueError, UnicodeDecodeError):
        continue

ser.close()

muestras = np.array(muestras)
print(f"\nCaptura completa: {len(muestras)} muestras en {DURACION}s")
print(f"  Rango: {muestras.min():.1f} — {muestras.max():.1f}")
print(f"  Media: {muestras.mean():.1f}")
print(f"  Desv. estándar: {muestras.std():.1f}")

# -------------------- Construir ventanas deslizantes --------------------
print(f"\nConstruyendo ventanas...")

X, y = [], []
for i in range(N, len(muestras)):
    X.append(muestras[i-N:i])
    y.append(muestras[i])

X = np.array(X)
y = np.array(y)

# -------------------- Entrenar perceptrón --------------------
print("\nEntrenamiento en proceso...")

model = LinearRegression()
model.fit(X, y)

print(f"  R²: {model.score(X, y):.6f}")

# -------------------- Calcular umbral --------------------
y_pred = model.predict(X)
errors = np.abs(y - y_pred)

print(f"\nEstadísticas del error en señal limpia:")
print(f"  Error medio:    {errors.mean():.3f}")
print(f"  Error máximo:   {errors.max():.3f}")
print(f"  Desv. estándar: {errors.std():.3f}")

# El umbral debe quedar por encima del peor error medido en la señal,
# de modo que se evitan falsos positivos. Para ello, tomamos el valor
# máximo obervado y añadimos un margen del 30%, para el piso consideramos
# media + 5 * desiaviación estándar
threshold = max(errors.max() * 1.3, errors.mean() + 5 * errors.std())
print(f"  Umbral: {threshold:.4f}")

# -------------------- Imprimir pesos --------------------
print(f"N = {N}\nPesos:\n")
print(",\n".join(f"{w:>12.6f}f" for w in model.coef_))
print(f"\nBias: {model.intercept_:.6f}")
print(f"\nUmbral: {threshold:.4f}")
