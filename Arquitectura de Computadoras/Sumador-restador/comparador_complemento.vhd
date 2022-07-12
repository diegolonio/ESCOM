LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY comparador_complemento IS
    PORT (
        A, B : IN std_logic_vector (3 DOWNTO 0);
        SA, SB : IN std_logic;
        complementar : OUT std_logic
    );
END comparador_complemento;

ARCHITECTURE comparador_complemento OF comparador_complemento IS
BEGIN
    PROCESS (SA, SB, A, B)
    BEGIN
        IF (SA = '0' AND SB = '0') THEN
            complementar <= '0';
        ELSIF (SA = '0' AND SB = '1') THEN
            IF (A > B) THEN
                complementar <= '0';
            ELSE
                complementar <= '1';
            END IF;
        ELSIF (SA = '1' AND SB = '0') THEN
            IF (B > A) THEN
                complementar <= '0';
            ELSE
                complementar <= '1';
            END IF;
        ELSE
            complementar <= '1';
        END IF;
    END PROCESS;
END comparador_complemento;
