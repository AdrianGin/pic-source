LIBRARY ieee;
USE ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

ENTITY uart_brgen IS

PORT ( 
	    rst 			 : IN STD_LOGIC := '0';
		 clk 			 : IN STD_LOGIC := '0';
		 div 			 : IN STD_LOGIC_VECTOR(15 downto 0) := (others => '0');
		 baudrate_8x : OUT STD_LOGIC := '0';
		 baudrate 	 : OUT STD_LOGIC := '0');
		 
END;


ARCHITECTURE behaviour OF uart_brgen IS

BEGIN

	--baudrate
	PROCESS(rst, clk, div)
	VARIABLE cnt : STD_LOGIC_VECTOR(15 downto 0); 
	VARIABLE half_cycle : STD_LOGIC_VECTOR(15 downto 0); 
	BEGIN
		half_cycle := '0' & div(15 downto 1);
		IF rst = '1' THEN
			cnt := (others => '0');
			baudrate <= '0';
		ELSIF clk'EVENT AND clk = '1' THEN
			cnt := cnt + conv_std_logic_vector(1, 16);
			
			IF cnt = half_cycle THEN
				baudrate <= '0';
			ELSIF cnt = div
			 THEN
				baudrate <= '1';
				cnt := (others => '0');
			END IF;
		END IF;
	END PROCESS;
	
	--baudrate 8x
	PROCESS(rst, clk, div)
	VARIABLE cnt : STD_LOGIC_VECTOR(15 downto 0); 
	VARIABLE half_cycle : STD_LOGIC_VECTOR(15 downto 0); 
	VARIABLE full_cycle : STD_LOGIC_VECTOR(15 downto 0); 
	
	
	BEGIN
		half_cycle := "0000" & div(15 downto 4);
		full_cycle := "000"  & div(15 downto 3);
		
		IF rst = '1' THEN
			cnt := (others => '0');
			baudrate_8x <= '0';
		ELSIF clk'EVENT AND clk = '1' THEN
			cnt := cnt + conv_std_logic_vector(1, 16);
			
			IF cnt = half_cycle THEN
				baudrate_8x <= '0';
			ELSIF cnt = full_cycle
			 THEN
				baudrate_8x <= '1';
				cnt := (others => '0');
			END IF;
		END IF;
	END PROCESS;


END;
