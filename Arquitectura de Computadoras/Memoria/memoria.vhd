LIBRARY ieee;
USE ieee.std_logic_1164.all;
USE ieee.std_logic_unsigned.all;
USE ieee.numeric_std.all;

ENTITY memoria IS
    GENERIC (
        tam_palabra: INTEGER := 4;
        bits_direcciones: INTEGER := 2
    );

    PORT (
        hab_mem, hab_esc: IN std_logic;
        dir_mem: IN std_logic_vector (bits_direcciones-1 DOWNTO 0);
        dato_en: IN std_logic_vector (tam_palabra-1 DOWNTO 0);
        dato_sa: OUT std_logic_vector (tam_palabra-1 DOWNTO 0)
        bandera: OUT std_logic
    );
END memoria;

ARCHITECTURE memoria OF memoria IS
    -- Para convertir la dirección de memoria en un índice
    SIGNAL posicion: INTEGER;
    -- Declaración del tipo Memoria: memoria de 2^{bits_direcciones} localidades de {tam_palabra} bits
    TYPE Memoria IS ARRAY (0 TO 2**bits_direcciones-1) OF std_logic_vector (tam_palabra-1 DOWNTO 0);
    SIGNAL entrada, salida: std_logic_vector (tam_palabra-1 DOWNTO 0) := (OTHERS => '0');
    SIGNAL direccion: std_logic_vector (bits_direcciones-1 DOWNTO 0) := (OTHERS => '0');
    -- Creación de la memoria
    SIGNAL mem: Memoria;
BEGIN
    direccion <= dir_mem;
    entrada <= dato_en;

    PROCESS (hab_mem, hab_esc, direccion, entrada, mem, posicion) BEGIN
        IF (hab_mem = '1') THEN
            posicion <= TO_INTEGER(UNSIGNED(direccion));

            IF (hab_esc = '1') THEN
                mem(posicion) <= entrada;
                salida <= (OTHERS => '0');
            ELSE
                salida <= mem(posicion);
            END IF;

            bandera <= '1';
        ELSE
            salida <= (OTHERS => '0');
            bandera <= '0';
        END IF;
    END PROCESS;

    dato_sa <= salida;
END memoria;
