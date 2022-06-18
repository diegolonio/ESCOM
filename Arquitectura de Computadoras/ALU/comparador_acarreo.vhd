LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY comparador_acarreo IS
	PORT (
		S : IN std_logic_vector (3 DOWNTO 0);
		Cin, Coutx : IN std_logic;
		Cout : OUT std_logic
	);
END comparador_acarreo;

ARCHITECTURE comparador_acarreo OF comparador_acarreo IS
BEGIN
	PROCESS (S, Cin, Coutx) BEGIN
		CASE (S) IS
			-- A - B
			WHEN "0001" =>
				IF (Cin = '1') THEN
					Cout <= '0';
				ELSE
					Cout <= Coutx;
				END IF;
			-- B - A
			WHEN "0100" =>
				IF (Cin = '1') THEN
	 		    	Cout <= '0';
				ELSE
					Cout <= Coutx;
				END IF;
			-- A - 1
			WHEN "0011" => Cout <= '0';
			-- B - 1
			WHEN "1100" => Cout <= '0';
			WHEN OTHERS => Cout <= Coutx;
		END CASE;
	END PROCESS;
END comparador_acarreo;
