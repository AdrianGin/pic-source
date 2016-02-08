LIBRARY ieee;
USE ieee.std_logic_1164.all;
use IEEE.numeric_std.ALL;

ENTITY timer IS

PORT ( clk : IN STD_LOGIC;
		 div : IN STD_LOGIC_VECTOR(15 downto 0);
		 rst : IN STD_LOGIC;
		 clkOut : OUT STD_LOGIC);
		 
END;


ARCHITECTURE behaviour OF timer IS
	SIGNAL cmp : UNSIGNED(15 downto 0) := X"0000";
	SIGNAL usdiv : UNSIGNED(15 downto 0) := X"0001";
	SIGNAL outBuf : STD_LOGIC := '0';
BEGIN
	
	PROCESS(rst, clk, div, cmp)
	BEGIN
	
		IF clk'EVENT AND clk = '1' AND rst = '1' THEN
			cmp <= X"0000";
		ELSIF clk'EVENT AND clk = '1' AND rst = '0' THEN
			usdiv <= unsigned(div) - 1;
			
			IF cmp = usdiv THEN
				cmp <= X"0000";
				outBuf <= NOT outBuf;
			ELSE
				cmp <= cmp + 1;
			END IF;
		END IF;
		
	END PROCESS;
	
	clkOut <= outBuf;
	
	
END behaviour;