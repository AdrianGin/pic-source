--! @file
--! @brief Test baud_generator module

--! Use standard library and import the packages (std_logic_1164,std_logic_unsigned,std_logic_arith)
library ieee;
use ieee.std_logic_1164.all;
--use ieee.std_logic_arith.all;
use ieee.numeric_std.all;


ENTITY FIFO IS

	GENERIC (
		CONSTANT DATA_WIDTH  : positive := 8;
		CONSTANT FIFO_DEPTH	: positive := 16
	);

	PORT ( dataIn : IN std_logic_vector(DATA_WIDTH-1 downto 0);
			 wen	  : IN std_logic;
			 ren	  : IN std_logic;
			 clk	  : IN std_logic;
			 rst	  : IN std_logic;
			 
			 
			 empty	: OUT std_logic;
			 full	   : OUT std_logic;
			 dataOut : OUT std_logic_vector(DATA_WIDTH-1 downto 0)
			 

			 );

END;


ARCHITECTURE behaviour OF FIFO IS
  SIGNAL iempty : std_logic := '1';
	BEGIN
	PROCESS(clk, iempty, wen, ren)
		TYPE fifo_element IS array(0 to FIFO_DEPTH - 1) OF STD_LOGIC_VECTOR(DATA_WIDTH-1 downto 0);
		VARIABLE Memory : fifo_element;
		VARIABLE wPointer  : unsigned(3 downto 0) := (others => '0');
		VARIABLE rPointer  : unsigned(3 downto 0) := (others => '0');
	  
	BEGIN
	
	  empty <= iempty;
	
		IF rising_edge(clk) THEN
		
			IF rst = '1' THEN
				wPointer := (others => '0');
				rPointer := (others => '0');
				
				iempty <= '1';
				full <= '0';
			END IF;
			
			IF wPointer /= rPointer THEN
				iempty <= '0';
			ELSE
				iempty <= '1';
			END IF;
			
			IF wPointer >= (rPointer) THEN
				IF (wPointer - rPointer) = FIFO_DEPTH THEN
					full <= '1';
				ELSE
					full <= '0';
				END IF;
			ELSE
				IF rPointer - wPointer = 1 THEN
					full <= '1';
				ELSE
					full <= '0';
				END IF;
			END IF;	
			
				
			IF (wen = '1') THEN
				  
				Memory( to_integer(wPointer) ) := dataIn;
				wPointer := wPointer + 1;
				
				IF wPointer = FIFO_DEPTH THEN
					wPointer := (others => '0');
				END IF;
			END IF;
					
			IF (ren = '1') and (iempty = '0') THEN
				dataOut <= Memory( to_integer(rPointer));
				rPointer := rPointer + 1;
				IF rPointer = FIFO_DEPTH THEN
					rPointer := (others => '0');
				END IF;						
			END IF;

		END IF;
		
		

				

	END PROCESS;


END;