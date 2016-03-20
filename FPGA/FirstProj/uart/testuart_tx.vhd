--! @file
--! @brief UART Tx Test Bench

--! Use standard library and import the packages (std_logic_1164,std_logic_unsigned,std_logic_arith)
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

--! Use Global Definitions package
 
ENTITY testuart_tx IS
END testuart_tx;
 
--! @brief Test baud_generator module
--! @details Exercise the baud generator with 50Mhz clock and dividing by 434, finally checking for period of 8.68 us
ARCHITECTURE behavior OF testuart_tx IS 
      
   COMPONENT uart_tx
	 
	PORT ( 
	    rst 			 : IN STD_LOGIC;
		 baud			 : IN STD_LOGIC;
		 data			 : IN STD_LOGIC_VECTOR(7 downto 0);
		 tx_out	    : OUT STD_LOGIC := '0';
		 byte_sent   : OUT STD_LOGIC := '0');
		 
	END COMPONENT;
    

--Inputs
	signal rst : std_logic := '0';																		--! Signal to connect with UUT
   signal baud : std_logic := '0';																		--! Signal to connect with UUT
   signal data : STD_LOGIC_VECTOR(7 downto 0); 
  
 	--Outputs
   signal tx_out : std_logic;					--! Signal to connect with UUT
	signal byte_sent : std_logic;		--! Signal to connect with UUT

   -- Clock period definitions (1.8432MHz)
   constant clk_period : time := 8.68 us; -- 8.68 us for 115200 baud
 
BEGIN
  	--! Instantiate the Unit Under Test (UUT)
	uut: uart_tx PORT MAP (
			 rst => rst,
			 baud => baud,
			 data => data,
			 tx_out => tx_out,
			 byte_sent => byte_sent
		  );

	-- Clock process definitions
	clk_process :process
	begin
		baud <= '0';
		wait for clk_period/2;
		baud <= '1';
		wait for clk_period/2;
	end process;
 

  
	stim_proc: PROCESS
	VARIABLE t1 : time;
	VARIABLE t2 : time;
	VARIABLE period : time;
	VARIABLE dataByte : STD_LOGIC_VECTOR(7 downto 0) := conv_std_logic_vector(16#55#, 8);
	
	BEGIN
		rst <= '1';
		data <= dataByte;

		wait for clk_period * 5;
 
		rst <= '0';
			
		wait until rising_edge( baud );
		wait for 10 ns;
		assert tx_out = '0' report "Invalid StartBit" severity failure;
		
		FOR numBit in 0 to 7 LOOP
       wait until rising_edge( baud );
		   wait for 1 ns;
		   
		   assert dataByte(numBit) = tx_out report "Wrong Data Bit" severity failure;
		  
		  
		END LOOP;
	  
		wait until rising_edge( baud );  
		wait until byte_sent = '1';
		
  		assert tx_out = '1' report "Invalid StopBit" severity failure;		  
		
		wait for clk_period * 5;
		
		-- Stop Simulation
		assert false report "NONE. End of simulation." severity failure;
		
		
	END PROCESS;
  
  
  

 


END;
