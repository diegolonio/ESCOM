import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import deque
import sys

# ── Configuración ──────────────────────────────────────────
PORT = '/dev/ttyUSB0'
BAUD = 115200
BUFFER_LEN = 500
Y_MIN = 0
Y_MAX = 1023

# ── Buffer circular (solo para la señal real) ──────────────
buf_real = deque([0] * BUFFER_LEN, maxlen=BUFFER_LEN)

# ── Conectar al puerto serial ──────────────────────────────
try:
    ser = serial.Serial(PORT, BAUD, timeout=1)
    print(f"Conectado a {PORT} a {BAUD} baudios")
except serial.SerialException as e:
    print(f"Error al abrir {PORT}: {e}")
    sys.exit(1)

# ── Configurar la figura ───────────────────────────────────
fig, ax = plt.subplots(figsize=(10, 4))
line_real, = ax.plot([], [], 'b-', linewidth=1.5, label='Señal real (A0)')

ax.set_ylim(Y_MIN - 50, Y_MAX + 50)
ax.set_xlim(0, BUFFER_LEN)
ax.set_ylabel('Valor ADC (0-1023)')
ax.set_xlabel('Muestras')
ax.set_title('Visualizador de Señal Senoidal (50Hz)')
ax.legend(loc='upper right')
ax.grid(True, alpha=0.3)
plt.tight_layout()

def update(frame):
    reads = 0
    # Leemos máximo 100 líneas por ciclo para evitar que la interfaz se congele
    while ser.in_waiting and reads < 100:
        try:
            line = ser.readline().decode('utf-8', errors='ignore').strip()
            reads += 1
            if not line:
                continue

            # El Arduino UNO manda solo un número
            parts = line.split('\t')
            if len(parts) >= 1: 
                y_real = float(parts[0])
                buf_real.append(y_real)

        except (ValueError, UnicodeDecodeError):
            continue

    x = list(range(BUFFER_LEN))
    line_real.set_data(x, list(buf_real))
    return line_real,

# ── Iniciar animación ──────────────────────────────────────
ani = FuncAnimation(fig, update, interval=50, blit=False, cache_frame_data=False)

try:
    plt.show()
except KeyboardInterrupt:
    print("\nCerrando el visualizador...")
finally:
    ser.close()
    print("Puerto serial liberado y cerrado.")
