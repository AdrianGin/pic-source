LIBRARY ieee;
USE ieee.std_logic_1164.all;
use IEEE.numeric_std.ALL;

ENTITY counter_down IS
GENERIC ( bit_depth : INTEGER := 8;
			 init_val  : INTEGER := 99999800);
PORT ( clk : IN STD_LOGIC ;
		 qout : OUT STD_LOGIC_VECTOR(bit_depth-1 downto 0)
		 );

END;

ARCHITECTURE counter_8 OF counter_down IS
	SIGNAL   cnt: UNSIGNED(bit_depth-1 downto 0) := to_unsigned(init_val, bit_depth);

BEGIN
	PROCESS(clk)
		BEGIN
			IF clk'EVENT AND clk = '1' THEN
				cnt <= cnt - 1;
			END IF;
	END PROCESS;
	
	qout <=  STD_LOGIC_VECTOR(cnt);
	
END counter_8;