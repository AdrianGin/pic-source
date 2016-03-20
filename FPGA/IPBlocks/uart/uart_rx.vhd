LIBRARY ieee;
USE ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

ENTITY uart_rx IS

--baud8x needs to be 8x the desired baud rate period.
PORT ( 
	    rst 			    : IN STD_LOGIC;
		 baud8x			 : IN STD_LOGIC;
		 rx_pin	       : IN STD_LOGIC := '0';
		 
		 data			 	 : OUT STD_LOGIC_VECTOR(7 downto 0);
		 data_ready     : OUT STD_LOGIC := '0');
		 
END;


ARCHITECTURE behaviour OF uart_rx IS
	TYPE UART_RX_STATES IS (idle, startbit, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7, stopbit, stopbit_end);

	TYPE UART_RX_INIT_STATES IS (wait_sb, wait_ticks, wait_nextsample, wait_stopb);
	
	
	SIGNAL current_s, next_s : UART_RX_STATES;
	SIGNAL init_s, next_init_s : UART_RX_INIT_STATES;

	SIGNAL dataOut : STD_LOGIC_VECTOR(7 downto 0) := (others => '0');
	SIGNAL bitCount : natural range 0 to 8 := 0;
	BEGIN 
		PROCESS(rst, rx_pin, baud8x)
			BEGIN
			--IF rst = '1' THEN			
			--	current_s <= idle;
			--	init_s <= wait_sb;
			--	next_init_s <= wait_sb;
			--ELSIF falling_edge(rx_pin) THEN
			--	next_init_s <= wait_4ticks;
			--	current_s <= next_s;
			--END IF;
		END PROCESS;
		
		PROCESS(rst, rx_pin, init_s, baud8x)
			VARIABLE tickCount : std_logic_vector(3 downto 0) := (others => '0');
			--VARIABLE bitCount : natural range 0 to 8 := 0;
			
			BEGIN
			

				IF rst = '1' THEN
					init_s <= wait_sb;
			      next_init_s <= wait_sb;
			     
	
			
				ELSIF rising_edge(baud8x) THEN
					init_s <= next_init_s;
				
					CASE init_s IS
					
							WHEN wait_sb =>
							  IF rx_pin = '0' THEN

								  next_init_s <= wait_ticks;
							  END IF;
								tickCount := (others => '0');
								bitCount <= 0;
						
							WHEN wait_ticks =>
							
								tickCount := tickCount + 1;
							
								IF rx_pin /= '0' THEN
									tickCount := (others => '0');
									init_s <= wait_sb;
									next_init_s <= wait_sb;
								END IF;
								
								IF tickCount = 3 THEN
									data_ready <= '0';
									next_init_s <= wait_nextsample;
									tickCount := (others => '0');
								END IF;
						
						
							WHEN wait_nextsample =>
							
								tickCount := tickCount + 1;
								IF tickCount = 8 THEN
									dataOut(bitCount) <= rx_pin;
									bitCount <= bitCount + 1;
									tickCount := (others => '0');
									
									IF bitCount = 7 THEN
									   next_init_s <= wait_stopb;
								  END IF;
									
								END IF;
								

							
							
							WHEN wait_stopb =>
							  tickCount := tickCount + 1;
							  IF tickCount = 8 THEN
								  tickCount := (others => '0');
								  IF rx_pin = '1' THEN
									  data <= dataOut;
									  data_ready <= '1';
								  ELSE
									  data_ready <= '0';
								  END IF;
								  next_init_s <= wait_sb;
							  END IF;
								  

							WHEN others =>
							
					END CASE;
					
				END IF;
					
			
		END PROCESS;


END;
