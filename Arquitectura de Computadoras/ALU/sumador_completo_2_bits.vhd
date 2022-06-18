LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY sumador_completo_2_bits IS
	PORT (
		A, B : IN std_logic_vector (1 DOWNTO 0);
		Cin : IN std_logic;
		Z : OUT std_logic_vector (1 DOWNTO 0);
		Cout : OUT std_logic
	);
END sumador_completo_2_bits;

ARCHITECTURE sumador_completo_2_bits OF sumador_completo_2_bits IS

	COMPONENT sumador_completo_1_bit
		PORT (
			A, B, Cin : IN std_logic;
			Z, Cout : OUT std_logic
		);
	END COMPONENT;

	SIGNAL C0 : std_logic;

BEGIN

	bit_0: sumador_completo_1_bit PORT MAP(
		Cin => Cin,
		A => A(0),
		B => B(0),
		Z => Z(0),
		Cout => C0
	);

	bit_1: sumador_completo_1_bit PORT MAP(
		Cin => C0,
		A => A(1),
		B => B(1),
		Z => Z(1),
		Cout => Cout
	);
	
END sumador_completo_2_bits;
