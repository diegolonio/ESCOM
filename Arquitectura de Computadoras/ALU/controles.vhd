LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY controles IS
    PORT (
        A, B : IN std_logic_vector (1 DOWNTO 0);
        S : IN std_logic_vector (3 DOWNTO 0);
        Ax, Bx : OUT std_logic_vector (1 DOWNTO 0)
    );
END controles;

ARCHITECTURE controles OF controles IS
BEGIN
    PROCESS (A, B, S) BEGIN
    -- S(1) y S(0) son los selectores de B
        CASE (S(1 DOWNTO 0)) IS
            WHEN "00" => Bx <= B;
            WHEN "01" => Bx <= NOT B;
            WHEN "10" => Bx <= "00";
            WHEN OTHERS => Bx <= "11";
        END CASE;
    -- S(2) Y S(3) son los selecotres de A
        CASE (S(3 DOWNTO 2)) IS
            WHEN "00" => Ax <= A;
            WHEN "01" => Ax <= NOT A;
            WHEN "10" => Ax <= "00";
            WHEN OTHERS => Ax <= "11";
        END CASE;
    END PROCESS;
END controles;
