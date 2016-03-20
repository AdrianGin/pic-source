library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;


ENTITY buffered_uart_tx IS

	PORT ( 
			 rst 				 : IN STD_LOGIC;
			 clk				 : IN STD_LOGIC;
			 baud				 : IN STD_LOGIC; 
			 tx_start		 : IN STD_LOGIC := '0';
			 dataIn			 : IN STD_LOGIC_VECTOR(7 downto 0);
			 
			 
			 empty : OUT STD_LOGIC := '1';
			 full : OUT STD_LOGIC := '0';			 
			 tx_pin_out   	 : OUT STD_LOGIC := '0');

END;




ARCHITECTURE behaviour OF buffered_uart_tx IS

	SIGNAL iren : std_logic := '0';
	SIGNAL iwen : std_logic := '0';
	
	SIGNAL tx_ready  		 : STD_LOGIC := '0';
	SIGNAL iempty : std_logic := '1';
	
	
	
	SIGNAL tx_retrig : std_logic := '1';
	SIGNAL dataByte : STD_LOGIC_VECTOR(7 downto 0);
	TYPE buffered_uart_tx_t IS (idle, wait_init, transmitting);
	TYPE write_en_toggle_t IS (ready, not_ready);
	
	
	SIGNAL current_s : buffered_uart_tx_t;
	SIGNAL write_en_toggle : write_en_toggle_t;



BEGIN

	
	
	fifo : ENTITY work.FIFO 
	
	generic map (
		DATA_WIDTH => 8,
		FIFO_DEPTH => 16
	)	
	port map (
		rst => rst,
		clk => clk,
		wen => iwen,
		ren => iren,
		empty => iempty,
		full => full,
		
		dataIn => dataIn,
		dataOut => dataByte
		
	
		);

	tx_module: ENTITY work.uart_tx port map (
		rst => tx_retrig,
		baud => baud,
		data => dataByte,
		tx_out => tx_pin_out,
		byte_sent => tx_ready
		);
		
		
	PROCESS(clk, current_s, iempty, tx_ready)
		BEGIN
		  
		empty <= iempty;
		  
		IF rising_edge(clk) THEN
		
			CASE write_en_toggle IS
				WHEN ready =>
					IF tx_start = '1' THEN
						iwen <= '1';
						write_en_toggle <= not_ready;
					END IF;
		
				WHEN not_ready =>
					iwen <= '0';
					IF tx_start = '0' THEN
						write_en_toggle <= ready;
					END IF;
					
			END CASE;
		  
			CASE current_s IS
				WHEN idle =>
					IF (iempty = '0') THEN
						iren <= '1';
						tx_retrig <= '0';
						current_s <= wait_init;
					ELSE
						iren <= '0';
						tx_retrig <= '1';
					END IF;
				
				WHEN wait_init =>
					iren <= '0';
					IF ( tx_ready = '0' ) THEN
      				  current_s <= transmitting;
  				  END IF;

				WHEN transmitting =>        
					IF (tx_ready = '1') THEN
						IF (iempty = '1') THEN
							iren <= '0';
							tx_retrig <= '1';
							current_s <= idle;
						ELSE
						  current_s <= idle;
						END IF;
						
						
						
					END IF; 
				END CASE;
			END IF;
			
										
		
	END PROCESS;



END;

