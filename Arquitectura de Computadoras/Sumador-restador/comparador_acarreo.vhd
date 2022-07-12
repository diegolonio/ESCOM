LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY comparador_acarreo IS
    PORT (
        A, BC2 : IN std_logic_vector (3 DOWNTO 0);
        SA, SB : IN std_logic;
        Coutx : IN std_logic;
        Cout : OUT std_logic
    );
END comparador_acarreo;

ARCHITECTURE comparador_acarreo OF comparador_acarreo IS
BEGIN
    PROCESS (SA, SB, A, BC2, Coutx) BEGIN
        IF (SA = '0' AND SB = '0') THEN
            Cout <= Coutx;
        ELSIF ((SA = '0' AND SB = '1') OR (SA = '1' AND SB = '0')) THEN
            Cout <= '0';
        ELSE
            IF (A > "0000" AND BC2 > "0000") THEN
                IF (A = BC2) THEN
                	Cout <= Coutx;
                ELSE
                	Cout <= NOT Coutx;
                END IF;
            ELSE
                Cout <= '0';
            END IF;
        END IF;
    END PROCESS;
END comparador_acarreo;
