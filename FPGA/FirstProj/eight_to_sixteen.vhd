LIBRARY ieee;
USE ieee.std_logic_1164.all;


ENTITY eight_to_sixteen IS
	PORT( in8 	: IN STD_LOGIC_VECTOR(7 downto 0);
			out16 : OUT STD_LOGIC_VECTOR(15 downto 0));

END;

ARCHITECTURE structure OF eight_to_sixteen IS

BEGIN
	out16 <= X"00" & in8(7 downto 0);
	
END structure;