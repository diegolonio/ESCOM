LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY multiplexor IS
	PORT (
		S, Eand, Eor, Exor, Esuma : IN std_logic_vector (1 DOWNTO 0);
		salida : OUT std_logic_vector (1 DOWNTO 0)
	);
END multiplexor;

ARCHITECTURE multiplexor OF multiplexor IS
BEGIN
	PROCESS (S, Eand, Eor, Exor, Esuma) BEGIN
		CASE (S) IS
			WHEN "00" => salida <= Eand;
			WHEN "01" => salida <= Eor;
			WHEN "10" => salida <= Exor;
			WHEN OTHERS => salida <= Esuma;
		END CASE;
	END PROCESS;
END multiplexor;
