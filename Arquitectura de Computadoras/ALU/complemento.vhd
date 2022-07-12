LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY complemento IS
    PORT (
        E : IN std_logic_vector (1 DOWNTO 0);
        complementar : IN std_logic;
        Ex : OUT std_logic_vector (1 DOWNTO 0)
    );
END complemento;

ARCHITECTURE complemento OF complemento IS
BEGIN
    PROCESS (E, complementar) BEGIN
        IF (complementar = '0') THEN
            Ex <= E;
        ELSE
            CASE (E) IS
                WHEN "00" => Ex <= "00";
                WHEN "01" => Ex <= "11";
                WHEN "10" => Ex <= "10";
                WHEN OTHERS => Ex <= "01";
            END CASE;
        END IF;
    END PROCESS;
END complemento;
