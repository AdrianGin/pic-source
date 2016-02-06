LIBRARY ieee;
USE ieee.std_logic_1164.all;
use IEEE.numeric_std.ALL;

ENTITY counter IS
GENERIC ( bit_depth : INTEGER := 8 );
PORT ( clk : IN STD_LOGIC ;
		 qout : OUT STD_LOGIC_VECTOR(bit_depth-1 downto 0)
		 );

END;

ARCHITECTURE counter_8 OF counter IS
	SIGNAL   cnt: UNSIGNED(bit_depth-1 downto 0) := (OTHERS => '0');

BEGIN
	PROCESS(clk)
		BEGIN
			IF clk'EVENT AND clk = '1' THEN
				cnt <= cnt + 1;
			END IF;
	END PROCESS;
	
	qout <=  STD_LOGIC_VECTOR(cnt);
	
END counter_8;