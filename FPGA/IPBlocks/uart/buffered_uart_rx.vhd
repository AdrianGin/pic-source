library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;


ENTITY buffered_uart_rx IS

	PORT ( 
			 rst 				 : IN STD_LOGIC;
			 clk				 : IN STD_LOGIC;
			 baud8x			 : IN STD_LOGIC; 
			 rx_pin_in   	 : IN STD_LOGIC := '0';
			 read_data    : IN STD_LOGIC := '0';
			 
			 data_received  : OUT STD_LOGIC := '0';
			 data_ready		 : OUT STD_LOGIC := '0';
			 data				 : OUT STD_LOGIC_VECTOR(7 downto 0);
			 
			 
			 empty 			 : OUT STD_LOGIC := '1';
			 full 			 : OUT STD_LOGIC := '0' 
			 );

END;




ARCHITECTURE behaviour OF buffered_uart_rx IS

	SIGNAL iren : std_logic := '0';
	SIGNAL iwen : std_logic := '0';
	
	SIGNAL iempty : std_logic := '1';
	
	SIGNAL dataByte : STD_LOGIC_VECTOR(7 downto 0);
	TYPE buffered_uart_rx_t IS (idle, wait_init, receiving);
	TYPE en_toggle_t IS (ready, not_ready);
	TYPE en_toggle_with_sync_t IS (ready, sync, not_ready);	  
	
	SIGNAL idata_received : STD_LOGIC := '0';
	
	SIGNAL current_s : buffered_uart_rx_t;
	SIGNAL write_en_toggle : en_toggle_with_sync_t;
	SIGNAL read_en_toggle : en_toggle_with_sync_t;



BEGIN

	
	
	fifo : ENTITY work.FIFO 
	
	generic map (
		DATA_WIDTH => 8,
		FIFO_DEPTH => 4
	)	
	
	port map (
		rst => rst,
		clk => clk,
		wen => iwen,
		ren => iren,
		empty => iempty,
		full => full,
		
		dataIn => dataByte,
		dataOut => data
		
	
		);

		 
	rx_module: ENTITY work.uart_rx port map (
		rst => rst,
		baud8x => baud8x,
		rx_pin => rx_pin_in,
		data => dataByte,
		data_ready => idata_received
		);
		
		
	PROCESS(clk, current_s, iempty, idata_received)
		BEGIN
		  
		empty <= iempty;
		--data_ready <= data_received;
		--data_received <= idata_received;
		  
		IF rising_edge(clk) THEN
		
			CASE write_en_toggle IS
				WHEN ready =>
					IF idata_received = '1' THEN
						
						iwen <= '1';
						write_en_toggle <= sync;
					END IF;
					
				WHEN sync =>
				   iwen <= '0';
					data_received <= '1';
					write_en_toggle <= not_ready;
				
		
				WHEN not_ready =>
					iwen <= '0';
					data_received <= '0';
					IF idata_received = '0' THEN
						write_en_toggle <= ready;
					END IF;
					
			END CASE;	
	
			CASE read_en_toggle IS
				WHEN ready =>
					IF read_data = '1' THEN
						iren <= '1';
						read_en_toggle <= sync;
					END IF;
		
		    WHEN sync =>
					iren <= '0';
					data_ready <= '1';		    
		      read_en_toggle <= not_ready;
		
				WHEN not_ready =>
					iren <= '0';
					data_ready <= '0';
					IF read_data = '0' THEN
						read_en_toggle <= ready;
					END IF;
					
			END CASE;	
	
	
	
							
		END IF;			
		
	END PROCESS;



END;

