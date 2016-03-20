--! @file
--! @brief UART Tx Test Bench

--! Use standard library and import the packages (std_logic_1164,std_logic_unsigned,std_logic_arith)
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

--! Use Global Definitions package
 
ENTITY testbuffered_uart_rx IS
END testbuffered_uart_rx;
 
--! @brief Test baud_generator module
--! @details Exercise the baud generator with 50Mhz clock and dividing by 434, finally checking for period of 8.68 us
ARCHITECTURE behavior OF testbuffered_uart_rx IS 
      
   COMPONENT buffered_uart_rx
	 
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

	END COMPONENT;
    

--Inputs
	signal rst : std_logic := '0';																		--! Signal to connect with UUT
   signal baud8x : std_logic := '0';																		--! Signal to connect with UUT
   signal data : STD_LOGIC_VECTOR(7 downto 0) := (others => '0'); 
   signal master_clk : std_logic;
	
	signal empty : std_logic := '1';
	signal full : std_logic := '0';
  
   signal data_received : std_logic := '0';
  
  signal read_data : std_logic := '0';
  
 	--Outputs
	signal data_ready : std_logic := '0';
   signal rx_pin_in : std_logic := '1';					--! Signal to connect with UUT
   
   -- Clock period definitions (1.8432MHz)
   constant clk_period : time := 1.085 us; -- 8.68 us for 115200 baud
   constant master_clk_period : time := 20 ns;
 
BEGIN
  	--! Instantiate the Unit Under Test (UUT)
	uut: buffered_uart_rx PORT MAP (
			 rst => rst,
			 clk => master_clk,
			 baud8x => baud8x,
			 rx_pin_in => rx_pin_in,
			 read_data => read_data,
			
			 data_ready => data_ready,
			 data_received => data_received,
			 data => data,
			 
			 empty => empty,
			 full => full
		  );

	-- Clock process definitions
	clk_process :process
	begin
		baud8x <= '0';
		wait for clk_period/2;
		baud8x <= '1';
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
	
	
	type testCases is array (3 downto 0) of STD_LOGIC_VECTOR(7 downto 0);
	
	VARIABLE dataBytes : testCases;
	VARIABLE testcase : STD_LOGIC_VECTOR(7 downto 0);

	
	BEGIN
	  dataBytes(0) := ( conv_std_logic_vector(16#55#, 8)  );
	  dataBytes(1) := ( conv_std_logic_vector(16#CF#, 8)  );
	  dataBytes(2) := ( conv_std_logic_vector(16#FF#, 8)  );
	  dataBytes(3) := ( conv_std_logic_vector(16#00#, 8)  );
	  	  	  	  
		rst <= '1';		
		wait for master_clk_period * 5;
		rst <= '0';
		wait for master_clk_period * 5;  	
   	   
	  for i in 0 to dataBytes'Length - 1 loop	 

		  rx_pin_in <= '0';
		  wait for clk_period * 8;
		  for j in 0 to 7 loop
		    rx_pin_in <= dataBytes(i)(j);
		    wait for clk_period * 8;
		  end loop;
		  rx_pin_in <= '1';
		  wait for clk_period * 16;
		  
		end loop;

    

				
    wait for clk_period * 16;
		
	  for i in 0 to dataBytes'Length - 1 loop	 
		  read_data <= '1';
		  wait for clk_period * 2;
		  read_data <= '0';
		  
		  assert data = dataBytes(i) report "Data Mismatch" severity failure;
		  wait for clk_period * 16;
		  
		end loop;
		
		read_data <= '1';
		wait for clk_period * 2;
		read_data <= '0';
		
		-- Stop Simulation
		assert false report "NONE. End of simulation." severity failure;
		
		
	END PROCESS;
  
  
  

 


END;

