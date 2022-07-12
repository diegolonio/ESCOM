LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY contador_anillo IS
    PORT (
        CLK: IN std_logic;
        S: OUT std_logic_vector (4 DOWNTO 0)
    );
END contador_anillo;

ARCHITECTURE contador_anillo OF contador_anillo IS
BEGIN
    PROCESS (CLK)
        VARIABLE contador: INTEGER RANGE 0 TO 4 := 0;
    BEGIN
        IF (CLK'EVENT AND CLK = '1') THEN
            CASE contador IS
                WHEN 0 => S <= "00001";
                WHEN 1 => S <= "00010";
                WHEN 2 => S <= "00100";
                WHEN 3 => S <= "01000";
                WHEN OTHERS => S <= "10000";
            END CASE;

            contador := contador + 1;
        END IF;
    END PROCESS;
END contador_anillo;
