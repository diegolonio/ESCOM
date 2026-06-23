import math

PUNTOS = 500
MAX_DAC = 4095

senoidal, cuadrada, sierra, triangular = [], [], [], []

for i in range(PUNTOS):
    # Fase de 0 a 2*PI
    fase = (i / PUNTOS) * (2.0 * math.pi)
    
    # 1. Senoidal
    val_sen = int((math.sin(fase) + 1.0) * (MAX_DAC / 2.0))
    senoidal.append(val_sen)
    
    # 2. Cuadrada
    val_cua = MAX_DAC if fase < math.pi else 0
    cuadrada.append(val_cua)
    
    # 3. Sierra
    val_sie = int((fase / (2.0 * math.pi)) * MAX_DAC)
    sierra.append(val_sie)
    
    # 4. Triangular
    if fase < math.pi:
        val_tri = int((fase / math.pi) * MAX_DAC)
    else:
        val_tri = int(((2.0 * math.pi - fase) / math.pi) * MAX_DAC)
    triangular.append(val_tri)

def imprimir_tabla(nombre, datos):
    print(f"const uint16_t {nombre}[{PUNTOS}] PROGMEM = {{")
    lineas = [", ".join(map(str, datos[i:i+10])) for i in range(0, PUNTOS, 10)]
    print("    " + ",\n    ".join(lineas))
    print("};\n")

print("#include <avr/pgmspace.h>\n")
imprimir_tabla("TABLA_SENOIDAL", senoidal)
imprimir_tabla("TABLA_CUADRADA", cuadrada)
imprimir_tabla("TABLA_SIERRA", sierra)
imprimir_tabla("TABLA_TRIANGULAR", triangular)
