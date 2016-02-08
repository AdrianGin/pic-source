LIBRARY ieee;
USE ieee.std_logic_1164.all;
use IEEE.numeric_std.ALL;

ENTITY counter IS
GENERIC ( bit_depth : INTEGER := 8;
			 init_val  : INTEGER := 00000);
PORT ( e 	: IN STD_LOGIC ;
		 clr 	: IN STD_LOGIC;
		 clk 	: IN STD_LOGIC ;
		 qout : OUT STD_LOGIC_VECTOR(bit_depth-1 downto 0)
		 );

END;

ARCHITECTURE behaviour OF counter IS
	SIGNAL   cnt: UNSIGNED(bit_depth-1 downto 0) := to_unsigned(init_val, bit_depth);

BEGIN
	PROCESS(clk, clr)
		BEGIN
			IF clk'EVENT AND clk = '1' AND clr = '1' THEN
				cnt <= (OTHERS => '0');
			ELSIF clk'EVENT AND clk = '1' AND e = '1' AND clr = '0' THEN
				cnt <= cnt + 1;				
			END IF;
				
	END PROCESS;

	
	qout <=  STD_LOGIC_VECTOR(cnt);
	
END behaviour;