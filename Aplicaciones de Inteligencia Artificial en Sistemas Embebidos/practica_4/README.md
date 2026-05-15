# Práctica 4 — Robot Planar con 2 Grados de Libertad

## ¿Qué hace?

Control de un brazo robótico planar de 2 grados de libertad (2 servomotores). La PC calcula la cinemática inversa y manda los ángulos al microcontrolador via serial. El robot puede trazar trayectorias (círculo, triángulo, cuadrado) de forma autónoma.

## Archivos

| Archivo | Descripción |
|---|---|
| `main.c` | Firmware del ATmega328P — recibe comandos y mueve los servos |
| `interfaz.py` | Interfaz gráfica para conectar y controlar el robot |
| `simulacion.py` | Simulación interactiva para visualizar la cinemática inversa |

---

## `main.c` — Firmware

El microcontrolador recibe comandos por UART a 9600 baud con el formato:

```
S1:90.5 S2:120.3
```

Y posiciona los servos usando el **Timer1 en modo Fast PWM** (50Hz, período de 20ms):

- **PB1 (OC1A)** → Servo 1 (hombro)
- **PB2 (OC1B)** → Servo 2 (codo)

Los pulsos se calibraron manualmente para cada servo:

```c
#define S1_MIN 1060
#define S1_MAX 4900
```

El handshake al conectar es simple: la PC manda `READY` y el micro responde `OK`.

---

## `simulacion.py` — Visualizador de cinemática inversa

Simulación interactiva con matplotlib que muestra el robot trazando un círculo. Tiene sliders para mover el centro, cambiar el radio y recorrer la trayectoria paso a paso.

Muestra todos los ángulos del método geométrico (ley de cosenos):

- **θ** — ángulo de la base al eslabón 1
- **α** — ángulo del codo
- **β** — ángulo de la base al punto deseado
- **γ** — ángulo interno del triángulo en la base

Las fórmulas usadas:

```
α = arccos((xd² + yd² - l1² - l2²) / (2·l1·l2))
β = arctan2(yd, xd)
γ = arccos((D² + l1² - l2²) / (2·D·l1))
θ = β - γ
```

Para correrla:

```bash
python3 simulacion.py
```

---

## `interfaz.py` — Interfaz gráfica de control

Interfaz en `tkinter` para controlar el robot real. Se conecta al Arduino en `/dev/ttyUSB0`.

**Funciones:**
- Seleccionar figura: Círculo, Triángulo o Cuadrado
- Ajustar parámetros (centro, radio/lado, número de puntos)
- Controlar velocidad (pausa en ms entre puntos)
- Mover el robot a un punto (X, Y) manualmente
- Loop continuo

La cinemática inversa corre en Python y manda los ángulos ya calculados al micro, así el ATmega solo se encarga de mover los servos.

Para correrla:

```bash
pip install pyserial
python3 interfaz.py
```

---

## Hardware

- Microcontrolador: **ATmega328P** (Arduino UNO / Nano)
- 2 servomotores conectados a **PB1** y **PB2**
- Comunicación serial a **9600 baud** (`/dev/ttyUSB0`)
- Eslabones: L1 = L2 = 6 cm
- Espacio de trabajo: 0 a 12 cm de alcance
