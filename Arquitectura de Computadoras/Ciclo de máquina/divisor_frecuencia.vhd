LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY divisor_frecuencia IS
	PORT (
		CLK: IN std_logic;
		pulso: OUT std_logic
	);
END divisor_frecuencia;

ARCHITECTURE divisor_frecuencia OF divisor_frecuencia IS
	SIGNAL auxiliar: std_logic;
BEGIN
	PROCESS (CLK)
		VARIABLE contador: INTEGER RANGE 0 TO 9 := 0;
	BEGIN
		IF (CLK'EVENT AND CLK = '1') THEN
			IF (contador = 9) THEN
				contador := 0;
				auxiliar <= NOT auxiliar;
 		    ELSE
				contador := contador + 1;
			END IF;
		END IF;
	END PROCESS;

	pulso <= auxiliar;
END divisor_frecuencia;
