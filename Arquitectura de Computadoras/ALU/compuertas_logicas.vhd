LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY compuertas_logicas IS
    PORT (
        A, B : IN std_logic_vector (1 DOWNTO 0);
        Sand, Sor, Sxor : OUT std_logic_vector (1 DOWNTO 0)
    );
END compuertas_logicas;

ARCHITECTURE compuertas_logicas OF compuertas_logicas IS
BEGIN
    Sand <= A AND B;
    Sor <= A OR B;
    Sxor <= A XOR B;
END compuertas_logicas;
