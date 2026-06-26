#!/usr/bin/env python3
import numpy as np, re, pandas as pd
from sklearn.linear_model import LogisticRegression
from sklearn.model_selection import train_test_split
from sklearn.metrics import confusion_matrix

# ---------------------- PARAMETROS ----------------------
FS = 50 # Hz, frecuencia de muestreo
VENTANA = 50 # muestras por ventana (1s)
G = 6950 # magnitud cruda de 1g en el sensor utilizado en particular (offset incluido)
         # este valor puede variar incluso entre sensores del mismo modelo
TH_PICO = G * 1.6 # umbral de impacto (golpe/caida)
TH_VALLE = G * 0.5 # umbral de caida libre
CAIDA_OFF = 35 # la ventana de caida empieza 35 muestras antes del pico
               # (para capturar el valle, que ocurre antes del impacto)

# ---------------------- LECTURA ROBUSTA ----------------------
def load_clean(path):
    """Lee un CSV ignorando bytes basura del reset serial y cabeceras coladas.
    Solo conserva lineas con el patron exacto 'entero,entero,entero'."""
    rows = []

    with open(path, 'rb') as f:
        raw = f.read().decode('utf-8', errors='ignore')

    for line in raw.splitlines():
        m = re.match(r'^\s*(-?\d+),(-?\d+),(-?\d+)\s*$', line.strip())

        if m:
            rows.append([int(m.group(1)), int(m.group(2)), int(m.group(3))])

    return pd.DataFrame(rows, columns=['ax', 'ay', 'az'])

def magnitude(df):
    return np.sqrt((df[['ax','ay','az']].astype(float)**2).sum(axis=1)).values

# ---------------------- FEATURES ----------------------
def features(win):
    """5 features sobre la magnitud de una ventana:
       media, desviacion estandar, minimo, maximo, y tiempo en valle
       (numero de muestras en caida libre). El 'tiempo en valle' es lo que
       distingue una caida (vuelo sostenido ~0g) de un golpe (pico aislado)."""
    return [win.mean(), win.std(), win.min(), win.max(), (win < TH_VALLE).sum()]

def find_events(mag, th):
    """Devuelve el indice central de cada evento (cruce sostenido sobre 'th')."""
    ev = []; i = 0

    while i < len(mag):
        if mag[i] > th:
            j = i

            while j < len(mag) and mag[j] > th:
                j += 1

            ev.append((i + j) // 2)
            i = j
        else:
            i += 1

    return ev

# ---------------------- CONSTRUIR DATASET ----------------------
def build_dataset():
    X, y = [], []

    # Pasivo: ventanas deslizantes que se desplazan 0.5s
    mag = magnitude(load_clean('reposo.csv'))
    for i in range(0, len(mag) - VENTANA, VENTANA // 2):
        w = mag[i:i+VENTANA]

        if w.max() < TH_PICO and w.min() > TH_VALLE:
            X.append(features(w)); y.append('pasivo')

    # Golpe: una ventana centrada en el pico
    mag = magnitude(load_clean('golpes.csv'))
    for c in find_events(mag, TH_PICO):
        a = max(0, c - VENTANA//2); b = a + VENTANA

        if b <= len(mag):
            X.append(features(mag[a:b])); y.append('golpe')

    # Caída: ventana que empieza 35 muestras antes del pico para incluir el valle
    mag = magnitude(load_clean('caidas.csv'))
    for c in find_events(mag, TH_PICO):
        a = max(0, c - CAIDA_OFF); b = a + VENTANA
        
        if b <= len(mag) and a >= 0:
            X.append(features(mag[a:b])); y.append('caida')

    return np.array(X), np.array(y)

# ---------------------- IMPRIMIR MODELO ----------------------
def print_model(clf, mu, sd):
    classes = list(clf.classes_)
    Wc, bc = clf.coef_, clf.intercept_
    feats = ['media', 'std', 'min', 'max', 'valle']

    print(f"VENTANA = {VENTANA}")
    print(f"UMBRAL_VALLE = {int(TH_VALLE)}")
    print("Clases: " + ", ".join(f"{k}={c}" for k, c in enumerate(classes)))
    print()

    print("MU (media de cada feature):")
    for name, v in zip(feats, mu):
        print(f"  {name:6s} = {v:.4f}")
    print()

    print("SD (desviacion de cada feature):")
    for name, v in zip(feats, sd):
        print(f"  {name:6s} = {v:.4f}")
    print()

    print("W (pesos por clase y feature):")
    for k, c in enumerate(classes):
        print(f"  {c}:")
        for name, v in zip(feats, Wc[k]):
            print(f"    {name:6s} = {v:+.6f}")
    print()

    print("B (sesgo por clase):")
    for c, v in zip(classes, bc):
        print(f"  {c:8s} = {v:+.6f}")

# ---------------------- MAIN ----------------------
def main():
    X, y = build_dataset()

    # Normalizacion (estandarizacion)
    mu, sd = X.mean(0), X.std(0)
    Xn = (X - mu) / sd

    # Matriz de confusion y precision sobre un hold-out
    Xtr, Xte, ytr, yte = train_test_split(Xn, y, test_size=0.3,
                                          random_state=42, stratify=y)
    clf = LogisticRegression(max_iter=5000).fit(Xtr, ytr)
    labels = ['pasivo','golpe','caida']
    ypred = clf.predict(Xte)
    cm = confusion_matrix(yte, ypred, labels=labels)
    print("Matriz de confusion (filas=real, cols=predicho):")
    print(f"{'':>8}" + "".join(f"{l:>8}" for l in labels))
    for i, l in enumerate(labels):
        print(f"{l:>8}" + "".join(f"{cm[i,j]:>8}" for j in range(3)))
    print(f"\nPrecision: {(ypred == yte).mean()*100:.1f}%")

    # Modelo final
    clf_final = LogisticRegression(max_iter=5000).fit(Xn, y)
    print("\n----- Valores del modelo -----\n")
    print_model(clf_final, mu, sd)

if __name__ == '__main__':
    main()
