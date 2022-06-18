LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY complemento IS
	PORT (
		E : IN std_logic_vector (3 DOWNTO 0);
		complementar : IN std_logic;
		Ex : OUT std_logic_vector (3 DOWNTO 0)
	);
END complemento;

ARCHITECTURE complemento OF complemento IS
BEGIN
	PROCESS (E, complementar) BEGIN
		IF (complementar = '0') THEN
			Ex <= E;
		ELSE
			CASE (E) IS
				WHEN "0000" => Ex <= "0000";
				WHEN "0001" => Ex <= "1111";
				WHEN "0010" => Ex <= "1110";
				WHEN "0011" => Ex <= "1101";
				WHEN "0100" => Ex <= "1100";
				WHEN "0101" => Ex <= "1011";
				WHEN "0110" => Ex <= "1010";
				WHEN "0111" => Ex <= "1001";
				WHEN "1000" => Ex <= "1000";
				WHEN "1001" => Ex <= "0111";
				WHEN "1010" => Ex <= "0110";
				WHEN "1011" => Ex <= "0101";
				WHEN "1100" => Ex <= "0100";
				WHEN "1101" => Ex <= "0011";
				WHEN "1110" => Ex <= "0010";
				WHEN OTHERS => Ex <= "0001";
			END CASE;
		END IF;
	END PROCESS;
END complemento;