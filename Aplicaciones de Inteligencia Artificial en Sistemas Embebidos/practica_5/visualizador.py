# ═══════════════════════════════════════════════════════════
#  Visualizador del detector de anomalías (Actualizado 250Hz)
#  Lee 4 columnas: y_real, y_hat, error, flag
# ═══════════════════════════════════════════════════════════
import serial
import matplotlib.pyplot as plt
from matplotlib.animation import FuncAnimation
from collections import deque
import sys

PORT       = '/dev/ttyUSB0'
BAUD       = 115200
BUFFER_LEN = 500
THRESHOLD  = 14.1507

buf_real    = deque([0] * BUFFER_LEN, maxlen=BUFFER_LEN)
buf_hat     = deque([0] * BUFFER_LEN, maxlen=BUFFER_LEN)
buf_error   = deque([0] * BUFFER_LEN, maxlen=BUFFER_LEN)
buf_anomaly = deque([False] * BUFFER_LEN, maxlen=BUFFER_LEN)

try:
    ser = serial.Serial(PORT, BAUD, timeout=1)
    print(f"Conectado a {PORT}")
except serial.SerialException as e:
    print(f"Error: {e}")
    sys.exit(1)

fig, (ax1, ax2) = plt.subplots(2, 1, figsize=(12, 6), sharex=True)

# ── Gráfica superior: señal real vs predicción ─────────────
line_real, = ax1.plot([], [], 'b-',  lw=1.2, label='Señal real (ADC)')
line_hat,  = ax1.plot([], [], 'g--', lw=1.0, label='Predicción perceptrón')
ax1.set_ylim(-50, 1073)
ax1.set_xlim(0, BUFFER_LEN)
ax1.set_ylabel('Valor ADC')
ax1.set_title('Detector de anomalías con perceptrón lineal (Timer1 - 250Hz)')
ax1.legend(loc='upper right')
ax1.grid(True, alpha=0.3)

anomaly_fill = ax1.fill_between(
    range(BUFFER_LEN), -50, 1073,
    where=[False] * BUFFER_LEN,
    color='red', alpha=0.2, label='Anomalía'
)

# ── Gráfica inferior: error vs umbral ──────────────────────
line_error, = ax2.plot([], [], color='orange', lw=1.0, label='Error |y_real - y_hat|')
ax2.axhline(y=THRESHOLD, color='red', ls='--', lw=1.2, label=f'Umbral ({THRESHOLD})')
ax2.set_ylim(0, THRESHOLD * 5)
ax2.set_xlim(0, BUFFER_LEN)
ax2.set_ylabel('Error')
ax2.set_xlabel('Muestra')
ax2.legend(loc='upper right')
ax2.grid(True, alpha=0.3)

plt.tight_layout()

def update(frame):
    global anomaly_fill

    reads = 0
    while ser.in_waiting and reads < 50:
        try:
            linea = ser.readline().decode('utf-8', errors='ignore').strip()
            reads += 1
            if not linea:
                continue

            parts = linea.split('\t')
            if len(parts) != 4:
                continue

            buf_real.append(float(parts[0]))
            buf_hat.append(float(parts[1]))
            buf_error.append(float(parts[2]))
            buf_anomaly.append(int(parts[3]) > 0)

        except (ValueError, UnicodeDecodeError):
            continue

    x = list(range(BUFFER_LEN))
    line_real.set_data(x, list(buf_real))
    line_hat.set_data(x, list(buf_hat))
    line_error.set_data(x, list(buf_error))

    anomaly_fill.remove()
    anomaly_fill = ax1.fill_between(
        x, -50, 1073,
        where=list(buf_anomaly),
        color='red', alpha=0.2
    )

    return line_real, line_hat, line_error, anomaly_fill

ani = FuncAnimation(fig, update, interval=50, blit=False, cache_frame_data=False)

try:
    plt.show()
except KeyboardInterrupt:
    print("\nCerrando...")
finally:
    ser.close()
    print("Puerto cerrado.")
