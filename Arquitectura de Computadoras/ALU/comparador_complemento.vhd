LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY comparador_complemento IS
	PORT (
		A, B : IN std_logic_vector (1 DOWNTO 0);
		S : IN std_logic_vector (3 DOWNTO 0);
		Cin : IN std_logic;
		complementar : OUT std_logic
	);
END comparador_complemento;

ARCHITECTURE comparador_complemento OF comparador_complemento IS
BEGIN
	PROCESS (A, B, S, Cin) BEGIN
		CASE (S) IS
			-- A - B
			WHEN "0001" =>
				IF (Cin = '1') THEN
	 		    	IF (A > B) THEN
						complementar <= '0';
					ELSE
						complementar <= '1';
					END IF;
				ELSE
					complementar <= '0';
				END IF;
			-- B - A
			WHEN "0100" =>
				IF (Cin = '1') THEN
	 		    	IF (B > A) THEN
						complementar <= '0';
					ELSE
						complementar <= '1';
					END IF;
				ELSE
					complementar <= '0';
				END IF;
			-- A - 1
			WHEN "0011" =>
				IF (Cin = '0') THEN
					IF (A = "00") THEN
						complementar <= '1';
					ELSE
						complementar <= '0';
					END IF;
				ELSE
					complementar <= '0';
				END IF;
			-- B - 1
			WHEN "1100" =>
				IF (Cin = '0') THEN
					IF (B = "00") THEN
						complementar <= '1';
					ELSE
						complementar <= '0';
					END IF;
				ELSE
					complementar <= '0';
				END IF;
			WHEN OTHERS => complementar <= '0';
		END CASE;
	END PROCESS;
END comparador_complemento;
