import tkinter as tk
from tkinter import ttk
import serial
import math
import threading
import time

# ============================================================
# PARÁMETROS DEL ROBOT (cm)
# ============================================================

L1 = 6.0
L2 = 6.0

# Posición home: brazo extendido hacia arriba
HOME_X = 0.0
HOME_Y = 12.0

# ============================================================
# CINEMÁTICA INVERSA
# ============================================================

def cinematica_inversa(xd, yd):
    D = math.sqrt(xd**2 + yd**2)
    if D > (L1 + L2) or D < abs(L1 - L2):
        return None
    cos_alpha = (xd**2 + yd**2 - L1**2 - L2**2) / (2.0 * L1 * L2)
    cos_alpha = max(-1.0, min(1.0, cos_alpha))
    alpha = math.acos(cos_alpha)
    beta = math.atan2(yd, xd)
    cos_gamma = (D**2 + L1**2 - L2**2) / (2.0 * D * L1)
    cos_gamma = max(-1.0, min(1.0, cos_gamma))
    gamma = math.acos(cos_gamma)
    theta = beta - gamma
    return (math.degrees(theta), math.degrees(alpha))


def angulos_a_servos(theta_deg, alpha_deg):
    s1 = theta_deg + 90.0
    s2 = 180.0 - alpha_deg
    return (s1, s2)

# ============================================================
# GENERADORES DE TRAYECTORIAS
# ============================================================

def generar_circulo(cx, cy, r, n):
    """Genera n puntos sobre un círculo."""
    puntos = []
    for i in range(n):
        t = 2.0 * math.pi * i / n
        x = cx + r * math.cos(t)
        y = cy + r * math.sin(t)
        puntos.append((x, y))
    # Cerrar el círculo
    puntos.append(puntos[0])
    return puntos


def generar_triangulo(cx, cy, lado, n):
    """Genera puntos sobre un triángulo equilátero centrado en (cx, cy)."""
    # Vértices del triángulo equilátero
    h = lado * math.sqrt(3) / 2
    v1 = (cx, cy + 2 * h / 3)
    v2 = (cx - lado / 2, cy - h / 3)
    v3 = (cx + lado / 2, cy - h / 3)
    vertices = [v1, v2, v3, v1]  # Cerrar

    return interpolar_vertices(vertices, n)


def generar_cuadrado(cx, cy, lado, n):
    """Genera puntos sobre un cuadrado centrado en (cx, cy)."""
    mitad = lado / 2
    v1 = (cx - mitad, cy - mitad)
    v2 = (cx + mitad, cy - mitad)
    v3 = (cx + mitad, cy + mitad)
    v4 = (cx - mitad, cy + mitad)
    vertices = [v1, v2, v3, v4, v1]  # Cerrar

    return interpolar_vertices(vertices, n)


def interpolar_vertices(vertices, n):
    """Interpola n puntos uniformemente entre los vértices."""
    # Calcular longitud total
    longitud_total = 0
    for i in range(len(vertices) - 1):
        dx = vertices[i+1][0] - vertices[i][0]
        dy = vertices[i+1][1] - vertices[i][1]
        longitud_total += math.sqrt(dx**2 + dy**2)

    # Puntos por lado proporcionales a la longitud
    puntos = []
    for i in range(len(vertices) - 1):
        x1, y1 = vertices[i]
        x2, y2 = vertices[i+1]
        dx = x2 - x1
        dy = y2 - y1
        lado_len = math.sqrt(dx**2 + dy**2)
        n_lado = max(2, int(n * lado_len / longitud_total))

        for j in range(n_lado):
            t = j / n_lado
            x = x1 + dx * t
            y = y1 + dy * t
            puntos.append((x, y))

    # Cerrar
    puntos.append(vertices[-1])
    return puntos

# ============================================================
# APLICACIÓN
# ============================================================

class RobotApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Robot Planar 2 GDL - Control")
        self.root.configure(bg='#0f1117')
        self.root.geometry("480x750")
        self.root.resizable(False, False)

        self.serial_port = None
        self.running = False
        self.loop_enabled = tk.BooleanVar(value=False)
        self.figura_var = tk.StringVar(value='Círculo')

        self.crear_ui()

    def crear_ui(self):
        bg = '#0f1117'
        card_bg = '#181b24'
        fg = '#e2e4ea'
        muted = '#8b8fa3'
        accent = '#6ee7b7'
        red = '#ef4444'

        style = ttk.Style()
        style.theme_use('clam')
        style.configure('Card.TFrame', background=card_bg)
        style.configure('Card.TLabel', background=card_bg, foreground=fg,
                        font=('Helvetica', 10))
        style.configure('Title.TLabel', background=card_bg, foreground=accent,
                        font=('Helvetica', 11, 'bold'))
        style.configure('Muted.TLabel', background=card_bg, foreground=muted,
                        font=('Helvetica', 9))
        style.configure('Status.TLabel', background=bg, foreground=muted,
                        font=('Helvetica', 9))

        # --- Conexión ---
        frame_serial = ttk.Frame(self.root, style='Card.TFrame', padding=12)
        frame_serial.pack(fill='x', padx=12, pady=(12, 6))

        ttk.Label(frame_serial, text="CONEXIÓN", style='Title.TLabel').pack(anchor='w')

        row_port = ttk.Frame(frame_serial, style='Card.TFrame')
        row_port.pack(fill='x', pady=(8, 0))

        ttk.Label(row_port, text="/dev/ttyUSB0", style='Card.TLabel').pack(side='left', padx=(0, 12))

        self.btn_connect = tk.Button(row_port, text="Conectar", command=self.conectar,
                                     bg=accent, fg='#0f1117', relief='flat',
                                     font=('Helvetica', 10, 'bold'), padx=12)
        self.btn_connect.pack(side='left')

        # --- Selector de figura ---
        frame_figura = ttk.Frame(self.root, style='Card.TFrame', padding=12)
        frame_figura.pack(fill='x', padx=12, pady=6)

        ttk.Label(frame_figura, text="FIGURA", style='Title.TLabel').pack(anchor='w')

        fig_row = ttk.Frame(frame_figura, style='Card.TFrame')
        fig_row.pack(fill='x', pady=(8, 0))

        for fig in ['Círculo', 'Triángulo', 'Cuadrado']:
            rb = tk.Radiobutton(fig_row, text=fig, variable=self.figura_var, value=fig,
                                bg=card_bg, fg=fg, selectcolor='#2a2e3a',
                                activebackground=card_bg, activeforeground=fg,
                                font=('Helvetica', 10), command=self.actualizar_params)
            rb.pack(side='left', padx=(0, 12))

        # --- Parámetros ---
        self.frame_params = ttk.Frame(self.root, style='Card.TFrame', padding=12)
        self.frame_params.pack(fill='x', padx=12, pady=6)

        self.lbl_params_title = ttk.Label(self.frame_params, text="PARÁMETROS (cm)",
                                           style='Title.TLabel')
        self.lbl_params_title.pack(anchor='w')

        espacio = f"Espacio de trabajo: [{abs(L1-L2):.1f}, {L1+L2:.1f}] cm"
        ttk.Label(self.frame_params, text=espacio, style='Muted.TLabel').pack(anchor='w', pady=(2, 8))

        self.params_container = ttk.Frame(self.frame_params, style='Card.TFrame')
        self.params_container.pack(fill='x')

        self.entries = {}
        self.actualizar_params()

        # --- Velocidad ---
        frame_vel = ttk.Frame(self.root, style='Card.TFrame', padding=12)
        frame_vel.pack(fill='x', padx=12, pady=6)

        ttk.Label(frame_vel, text="VELOCIDAD", style='Title.TLabel').pack(anchor='w')

        vel_row = ttk.Frame(frame_vel, style='Card.TFrame')
        vel_row.pack(fill='x', pady=(8, 0))

        ttk.Label(vel_row, text="Pausa entre puntos (ms):", style='Card.TLabel').pack(side='left')

        self.entry_pausa = tk.Entry(vel_row, width=6, bg='#2a2e3a', fg='#e2e4ea',
                                    insertbackground='#e2e4ea', relief='flat',
                                    font=('Helvetica', 11), justify='center')
        self.entry_pausa.insert(0, '20')
        self.entry_pausa.pack(side='left', padx=(8, 0))

        # --- Control ---
        frame_control = ttk.Frame(self.root, style='Card.TFrame', padding=12)
        frame_control.pack(fill='x', padx=12, pady=6)

        ttk.Label(frame_control, text="CONTROL", style='Title.TLabel').pack(anchor='w')

        btn_row = ttk.Frame(frame_control, style='Card.TFrame')
        btn_row.pack(fill='x', pady=(8, 0))

        self.btn_start = tk.Button(btn_row, text="▶ Iniciar", command=self.iniciar,
                                   bg=accent, fg='#0f1117', relief='flat',
                                   font=('Helvetica', 12, 'bold'), padx=16, pady=6,
                                   state='disabled')
        self.btn_start.pack(side='left', padx=(0, 6))

        self.btn_stop = tk.Button(btn_row, text="■ Detener", command=self.detener,
                                  bg=red, fg='white', relief='flat',
                                  font=('Helvetica', 12, 'bold'), padx=16, pady=6,
                                  state='disabled')
        self.btn_stop.pack(side='left', padx=(0, 6))

        self.btn_home = tk.Button(btn_row, text="⌂ Home", command=self.ir_a_home,
                                   bg='#f9a825', fg='#0f1117', relief='flat',
                                   font=('Helvetica', 12, 'bold'), padx=16, pady=6,
                                   state='disabled')
        self.btn_home.pack(side='left')

        loop_row = ttk.Frame(frame_control, style='Card.TFrame')
        loop_row.pack(fill='x', pady=(10, 0))

        self.chk_loop = tk.Checkbutton(loop_row, text="Loop continuo",
                                        variable=self.loop_enabled,
                                        bg=card_bg, fg='#e2e4ea', selectcolor='#2a2e3a',
                                        activebackground=card_bg, activeforeground='#e2e4ea',
                                        font=('Helvetica', 10))
        self.chk_loop.pack(side='left')

        # --- Ir a punto ---
        goto_row = ttk.Frame(frame_control, style='Card.TFrame')
        goto_row.pack(fill='x', pady=(10, 0))

        ttk.Label(goto_row, text="Ir a:", style='Card.TLabel').pack(side='left', padx=(0, 6))

        ttk.Label(goto_row, text="X", style='Card.TLabel').pack(side='left')
        self.entry_goto_x = tk.Entry(goto_row, width=5, bg='#2a2e3a', fg='#e2e4ea',
                                     insertbackground='#e2e4ea', relief='flat',
                                     font=('Helvetica', 11), justify='center')
        self.entry_goto_x.insert(0, '0')
        self.entry_goto_x.pack(side='left', padx=(4, 8))

        ttk.Label(goto_row, text="Y", style='Card.TLabel').pack(side='left')
        self.entry_goto_y = tk.Entry(goto_row, width=5, bg='#2a2e3a', fg='#e2e4ea',
                                     insertbackground='#e2e4ea', relief='flat',
                                     font=('Helvetica', 11), justify='center')
        self.entry_goto_y.insert(0, '12')
        self.entry_goto_y.pack(side='left', padx=(4, 8))

        self.btn_goto = tk.Button(goto_row, text="→ Enviar", command=self.ir_a_punto,
                                   bg='#60a5fa', fg='#0f1117', relief='flat',
                                   font=('Helvetica', 10, 'bold'), padx=10,
                                   state='disabled')
        self.btn_goto.pack(side='left')

        # --- Estado ---
        self.lbl_status = ttk.Label(self.root, text="Desconectado", style='Status.TLabel')
        self.lbl_status.pack(fill='x', padx=12, pady=(6, 4))

        self.lbl_info = ttk.Label(self.root, text="", style='Status.TLabel')
        self.lbl_info.pack(fill='x', padx=12, pady=(0, 12))

    def actualizar_params(self):
        """Recrea los campos de parámetros según la figura seleccionada."""
        for widget in self.params_container.winfo_children():
            widget.destroy()
        self.entries = {}

        figura = self.figura_var.get()
        fg = '#e2e4ea'

        if figura == 'Círculo':
            defaults = {'Cx': '7.0', 'Cy': '3.0', 'Radio': '2.0', 'Puntos': '100'}
        elif figura == 'Triángulo':
            defaults = {'Cx': '7.0', 'Cy': '3.0', 'Lado': '3.0', 'Puntos': '100'}
        else:  # Cuadrado
            defaults = {'Cx': '7.0', 'Cy': '3.0', 'Lado': '3.0', 'Puntos': '100'}

        for i, (label, default) in enumerate(defaults.items()):
            row = i // 2
            col = (i % 2) * 2

            ttk.Label(self.params_container, text=label, style='Card.TLabel').grid(
                row=row, column=col, sticky='w', padx=(0, 6), pady=4)

            entry = tk.Entry(self.params_container, width=8, bg='#2a2e3a', fg=fg,
                           insertbackground=fg, relief='flat', font=('Helvetica', 11),
                           justify='center')
            entry.insert(0, default)
            entry.grid(row=row, column=col+1, padx=(0, 16), pady=4)
            self.entries[label] = entry

    # ============================================================
    # CONEXIÓN
    # ============================================================

    def conectar(self):
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.close()
            self.serial_port = None
            self.btn_connect.configure(text="Conectar", bg='#6ee7b7')
            self.btn_start.configure(state='disabled')
            self.btn_home.configure(state='disabled')
            self.btn_goto.configure(state='disabled')
            self.lbl_status.configure(text="Desconectado")
            return

        puerto = '/dev/ttyUSB0'

        try:
            self.serial_port = serial.Serial(puerto, 9600, timeout=2)
            time.sleep(2)

            while self.serial_port.in_waiting:
                self.serial_port.readline()

            self.serial_port.write(b'READY\n')
            respuesta = self.serial_port.readline().decode().strip()

            if respuesta == 'OK':
                self.btn_connect.configure(text="Desconectar", bg='#ef4444')
                self.btn_start.configure(state='normal')
                self.btn_home.configure(state='normal')
                self.btn_goto.configure(state='normal')
                self.lbl_status.configure(text=f"Conectado a {puerto}")
                self.ir_a_home()
            else:
                self.serial_port.close()
                self.serial_port = None
                self.lbl_status.configure(text="Sin respuesta del robot")

        except Exception as e:
            self.lbl_status.configure(text=f"Error: {e}")

    # ============================================================
    # CONTROL
    # ============================================================

    def iniciar(self):
        if not self.serial_port or not self.serial_port.is_open:
            return

        self.running = True
        self.btn_start.configure(state='disabled')
        self.btn_stop.configure(state='normal')

        try:
            cx = float(self.entries['Cx'].get())
            cy = float(self.entries['Cy'].get())
            n = int(self.entries['Puntos'].get())
            pausa = int(self.entry_pausa.get())
            figura = self.figura_var.get()

            if figura == 'Círculo':
                r = float(self.entries['Radio'].get())
                puntos = generar_circulo(cx, cy, r, n)
            elif figura == 'Triángulo':
                lado = float(self.entries['Lado'].get())
                puntos = generar_triangulo(cx, cy, lado, n)
            else:
                lado = float(self.entries['Lado'].get())
                puntos = generar_cuadrado(cx, cy, lado, n)

        except ValueError:
            self.lbl_status.configure(text="Error en los parámetros")
            self.running = False
            self.btn_start.configure(state='normal')
            self.btn_stop.configure(state='disabled')
            return

        thread = threading.Thread(target=self.trazar_figura,
                                  args=(puntos, pausa, figura), daemon=True)
        thread.start()

    def trazar_figura(self, puntos, pausa, nombre):
        self.root.after(0, lambda: self.lbl_status.configure(
            text=f"Trazando {nombre}... ({len(puntos)} puntos)"))

        while self.running:
            for i, (xd, yd) in enumerate(puntos):
                if not self.running:
                    break

                resultado = cinematica_inversa(xd, yd)

                if resultado:
                    theta_deg, alpha_deg = resultado
                    s1, s2 = angulos_a_servos(theta_deg, alpha_deg)

                    cmd = f"S1:{s1:.1f} S2:{s2:.1f}\n"
                    try:
                        self.serial_port.write(cmd.encode())
                        self.serial_port.readline()
                    except:
                        self.running = False
                        self.root.after(0, lambda: self.lbl_status.configure(
                            text="Error de comunicación"))
                        break

                    if i % 5 == 0:
                        info = f"pt {i}/{len(puntos)}  ({xd:.1f},{yd:.1f})  θ={theta_deg:.1f}°  α={alpha_deg:.1f}°"
                        self.root.after(0, lambda i=info: self.lbl_info.configure(text=i))
                else:
                    self.root.after(0, lambda x=xd, y=yd: self.lbl_status.configure(
                        text=f"Inalcanzable ({x:.1f}, {y:.1f})"))

                time.sleep(pausa / 1000.0)

            if not self.loop_enabled.get():
                break

            self.root.after(0, lambda: self.lbl_status.configure(text="Reiniciando loop..."))
            time.sleep(0.5)

        # Volver a home si no es loop
        if not self.loop_enabled.get():
            self.root.after(0, lambda: self.lbl_status.configure(text="Regresando a home..."))
            time.sleep(0.3)
            self.enviar_posicion(HOME_X, HOME_Y)

        self.running = False
        self.root.after(0, lambda: self.btn_start.configure(state='normal'))
        self.root.after(0, lambda: self.btn_stop.configure(state='disabled'))
        if not self.loop_enabled.get():
            self.root.after(0, lambda: self.lbl_status.configure(
                text=f"Listo — Home ({HOME_X}, {HOME_Y}) cm"))
        else:
            self.root.after(0, lambda: self.lbl_status.configure(text="Detenido"))

    def enviar_posicion(self, xd, yd):
        resultado = cinematica_inversa(xd, yd)
        if resultado and self.serial_port and self.serial_port.is_open:
            theta_deg, alpha_deg = resultado
            s1, s2 = angulos_a_servos(theta_deg, alpha_deg)
            cmd = f"S1:{s1:.1f} S2:{s2:.1f}\n"
            self.serial_port.write(cmd.encode())
            self.serial_port.readline()
            return True
        return False

    def ir_a_home(self):
        if self.enviar_posicion(HOME_X, HOME_Y):
            self.lbl_status.configure(text=f"Home ({HOME_X}, {HOME_Y}) cm")
            self.lbl_info.configure(text="")

    def ir_a_punto(self):
        try:
            x = float(self.entry_goto_x.get())
            y = float(self.entry_goto_y.get())
        except ValueError:
            self.lbl_status.configure(text="Coordenadas inválidas")
            return

        if self.enviar_posicion(x, y):
            resultado = cinematica_inversa(x, y)
            if resultado:
                theta_deg, alpha_deg = resultado
                s1, s2 = angulos_a_servos(theta_deg, alpha_deg)
                self.lbl_status.configure(text=f"En ({x}, {y}) cm")
                self.lbl_info.configure(text=f"θ={theta_deg:.1f}°  α={alpha_deg:.1f}°  s1={s1:.1f}°  s2={s2:.1f}°")
        else:
            self.lbl_status.configure(text=f"Punto ({x}, {y}) inalcanzable")

    def detener(self):
        self.running = False

    def __del__(self):
        if self.serial_port and self.serial_port.is_open:
            self.serial_port.close()


# ============================================================
# MAIN
# ============================================================

if __name__ == '__main__':
    root = tk.Tk()
    app = RobotApp(root)
    root.mainloop()
    