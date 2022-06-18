LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY sumador_completo_1_bit IS
	PORT (
		A, B, Cin : IN std_logic;
		Z, Cout : OUT std_logic
	);
END sumador_completo_1_bit;

ARCHITECTURE sumador_completo_1_bit OF sumador_completo_1_bit IS
BEGIN
	Z <= (Cin XOR A XOR B);
	Cout <= (A AND B) OR (Cin AND (A XOR B));			
END sumador_completo_1_bit;
