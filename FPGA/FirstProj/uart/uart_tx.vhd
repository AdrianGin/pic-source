LIBRARY ieee;
USE ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;

ENTITY uart_tx IS

PORT ( 
	    rst 			 : IN STD_LOGIC;
		 baud			 : IN STD_LOGIC;
		 data			 : IN STD_LOGIC_VECTOR(7 downto 0);
		 tx_out	    : OUT STD_LOGIC := '0';
		 byte_sent   : OUT STD_LOGIC := '0');
		 
END;


ARCHITECTURE behaviour OF uart_tx IS
	TYPE UART_TX_STATES IS (idle, startbit, bit0, bit1, bit2, bit3, bit4, bit5, bit6, bit7, stopbit, stopbit_end);

	SIGNAL current_s, next_s : UART_TX_STATES;

	
	BEGIN 
		PROCESS(rst, baud)
			BEGIN
			IF rst = '1' THEN			
				current_s <= idle;
			ELSIF rising_edge(baud) THEN
				current_s <= next_s;
			END IF;
		END PROCESS;
		
		
		PROCESS( current_s, data )
		BEGIN
			CASE current_s IS
				WHEN idle =>
					tx_out <= '1';
					next_s <= startbit;
					byte_sent <= '1';
										
				WHEN startbit =>
					tx_out <= '0';
					next_s <= bit0;
					byte_sent <= '0';
					
				WHEN bit0 =>
					tx_out <= data(0);
					next_s <= bit1;
					byte_sent <= '0';
				
				WHEN bit1 =>
					tx_out <= data(1);
					next_s <= bit2;
					byte_sent <= '0';
					
				WHEN bit2 =>
					tx_out <= data(2);
					next_s <= bit3;
					byte_sent <= '0';
				
				WHEN bit3 =>
					tx_out <= data(3);
					next_s <= bit4;
					byte_sent <= '0';
				
				WHEN bit4 =>
					tx_out <= data(4);
					next_s <= bit5;
					byte_sent <= '0';
					
				WHEN bit5 =>
					tx_out <= data(5);
					next_s <= bit6;
					byte_sent <= '0';
					
				WHEN bit6 =>
					tx_out <= data(6);
					next_s <= bit7;
					byte_sent <= '0';
					
				WHEN bit7 =>
					tx_out <= data(7);
					next_s <= stopbit;
					byte_sent <= '0';
					
				WHEN stopbit =>
					tx_out <= '1';
					next_s <= startbit;
					byte_sent <= '1';
					
				WHEN stopbit_end =>
				  byte_sent <= '1';
					next_s <= startbit;
					
			END CASE;
				
		
		END PROCESS;
		


END;
