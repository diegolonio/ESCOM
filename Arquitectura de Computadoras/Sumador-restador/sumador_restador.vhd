LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY sumador_restador IS
	PORT (
		A, B : IN std_logic_vector (3 DOWNTO 0);
		SA, SB, Cin : IN std_logic;
		Z : OUT std_logic_vector (3 DOWNTO 0);
		Cout, signo : OUT std_logic
	);
END sumador_restador;

ARCHITECTURE sumador_restador OF sumador_restador IS
	
	-- Sumador completo de 4 bits
	COMPONENT sumador_completo_4_bits
		PORT (
			A, B : IN std_logic_vector (3 DOWNTO 0);
			Cin : IN std_logic;
			Z : OUT std_logic_vector (3 DOWNTO 0);
			Cout : OUT std_logic
		);
	END COMPONENT;

	-- Complementador
	COMPONENT complemento
		PORT (
			E : IN std_logic_vector (3 DOWNTO 0);
			complementar : IN std_logic;
			Ex : OUT std_logic_vector (3 DOWNTO 0)
		);
	END COMPONENT;

	-- Comparador del complemento de la salida
	COMPONENT comparador_complemento
		PORT (
			A, B : IN std_logic_vector (3 DOWNTO 0);
			SA, SB : IN std_logic;
			complementar : OUT std_logic
		);
	END COMPONENT;

	-- Comparador del acarreo
	COMPONENT comparador_acarreo
		PORT (
			A, BC2 : IN std_logic_vector (3 DOWNTO 0);
			SA, SB : IN std_logic;
			Coutx : IN std_logic;
			Cout : OUT std_logic
		);
	END COMPONENT;

	-- Comparador del signo
	COMPONENT comparador_signo
		PORT (
			A, B : IN std_logic_vector (3 DOWNTO 0);
			SA, SB : IN std_logic;
			signo : OUT std_logic
		);
	END COMPONENT;

	SIGNAL Ax, Bx, Zx : std_logic_vector (3 DOWNTO 0);
	SIGNAL Coutx, complementar : std_logic;

BEGIN

	-- Conexión de A y B a los complementadores
	-- El signo se usa para determinar si la entrada se complementa a 2 o no
	entrada_A: complemento PORT MAP(
		E => A,
		complementar => SA,
		Ex => Ax
	);

	entrada_B: complemento PORT MAP(
		E => B,
		complementar => SB,
		Ex => Bx
	);

	-- Conexión de Ax y Bx al sumador completo de 4 bits
	sumador: sumador_completo_4_bits PORT MAP(
		Cin => Cin,
		A => Ax,
		B => Bx,
		Z => Zx,
		Cout => Coutx
	);

	-- Conexión de Zx al complementador de la salida
	salida: complemento PORT MAP(
		E => Zx,
		complementar => complementar,
		Ex => Z
	);

	-- Conexión del comparador del complemento de la salida al complementador de la salida
	comparador_salida: comparador_complemento PORT MAP(
		A => A,
		B => B,
		SA => SA,
		SB => SB,
		complementar => complementar
	);

	-- Conexión del acarreo de salida del sumador completo al comparador del acarreo de salida
	acarreo_salida: comparador_acarreo PORT MAP(
		A => A,
		BC2 => Bx,
		SA => SA,
		SB => SB,
		Coutx => Coutx,
		Cout => Cout
	);

	-- Conexión del comparador del signo
	signo_resultado: comparador_signo PORT MAP(
		A => A,
		B => B,
		SA => SA,
		SB => SB,
		signo => signo
	);

END sumador_restador;
