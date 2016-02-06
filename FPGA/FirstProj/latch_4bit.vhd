LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY latch_4bit IS

PORT ( din  : IN STD_LOGIC_VECTOR(3 downto 0); 
		 load : IN STD_LOGIC;
		 dout : OUT STD_LOGIC_VECTOR(3 downto 0));
		
		
END;

ARCHITECTURE structure OF latch_4bit IS

BEGIN
	PROCESS(load)
	BEGIN
		IF (load'EVENT AND load = '1') THEN
			dout <= din;
		END IF;
	
	
	END PROCESS;
	


END structure;