LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY ciclo_maquina IS
	PORT (
		CLK: IN std_logic;
		S: OUT std_logic_vector (4 DOWNTO 0)
	);
END ciclo_maquina;

ARCHITECTURE ciclo_maquina OF ciclo_maquina IS

	-- Divisor de frecuencia
	COMPONENT divisor_frecuencia
		PORT (
			CLK: IN std_logic;
			pulso: OUT std_logic
		);
	END COMPONENT;

	-- Contador de anillo
	COMPONENT contador_anillo
		PORT (
			CLK: IN std_logic;
			S: OUT std_logic_vector (4 DOWNTO 0)
		);
	END COMPONENT;
	
	-- Para conectar la salida del divisor de frecuencia al contador de anillo
	SIGNAL conexion: std_logic;

BEGIN

    -- Conexión del divisor de frecuencia al pulso de reloj de la tarjeta
	divisor: divisor_frecuencia PORT MAP (
		CLK => CLK,
		pulso => conexion
	);

    -- Conexión del contador de anillo a la salida del divisor de frecuencia
	contador: contador_anillo PORT MAP (
		CLK => conexion,
		S => S
	);
	
END ciclo_maquina;
