/*
 * Apolonio Villegas Diego Armando
 * Palacios Cabrera Alberto
 * 
 * Práctica 7
 *
 */
    .DEF	TEMP = R16
	.DEF	CONT1 = R17
	.DEF	CONT2 = R18
	.DEF	CONT3 = R19
	.CSEG
	.ORG	0

	LDI	TEMP, $FE
	OUT DDRD, TEMP // Configura Puerto D
	CALL DELAY_100M
	
	// Función set de 4 bits
	LDI	TEMP, $28	// Enable en 1
	OUT	PORTD, TEMP
	LDI TEMP, $20	// Enable en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Función set de 8 bits
	LDI	TEMP, $28	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $20	// Enable en 0, Rs en 0
	OUT PORTD, TEMP

	LDI	TEMP, $88	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $80	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Display on/off
	LDI	TEMP, $08	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $00	// Enable en 0, Rs en 0
	OUT PORTD, TEMP

	LDI	TEMP, $E8	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $E0	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Entry Mode
	LDI	TEMP, $08	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $00	// Enable en 0, Rs en 0
	OUT PORTD, TEMP

	LDI	TEMP, $68	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $60	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	/*// Clear display
	LDI	TEMP, $08	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $00	// Enable en 0, Rs en 0
	OUT PORTD, TEMP

	LDI	TEMP, $18	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $10	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M*/
	CALL CLEAR_DISPLAY

MAIN:
	CALL FILA_1
	CALL SALTO_LINEA
	CALL FILA_2
	CALL DELAY_1S
	CALL CLEAR_DISPLAY
	JMP	MAIN

DELAY_100M:
	LDI	CONT1, 4
LAZO3:	
	LDI	CONT2, 200
LAZO2:	
	LDI	CONT3, 200
LAZO1:
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	DEC	CONT3
	BRNE LAZO1
	DEC CONT2
	BRNE LAZO2
	DEC CONT1
	BRNE LAZO3
	RET

DELAY_10M:
	LDI	CONT1, 80
LAZO5:	
	LDI	CONT2, 200
LAZO4:
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	NOP
	DEC	CONT2
	BRNE LAZO4
	DEC CONT1
	BRNE LAZO5
	RET

FILA_1:
	// Letra D

	LDI	TEMP, $4C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $44	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $4C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $44	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra i

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $9C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $94	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra e

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $5C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $54	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra g

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $7C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $74	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra o

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $FC	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $F4	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Espacio

	LDI	TEMP, $2C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $24	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $0C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $04	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra A

	LDI	TEMP, $4C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $44	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $1C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $14	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	/*// Punto

	LDI	TEMP, $2C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $24	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $EC	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $E4	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M*/

	// Espacio

	LDI	TEMP, $2C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $24	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $0C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $04	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra A

	LDI	TEMP, $4C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $44	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $1C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $14	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra p

	LDI	TEMP, $7C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $74	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $0C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $04	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra o

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $FC	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $F4	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra l

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $CC	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $C4	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra o

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $FC	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $F4	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra n

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $EC	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $E4	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra i

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $9C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $94	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra o

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP
	CALL DELAY_100M

	LDI	TEMP, $FC	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $F4	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_100M

	/*// Shift display
	LDI	TEMP, $18	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $10	// Enable en 0, Rs en 0
	OUT PORTD, TEMP

	LDI	TEMP, $88	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $80	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M*/

	RET

FILA_2:
	// Letra A

	LDI	TEMP, $4C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $44	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $1C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $14	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra l

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $CC	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $C4	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra b

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $2C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $24	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra e

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $5C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $54	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra r

	LDI	TEMP, $7C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $74	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $2C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $24	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra t

	LDI	TEMP, $7C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $74	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $4C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $44	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra o

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $FC	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $F4	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Espacio

	LDI	TEMP, $2C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $24	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $0C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $04	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra P

	LDI	TEMP, $5C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $54	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $0C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $04	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra a

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $1C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $14	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra l

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $CC	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $C4	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra a

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $1C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $14	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra c

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $3C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $34	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra i

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $9C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $94	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra o

	LDI	TEMP, $6C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $64	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $FC	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $F4	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	// Letra s

	LDI	TEMP, $7C	// Enable en 1, Rs en 1
	OUT	PORTD, TEMP
	LDI TEMP, $74	// Enable en 0, Rs en 1
	OUT PORTD, TEMP

	LDI	TEMP, $3C	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $34	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	RET

SALTO_LINEA:
	// Cambiar línea

	LDI TEMP, $C8
    OUT PORTD, TEMP // E en 1, RS en 0
    LDI TEMP, $C0
    OUT PORTD, TEMP // E en 0, RS en 0
    CALL DELAY_100M

    LDI TEMP, $08
    OUT PORTD, TEMP // E en 1, RS en 0
    LDI TEMP, $00
    OUT PORTD, TEMP // E en 0, RS en 0
    CALL DELAY_100M

	RET

CLEAR_DISPLAY:
	// Clear display
	LDI	TEMP, $08	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $00	// Enable en 0, Rs en 0
	OUT PORTD, TEMP

	LDI	TEMP, $18	// Enable en 1, Rs en 0
	OUT	PORTD, TEMP
	LDI TEMP, $10	// Enable en 0, Rs en 0
	OUT PORTD, TEMP
	CALL DELAY_10M

	RET

DELAY_1S:
	CALL DELAY_100M
	CALL DELAY_100M
	CALL DELAY_100M
	CALL DELAY_100M
	CALL DELAY_100M
	CALL DELAY_100M
	CALL DELAY_100M
	CALL DELAY_100M
	CALL DELAY_100M
	CALL DELAY_100M
	RET
