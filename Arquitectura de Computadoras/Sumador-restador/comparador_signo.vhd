LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY comparador_signo IS
	PORT (
		A, B : IN std_logic_vector (3 DOWNTO 0);
		SA, SB : IN std_logic;
		signo : OUT std_logic
	);
END comparador_signo;

ARCHITECTURE comparador_signo OF comparador_signo IS
	
BEGIN

	PROCESS (SA, SB, A, B)
	BEGIN
		IF (SA = '0' AND SB = '0') THEN
			signo <= '0';
		ELSIF (SA = '0' AND SB = '1') THEN
			IF (A >= B) THEN
				signo <= '0';
			ELSE
				signo <= '1';
			END IF;
		ELSIF (SA = '1' AND SB = '0') THEN
			IF (A > B) THEN
				signo <= '1';
			ELSE
				signo <= '0';
			END IF;
		ELSE
			signo <= '1';
		END IF;
	END PROCESS;

END comparador_signo;
