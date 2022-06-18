LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY alu IS
	PORT (
		-- S(1) y S(0) son controles de B
		-- S(3) y S(2) son controles de A
		-- S(5) y S(4) son controles del multiplexor de la salida
		S : IN std_logic_vector (5 DOWNTO 0);
		A, B : IN std_logic_vector (1 DOWNTO 0);
		Cin : IN std_logic;
		F : OUT std_logic_vector (1 DOWNTO 0);
		Cout : OUT std_logic
	);
END alu;

ARCHITECTURE alu OF alu IS

	-- Controles de A y B
	COMPONENT controles
		PORT (
			A, B : IN std_logic_vector (1 DOWNTO 0);
			S : IN std_logic_vector (3 DOWNTO 0);
			Ax, Bx : OUT std_logic_vector (1 DOWNTO 0)
		);
	END COMPONENT;

	-- Sumador completo de 2 bits
	COMPONENT sumador_completo_2_bits
		PORT (
			A, B : IN std_logic_vector (1 DOWNTO 0);
			Cin : IN std_logic;
			Z : OUT std_logic_vector (1 DOWNTO 0);
			Cout : OUT std_logic
		);
	END COMPONENT;

	-- Complementador de la salida
	COMPONENT complemento
		PORT (
			E : IN std_logic_vector (1 DOWNTO 0);
			complementar : IN std_logic;
			Ex : OUT std_logic_vector (1 DOWNTO 0)
		);
	END COMPONENT;

	-- Comparador del complementador
	COMPONENT comparador_complemento
		PORT (
			A, B : IN std_logic_vector (1 DOWNTO 0);
			S : IN std_logic_vector (3 DOWNTO 0);
			Cin : IN std_logic;
			complementar : OUT std_logic
		);
	END COMPONENT;

	-- Comparador del acarreo
	COMPONENT comparador_acarreo
		PORT (
			S : IN std_logic_vector (3 DOWNTO 0);
			Cin, Coutx : IN std_logic;
			Cout : OUT std_logic
		);
	END COMPONENT;

	-- Compuertas lógicas
	COMPONENT compuertas_logicas
		PORT (
			A, B : IN std_logic_vector (1 DOWNTO 0);
			Sand, Sor, Sxor : OUT std_logic_vector (1 DOWNTO 0)
		);
	END COMPONENT;

	-- Multiplexor de la salida de la ALU
	COMPONENT multiplexor
		PORT (
			S, Eand, Eor, Exor, Esuma : IN std_logic_vector (1 DOWNTO 0);
			salida : OUT std_logic_vector (1 DOWNTO 0)
		);
	END COMPONENT;

	SIGNAL Ax, Bx, Zx, Zy, andx, orx, xorx : std_logic_vector (1 DOWNTO 0);
	SIGNAL Coutx, Couty, complementar : std_logic;

BEGIN

	-- Conexión de los controles
	controles_A_B: controles PORT MAP (
		A => A,
		B => B,
		S => S(3 DOWNTO 0),
		Ax => Ax,
		Bx => Bx
	);

	-- Conexión del sumador completo
	sumador: sumador_completo_2_bits PORT MAP (
		Cin => Cin,
		A => Ax,
		B => Bx,
		Z => Zx,
		Cout => Coutx
	);

	-- Conexión del comparador del complementador de la salida
	complementar_salida: comparador_complemento PORT MAP (
		A => A,
		B => B,
		S => S(3 DOWNTO 0),
		Cin => Cin,
		complementar => complementar
	);

	-- Conexión del complementador de la salida
	complementador: complemento PORT MAP (
		E => Zx,
		complementar => complementar,
		Ex => Zy
	);

	-- Conexión del comparador del acarreo
	acarreo: comparador_acarreo PORT MAP (
		S => S(3 DOWNTO 0),
		Cin => Cin,
		Coutx => Coutx,
		Cout => Couty
	);

	-- Conexión de las compuertas lógicas
	logicas: compuertas_logicas PORT MAP (
		A => Ax,
		B => Bx,
		Sand => andx,
		Sor => orx,
		Sxor => xorx
	);

	-- Conexión del multiplexor
	seleccionar_salida: multiplexor PORT MAP (
		S => S(5 DOWNTO 4),
		Eand => andx,
		Eor => orx,
		Exor => xorx,
		Esuma => Zy,
		salida => F
	);

	Cout <= S(5) and S(4) and Couty;

END alu;
