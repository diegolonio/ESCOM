LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY sumador_completo_4_bits IS
	PORT (
		A, B : IN std_logic_vector (3 DOWNTO 0);
		Cin : IN std_logic;
		Z : OUT std_logic_vector (3 DOWNTO 0);
		Cout : OUT std_logic
	);
END sumador_completo_4_bits;

ARCHITECTURE sumador_completo_4_bits OF sumador_completo_4_bits IS

	COMPONENT sumador_completo_1_bit
		PORT (
			A, B, Cin : IN std_logic;
			Z, Cout : OUT std_logic
		);
	END COMPONENT;

	SIGNAL C0, C1, C2 : std_logic;

BEGIN

	bit_0: sumador_completo_1_bit PORT MAP (
		Cin => Cin,
		A => A(0),
		B => B(0),
		Z => Z(0),
		Cout => C0
	);

	bit_1: sumador_completo_1_bit PORT MAP (
		Cin => C0,
		A => A(1),
		B => B(1),
		Z => Z(1),
		Cout => C1
	);

	bit_2: sumador_completo_1_bit PORT MAP (
		Cin => C1,
		A => A(2),
		B => B(2),
		Z => Z(2),
		Cout => C2
	);

	bit_3: sumador_completo_1_bit PORT MAP (
		Cin => C2,
		A => A(3),
		B => B(3),
		Z => Z(3),
		Cout => Cout
	);
	
END sumador_completo_4_bits;
