"""
Simulación Interactiva: Robot planar 2 GDL trazando un círculo
Método geométrico (Ley de cosenos)

Controles:
    - Slider t: controla la posición actual sobre el círculo
    - Slider cx, cy: mueven el centro del círculo
    - Slider r: cambia el radio del círculo

Ángulos visibles en el diagrama:
    θ (rojo)   — ángulo de la base, del eje X al eslabón 1
    α (morado) — ángulo del codo, entre extensión de l1 y l2
    β (gris)   — ángulo de la base hacia el punto deseado
    γ (verde)  — ángulo interno del triángulo en la base

Fórmulas:
    α = arccos((xd² + yd² - l1² - l2²) / (2·l1·l2))
    β = arctan(yd / xd)
    γ = arccos((D² + l1² - l2²) / (2·D·l1))
    θ = β - γ
"""

import numpy as np
import matplotlib.pyplot as plt
from matplotlib.widgets import Slider
from matplotlib.patches import Arc

# ============================================================
# PARÁMETROS DEL ROBOT (fijos)
# ============================================================
l1 = 1.0
l2 = 0.8

# ============================================================
# FUNCIONES
# ============================================================
def cinematica_inversa(xd, yd, l1, l2):
    D = np.sqrt(xd**2 + yd**2)
    if D > (l1 + l2) or D < abs(l1 - l2):
        return None, None, None, None
    cos_alpha = (xd**2 + yd**2 - l1**2 - l2**2) / (2 * l1 * l2)
    cos_alpha = np.clip(cos_alpha, -1, 1)
    alpha = np.arccos(cos_alpha)
    beta = np.arctan2(yd, xd)
    cos_gamma = (D**2 + l1**2 - l2**2) / (2 * D * l1)
    cos_gamma = np.clip(cos_gamma, -1, 1)
    gamma = np.arccos(cos_gamma)
    theta = beta - gamma
    return theta, alpha, beta, gamma

def cinematica_directa(theta, alpha, l1, l2):
    x1 = l1 * np.cos(theta)
    y1 = l1 * np.sin(theta)
    x2 = x1 + l2 * np.cos(theta + alpha)
    y2 = y1 + l2 * np.sin(theta + alpha)
    return x1, y1, x2, y2

def dibujar_arco(ax, centro_x, centro_y, radio, angulo_inicio_deg, angulo_fin_deg,
                 color, linewidth=2, label=None, label_offset=1.4):
    """Dibuja un arco de ángulo y opcionalmente su etiqueta."""
    a1 = min(angulo_inicio_deg, angulo_fin_deg)
    a2 = max(angulo_inicio_deg, angulo_fin_deg)

    if a2 - a1 > 0.5:
        arco = Arc((centro_x, centro_y), 2 * radio, 2 * radio,
                   angle=0, theta1=a1, theta2=a2,
                   color=color, linewidth=linewidth, zorder=7)
        ax.add_patch(arco)

    if label:
        angulo_medio = np.radians((a1 + a2) / 2)
        lx = centro_x + radio * label_offset * np.cos(angulo_medio)
        ly = centro_y + radio * label_offset * np.sin(angulo_medio)
        ax.text(lx, ly, label, color=color, fontsize=12, fontweight='bold',
                ha='center', va='center', zorder=8)

# ============================================================
# CONFIGURAR FIGURA
# ============================================================
fig = plt.figure(figsize=(14, 9))
fig.patch.set_facecolor('#0f1117')

ax = fig.add_axes([0.06, 0.30, 0.55, 0.62])
ax.set_facecolor('#181b24')
ax.set_aspect('equal')

ax_info = fig.add_axes([0.64, 0.30, 0.34, 0.62])
ax_info.set_facecolor('#181b24')
ax_info.axis('off')

# ============================================================
# SLIDERS
# ============================================================
slider_color = '#2a2e3a'

