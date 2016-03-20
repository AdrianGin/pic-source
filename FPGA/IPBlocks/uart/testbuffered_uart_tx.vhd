--! @file
--! @brief UART Tx Test Bench

--! Use standard library and import the packages (std_logic_1164,std_logic_unsigned,std_logic_arith)
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

--! Use Global Definitions package
 
ENTITY testbuffered_uart_tx IS
END testbuffered_uart_tx;
 
--! @brief Test baud_generator module
--! @details Exercise the baud generator with 50Mhz clock and dividing by 434, finally checking for period of 8.68 us
ARCHITECTURE behavior OF testbuffered_uart_tx IS 
      
   COMPONENT buffered_uart_tx
	 
	PORT ( 
			 rst 				 : IN STD_LOGIC;
			 clk				 : IN STD_LOGIC;
			 
			 baud				 : IN STD_LOGIC;
			 
			 tx_start		 : IN STD_LOGIC := '0';
			 dataIn			 : IN STD_LOGIC_VECTOR(7 downto 0);
			 
			 tx_pin_out   	 : OUT STD_LOGIC := '0');

	END COMPONENT;
    

--Inputs
	signal rst : std_logic := '0';																		--! Signal to connect with UUT
   signal baud : std_logic := '0';																		--! Signal to connect with UUT
   signal data : STD_LOGIC_VECTOR(7 downto 0); 
   signal master_clk : std_logic;
  
 	--Outputs
	signal tx_start : std_logic;
   signal tx_out : std_logic;					--! Signal to connect with UUT
   
   -- Clock period definitions (1.8432MHz)
   constant clk_period : time := 8.68 us; -- 8.68 us for 115200 baud
   constant master_clk_period : time := 20 ns;
 
BEGIN
  	--! Instantiate the Unit Under Test (UUT)
	uut: buffered_uart_tx PORT MAP (
			 rst => rst,
			 clk => master_clk,
			 baud => baud,
			 dataIn => data,
			 tx_start => tx_start,
			 tx_pin_out => tx_out
		  );

	-- Clock process definitions
	clk_process :process
	begin
		baud <= '0';
		wait for clk_period/2;
		baud <= '1';
		wait for clk_period/2;
	end process;
 
	mstr_clk_process :process
	begin
		master_clk <= '0';
		wait for master_clk_period/2;
		master_clk <= '1';
		wait for master_clk_period/2;
	end process;
  
	stim_proc: PROCESS
	VARIABLE dataByte : STD_LOGIC_VECTOR(7 downto 0) := conv_std_logic_vector(16#55#, 8);
	VARIABLE dataByte2 : STD_LOGIC_VECTOR(7 downto 0) := conv_std_logic_vector(16#CF#, 8);	
	VARIABLE dataByte3 : STD_LOGIC_VECTOR(7 downto 0) := conv_std_logic_vector(16#FF#, 8);	
   VARIABLE dataByte4 : STD_LOGIC_VECTOR(7 downto 0) := conv_std_logic_vector(16#00#, 8);	
	
	BEGIN
		rst <= '1';
		tx_start <= '0';
		
		wait for master_clk_period * 5;
		rst <= '0';
		wait for master_clk_period * 5;
		tx_start <= '1';
		data <= dataByte;
		wait for master_clk_period;
   	tx_start <= '0';
   	wait for master_clk_period;
   	
   	
		tx_start <= '1';
		data <= dataByte2;
		wait for master_clk_period;
   	tx_start <= '0';
   	wait for master_clk_period;
   	   	
		tx_start <= '1';
		data <= dataByte3;
		wait for master_clk_period;
   	tx_start <= '0';
   	wait for master_clk_period;   	   	


		tx_start <= '1';
		data <= dataByte4;
		wait for master_clk_period;
   	tx_start <= '0';
   	wait for master_clk_period;   	   	
   	   	   	   	
   	 --DataByte
		wait until tx_out = '0';
		assert tx_out = '0' report "Invalid StartBit" severity failure;
		FOR numBit in 0 to 7 LOOP
			wait until rising_edge( baud );
			wait for 3 ns;
		   assert dataByte(numBit) = tx_out report "Wrong Data Bit" severity failure;
		END LOOP;
		wait until rising_edge( baud );  
		wait for 10 ns;
  		assert tx_out = '1' report "Invalid StopBit" severity failure;		  
		
		--DataByte2
		wait until tx_out = '0';
		assert tx_out = '0' report "Invalid StartBit" severity failure;
		FOR numBit in 0 to 7 LOOP
			wait until rising_edge( baud );
		   wait for 3 ns;
		   assert dataByte2(numBit) = tx_out report "Wrong Data Bit" severity failure;
		END LOOP;
		wait until rising_edge( baud );  
		wait for 10 ns;
  		assert tx_out = '1' report "Invalid StopBit" severity failure;		  


  --DataByte3				
		wait until tx_out = '0';
		assert tx_out = '0' report "Invalid StartBit" severity failure;
		FOR numBit in 0 to 7 LOOP
			wait until rising_edge( baud );
		   wait for 3 ns;
		   assert dataByte3(numBit) = tx_out report "Wrong Data Bit" severity failure;
		END LOOP;
		wait until rising_edge( baud );  
		wait for 10 ns;
  		assert tx_out = '1' report "Invalid StopBit" severity failure;		  
						

  --DataByte4
		wait until tx_out = '0';
		assert tx_out = '0' report "Invalid StartBit" severity failure;
		FOR numBit in 0 to 7 LOOP
			wait until rising_edge( baud );
		   wait for 3 ns;
		   assert dataByte4(numBit) = tx_out report "Wrong Data Bit" severity failure;
		END LOOP;
		wait until rising_edge( baud );  
		wait for 10 ns;
  		assert tx_out = '1' report "Invalid StopBit" severity failure;		  
						
		wait for clk_period * 10;
		
		-- Stop Simulation
		assert false report "NONE. End of simulation." severity failure;
		
		
	END PROCESS;
  
  
  

 


END;
