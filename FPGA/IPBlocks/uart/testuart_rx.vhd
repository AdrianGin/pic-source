--! @file
--! @brief UART Rx Test Bench

--! Use standard library and import the packages (std_logic_1164,std_logic_unsigned,std_logic_arith)
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

--! Use Global Definitions package
 
ENTITY testuart_rx IS
END testuart_rx;
 
--! @brief Test baud_generator module
--! @details Exercise the baud generator with 50Mhz clock and dividing by 434, finally checking for period of 8.68 us
ARCHITECTURE behavior OF testuart_rx IS 
      
   COMPONENT uart_rx
	 
	PORT ( 
		 rst 			    : IN STD_LOGIC;
		 baud8x			 : IN STD_LOGIC;
		 rx_pin	       : IN STD_LOGIC := '0';
		 
		 data			 	 : OUT STD_LOGIC_VECTOR(7 downto 0);
		 data_ready     : OUT STD_LOGIC := '0');
		

	END COMPONENT;
    

--Inputs
	signal rst : std_logic := '0';																		--! Signal to connect with UUT
   signal baud8x : std_logic := '0';																		--! Signal to connect with UUT
   signal data : STD_LOGIC_VECTOR(7 downto 0); 
  
 	--Outputs
   signal rx_pin : std_logic;					--! Signal to connect with UUT
	signal data_ready : std_logic;		--! Signal to connect with UUT

   -- Clock period definitions (1.8432MHz)
   constant clk_period : time := 8.68 us; -- 8.68 us for 115200 baud
 
BEGIN
  	--! Instantiate the Unit Under Test (UUT)
	uut: uart_rx PORT MAP (
			 rst => rst,
			 baud8x => baud8x,
			 data => data,
			 rx_pin => rx_pin,
			 data_ready => data_ready
		  );

	-- Clock process definitions
	clk_process :process
	begin
		baud8x <= '0';
		wait for clk_period/2;
		baud8x <= '1';
		wait for clk_period/2;
	end process;
 

  
	stim_proc: PROCESS
	VARIABLE t1 : time;
	VARIABLE t2 : time;
	VARIABLE period : time;
	VARIABLE dataByte : STD_LOGIC_VECTOR(7 downto 0) := conv_std_logic_vector(16#55#, 8);
	VARIABLE dataByte2 : STD_LOGIC_VECTOR(7 downto 0) := conv_std_logic_vector(16#0F#, 8);	
	
	BEGIN
		rst <= '1';
		rx_pin <= '1';

		wait for clk_period * 5;
		rst <= '0';
		
		
		rx_pin <= '0';
		wait for clk_period * 8;
		for i in 0 to 7 loop
		   rx_pin <= dataByte(i);
		   wait for clk_period * 8;
		end loop;
		rx_pin <= '1';
		wait for clk_period * 16;
		
		rx_pin <= '0';
		wait for clk_period * 8;
		for i in 0 to 7 loop
		   rx_pin <= dataByte2(i);
		   wait for clk_period * 8;
		end loop;
		rx_pin <= '1';
		wait for clk_period * 20;
		
		
		
		-- Stop Simulation
		assert false report "NONE. End of simulation." severity failure;
		
		
	END PROCESS;
  
  
  

 


END;