ax_t  = fig.add_axes([0.15, 0.18, 0.70, 0.025], facecolor=slider_color)
ax_cx = fig.add_axes([0.15, 0.14, 0.70, 0.025], facecolor=slider_color)
ax_cy = fig.add_axes([0.15, 0.10, 0.70, 0.025], facecolor=slider_color)
ax_r  = fig.add_axes([0.15, 0.06, 0.70, 0.025], facecolor=slider_color)

s_t  = Slider(ax_t,  't (°)',  0, 360, valinit=0,   valstep=1,   color='#60a5fa')
s_cx = Slider(ax_cx, 'Cx',    -1.0, 1.5, valinit=1.0, valstep=0.05, color='#f9a825')
s_cy = Slider(ax_cy, 'Cy',    -1.0, 1.5, valinit=0.5, valstep=0.05, color='#f9a825')
s_r  = Slider(ax_r,  'Radio',  0.05, 0.8, valinit=0.3, valstep=0.05, color='#34d399')

for s in [s_t, s_cx, s_cy, s_r]:
    s.label.set_color('white')
    s.label.set_fontsize(10)
    s.valtext.set_color('white')

# ============================================================
# FUNCIÓN DE DIBUJO
# ============================================================
def actualizar(val):
    ax.cla()
    ax_info.cla()
    ax.set_facecolor('#181b24')
    ax_info.set_facecolor('#181b24')
    ax_info.set_xlim(0, 1)
    ax_info.set_ylim(0, 1)
    ax_info.axis('off')

    ax.set_aspect('equal')
    margen = 0.3
    lim = l1 + l2 + margen  # 2.1 para ver todo el espacio de trabajo
    ax.set_xlim(-lim, lim)
    ax.set_ylim(-lim, lim)
    ax.set_xlabel('X (m)', color='#8b8fa3')
    ax.set_ylabel('Y (m)', color='#8b8fa3')
    ax.tick_params(colors='#8b8fa3')
    for spine in ax.spines.values():
        spine.set_color('#2a2e3a')
    ax.grid(True, alpha=0.15, color='#3a3f52')

    # Leer parámetros
    t_deg = s_t.val
    cx = s_cx.val
    cy = s_cy.val
    r = s_r.val
    t_rad = np.radians(t_deg)

    # --- Espacio de trabajo ---
    angulos_ws = np.linspace(0, 2 * np.pi, 200)
    ax.plot((l1 + l2) * np.cos(angulos_ws), (l1 + l2) * np.sin(angulos_ws),
            '--', color='#3a3f52', linewidth=1, alpha=0.5)
    if abs(l1 - l2) > 0.01:
        ax.plot(abs(l1 - l2) * np.cos(angulos_ws), abs(l1 - l2) * np.sin(angulos_ws),
                '--', color='#3a3f52', linewidth=1, alpha=0.5)

    # --- Ejes ---
    ax.axhline(y=0, color='#3a3f52', linewidth=0.8)
    ax.axvline(x=0, color='#3a3f52', linewidth=0.8)

    # --- Círculo deseado ---
    circulo_t = np.linspace(0, 2 * np.pi, 200)
    circulo_x = cx + r * np.cos(circulo_t)
    circulo_y = cy + r * np.sin(circulo_t)
    ax.plot(circulo_x, circulo_y, '-', color='#60a5fa', linewidth=1.5, alpha=0.4)
    ax.plot(cx, cy, '+', color='#60a5fa', markersize=10, markeredgewidth=2)

    # --- Trayectoria recorrida ---
    t_recorrido = np.linspace(0, t_rad, max(2, int(t_deg)))
    tray_x = []
    tray_y = []
    for ti in t_recorrido:
        xdi = cx + r * np.cos(ti)
        ydi = cy + r * np.sin(ti)
        th, al, _, _ = cinematica_inversa(xdi, ydi, l1, l2)
        if th is not None:
            _, _, x2i, y2i = cinematica_directa(th, al, l1, l2)
            tray_x.append(x2i)
            tray_y.append(y2i)

    if len(tray_x) > 1:
        ax.plot(tray_x, tray_y, '-', color='#ef4444', linewidth=2.5, alpha=0.8)

    # --- Punto actual ---
    xd = cx + r * np.cos(t_rad)
    yd = cy + r * np.sin(t_rad)
    ax.plot(xd, yd, 'o', color='#60a5fa', markersize=8, alpha=0.6)

    # --- Robot ---
    theta, alpha, beta, gamma = cinematica_inversa(xd, yd, l1, l2)

    if theta is not None:
        x1, y1, x2, y2 = cinematica_directa(theta, alpha, l1, l2)
        D = np.sqrt(xd**2 + yd**2)
        theta_deg = np.degrees(theta)
        alpha_deg = np.degrees(alpha)
        beta_deg = np.degrees(beta)
        gamma_deg = np.degrees(gamma)

        # Línea D (base a efector)
        ax.plot([0, x2], [0, y2], '--', color='#60a5fa', linewidth=1.5, alpha=0.35, zorder=3)
        ax.annotate('D', xy=((x2) / 2 + 0.04, (y2) / 2 - 0.08),
                     color='#60a5fa', fontsize=11, fontweight='bold', zorder=8)

        # Extensión de l1 más allá del codo (línea punteada)
        ext_len = 0.35
        ext_x = x1 + ext_len * np.cos(theta)
        ext_y = y1 + ext_len * np.sin(theta)
        ax.plot([x1, ext_x], [y1, ext_y], '--', color='#ef4444',
                linewidth=1.2, alpha=0.35, zorder=3)

        # =============================================
        # ÁNGULOS (arcos)
        # =============================================

        # θ — del eje X al eslabón 1 (en la base)
        dibujar_arco(ax, 0, 0, 0.28,
                     0, theta_deg,
                     '#ef4444', linewidth=2.5,
                     label='θ', label_offset=1.5)

        # β — del eje X a la línea D (en la base)
        dibujar_arco(ax, 0, 0, 0.42,
                     0, beta_deg,
                     '#8b8fa3', linewidth=1.5,
                     label='β', label_offset=1.4)

        # γ — entre l1 y D (en la base, entre θ y β)
        dibujar_arco(ax, 0, 0, 0.35,
                     theta_deg, beta_deg,
                     '#34d399', linewidth=2,
                     label='γ', label_offset=1.5)

        # α — entre extensión de l1 y l2 (en el codo)
        dibujar_arco(ax, x1, y1, 0.22,
                     theta_deg, theta_deg + alpha_deg,
                     '#a78bfa', linewidth=2.5,
                     label='α', label_offset=1.5)

        # =============================================
        # ESLABONES
        # =============================================

        # Eslabón 1
        ax.plot([0, x1], [0, y1], '-', color='#ef4444',
                linewidth=6, solid_capstyle='round', zorder=5)
        
        # Colocar l₁ del lado opuesto a los ángulos
        mid_x = x1 / 2
        mid_y = y1 / 2
        perp_x =  np.sin(theta) * 0.25
        perp_y = -np.cos(theta) * 0.25
        ax.annotate('l₁', xy=(mid_x + perp_x, mid_y + perp_y),
                     color='#ef4444', fontsize=12, fontweight='bold', zorder=8)

        # Eslabón 2
        ax.plot([x1, x2], [y1, y2], '-', color='#a78bfa',
                linewidth=5, solid_capstyle='round', zorder=5)
        ax.annotate('l₂', xy=((x1 + x2) / 2 + 0.06, (y1 + y2) / 2 + 0.08),
                     color='#a78bfa', fontsize=12, fontweight='bold', zorder=8)

        # =============================================
        # ARTICULACIONES
        # =============================================
        ax.plot(0, 0, 's', color='#6ee7b7', markersize=13, zorder=6)
        ax.plot(x1, y1, 'o', color='#f9a825', markersize=11, zorder=6)
        ax.plot(x2, y2, 'o', color='#a78bfa', markersize=11, zorder=6)
        ax.plot(x2, y2, 'o', color='#a78bfa', markersize=18, alpha=0.3, zorder=6)
        ax.annotate(f'({x2:.2f}, {y2:.2f})', xy=(x2 + 0.08, y2 + 0.08),
                     color='#e2e4ea', fontsize=9, zorder=8)

        # =============================================
        # PANEL DE INFORMACIÓN
        # =============================================
        y_pos = 0.97
        line_h = 0.040

        def info(text, color, size=10, bold=False):
            nonlocal y_pos
            weight = 'bold' if bold else 'normal'
            ax_info.text(0.03, y_pos, text, color=color, fontsize=size,
                        fontweight=weight, fontfamily='monospace',
                        transform=ax_info.transAxes)
            y_pos -= line_h

        def espacio():
            nonlocal y_pos
            y_pos -= 0.012

        info('PARÁMETROS DEL CÍRCULO', 'white', 11, True)
        info(f'Centro: ({cx:.2f}, {cy:.2f})', '#f9a825')
        info(f'Radio:  {r:.2f} m', '#34d399')

        espacio()
        info(f'PUNTO ACTUAL  t = {t_deg:.0f}°', 'white', 11, True)
        info(f'xd = {cx:.2f}+{r:.2f}·cos({t_deg:.0f}°) = {xd:.3f}', '#60a5fa')
        info(f'yd = {cy:.2f}+{r:.2f}·sin({t_deg:.0f}°) = {yd:.3f}', '#60a5fa')

        espacio()
        info('CINEMÁTICA INVERSA', 'white', 11, True)
        info(f'D = √(xd²+yd²) = {D:.3f} m', '#60a5fa')

        espacio()
        cos_a_val = (xd**2 + yd**2 - l1**2 - l2**2) / (2 * l1 * l2)
        info(f'cos(α) = {cos_a_val:.4f}', '#a78bfa', 9)
        info(f'α = arccos(...) = {alpha_deg:.1f}°', '#a78bfa', 12, True)

        espacio()
        info(f'β = arctan(yd/xd) = {beta_deg:.1f}°', '#8b8fa3', 10)
        cos_g_val = (D**2 + l1**2 - l2**2) / (2 * D * l1)
        info(f'cos(γ) = {cos_g_val:.4f}', '#34d399', 9)
        info(f'γ = arccos(...) = {gamma_deg:.1f}°', '#34d399', 10)

        espacio()
        info(f'θ = β - γ', '#ef4444', 10)
        info(f'θ = {beta_deg:.1f}° - {gamma_deg:.1f}°', '#ef4444', 10)
        info(f'θ = {theta_deg:.1f}°', '#ef4444', 13, True)

        espacio()
        info('VERIFICACIÓN (directa)', 'white', 11, True)
        info(f'x₂={x2:.4f}  (deseado:{xd:.4f})', '#6ee7b7')
        info(f'y₂={y2:.4f}  (deseado:{yd:.4f})', '#6ee7b7')

        espacio()
        info('RESTRICCIÓN', 'white', 10, True)
        info(f'{abs(l1-l2):.2f} ≤ D={D:.2f} ≤ {l1+l2:.2f} ✓', '#34d399')

    else:
        ax.text(0.3, 0.5, '⚠ Punto inalcanzable',
                color='#ef4444', fontsize=16, ha='center', fontweight='bold')
        ax_info.text(0.05, 0.5, 'El punto está fuera\ndel espacio de trabajo',
                    color='#ef4444', fontsize=12, transform=ax_info.transAxes)

    fig.canvas.draw_idle()

# ============================================================
# CONECTAR SLIDERS Y MOSTRAR
# ============================================================
s_t.on_changed(actualizar)
s_cx.on_changed(actualizar)
s_cy.on_changed(actualizar)
s_r.on_changed(actualizar)

actualizar(None)
plt.show()
