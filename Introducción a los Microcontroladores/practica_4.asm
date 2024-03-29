/*
 * Apolonio Villegas Diego Armando
 * Palacios Cabrera Alberto
 * 
 * Práctica 4
 *
 */
	.DEF TMP = R16
	.DEF UNIDADES = R17
	.DEF DECENAS = R18
	.DEF MUX = R19
	.DEF CONT1 = R20
	.DEF CONT2 = R21
	.DEF CONT3 = R22 
	.CSEG
	.ORG 0

; Codificación de los números para el display de 7 segmentos
CONSTANTES:
	.DB $3F, $06, $5B, $4F, $66, $6D, $7D, $07, $7F, $67

	RJMP CONFIGURACION_PUERTOS
	.ORG $020
	RJMP DISPLAY

CONFIGURACION_PUERTOS:
; Configurar puerto D como salida 
	LDI TMP, $FF
	OUT DDRD, TMP
; Configurar puerto C
	LDI TMP, $03
	OUT DDRC, TMP
;Configurar resistencia de pull-up del pin C2
	LDI TMP, $04
	OUT PORTC, TMP
; Configuración del timer0
	LDI TMP, $04
	OUT TCCR0B, TMP
	LDI TMP, $01
	STS TIMSK0, TMP
	SEI

	LDI UNIDADES, $00
	LDI DECENAS, $00

MAIN:
	IN TMP, PINC
	ANDI TMP, $04
	CPI TMP, $00
	BRNE CUENTA_DESCENDENTE
	JMP CUENTA_ASCENDENTE

CUENTA_ASCENDENTE:
	CALL DELAY_1SEG
	INC UNIDADES
	CPI UNIDADES, $0A
	BRNE MAIN
	JMP INCREMENTA_DECENAS

INCREMENTA_DECENAS:
	LDI UNIDADES, $00
	INC DECENAS
	CPI DECENAS, $0A
	BRNE MAIN
	JMP DECENAS_IGUAL_0

DECENAS_IGUAL_0:
	LDI DECENAS, $00
	JMP MAIN

CUENTA_DESCENDENTE:
	CALL DELAY_1SEG
	DEC UNIDADES
	CPI UNIDADES, $FF
	BRNE MAIN
	JMP DECREMENTA_DECENAS

DECREMENTA_DECENAS:
	LDI UNIDADES, $09
	DEC DECENAS
	CPI DECENAS, $FF
	BRNE MAIN
	JMP DECENAS_IGUAL_9

DECENAS_IGUAL_9:
	LDI DECENAS, $09
	JMP MAIN

DELAY_1SEG:
	LDI CONT1, 200
LAZO3:
	LDI CONT2, 200
LAZO2:
	LDI CONT3, 200
LAZO1:
	DEC CONT3
	BRNE LAZO1
	DEC CONT2
	BRNE LAZO2
	DEC CONT1
	BRNE LAZO3
	RET

DISPLAY:
	CPI MUX, $05
	BRNE ENCIENDE_UNIDADES
	JMP ENCIENDE_DECENAS

ENCIENDE_UNIDADES:
	MOV ZL, UNIDADES
	LDI ZH, $00
	LPM TMP, Z
	OUT PORTD, TMP
	OUT PORTC, MUX
	LDI MUX, $05
	RETI

ENCIENDE_DECENAS:
	MOV ZL, DECENAS
	LDI ZH, $00
	LPM TMP, Z
	OUT PORTD, TMP
	OUT PORTC, MUX
	LDI MUX, $06
	RETI
