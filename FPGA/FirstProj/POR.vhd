library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;


ENTITY POR IS
	PORT( clk 		: IN STD_LOGIC;
			POR		: OUT STD_LOGIC := '0');

END;


ARCHITECTURE behaviour of POR IS

BEGIN
	PROCESS ( clk )
	
	variable cnt  : UNSIGNED (4 downto 0);

	BEGIN
		IF clk'EVENT AND clk = '1' THEN
			
			IF cnt < 14 THEN
				POR <= '1';
				cnt := cnt + 1;
			ELSE
				POR <= '0';
			END IF;
		END IF;
		
	END PROCESS;

END behaviour;