--! @file
--! @brief Test baud_generator module

--! Use standard library and import the packages (std_logic_1164,std_logic_unsigned,std_logic_arith)
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_arith.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;

--! Use Global Definitions package
 
ENTITY testuart_brgen IS
END testuart_brgen;
 
--! @brief Test baud_generator module
--! @details Exercise the baud generator with 50Mhz clock and dividing by 434, finally checking for period of 8.68 us
ARCHITECTURE behavior OF testuart_brgen IS 
      
    COMPONENT uart_brgen
	 
	PORT ( 
			 rst 			 : IN STD_LOGIC;
			 clk 			 : IN STD_LOGIC;
			 div 			 : IN STD_LOGIC_VECTOR(15 downto 0);
			 baudrate_8x : OUT STD_LOGIC;
			 baudrate 	 : OUT STD_LOGIC);
			 
    END COMPONENT;
    

   --Inputs
	
	signal rst : std_logic := '0';																		--! Signal to connect with UUT
  signal clk : std_logic := '0';																		--! Signal to connect with UUT
  
  signal div : std_logic_vector(15 downto 0);
  
  
 	--Outputs
   signal baud : std_logic;					--! Signal to connect with UUT
	signal baud_oversample : std_logic;		--! Signal to connect with UUT

   -- Clock period definitions (1.8432MHz)
   constant clk_period : time := 20 ns; -- 0.543us (1.8432Mhz) 20ns (50Mhz)
 
BEGIN
  	--! Instantiate the Unit Under Test (UUT)
	uut: uart_brgen PORT MAP (
			 rst => rst,
			clk => clk,
			 div => div,
			 baudrate_8x => baud_oversample,
			 baudrate => baud
		  );

	-- Clock process definitions
	clk_process :process
	begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
	end process;
 

  
	stim_proc: PROCESS
	VARIABLE t1 : time;
	VARIABLE t2 : time;
	VARIABLE period : time;	
	
	BEGIN
	  
	  
		rst <= '1';
		div <= conv_std_logic_vector(5208, 16); -- 50000000/9600
    wait for clk_period * 60;
		rst <= '0';
		wait for clk_period * 60;
		
		wait until baud = '1';
		t1 := now;
		wait until baud = '0';
		wait until baud = '1';
		t2 := now;
		
		period := t2 - t1;
		
		assert period = 104.160 us report "Wrong period expected 8.68us, got: " & time'image(period) severity failure;
		
		wait until baud_oversample = '1';
   	t1 := now;
		wait until baud_oversample = '0';
		wait until baud_oversample = '1';
		t2 := now;
		
		period := t2 - t1;
		assert period = 13.020 us report "Wrong period expected 1.085us, got: " & time'image(period) severity failure;
		
		
		
		rst <= '1';
		div <= conv_std_logic_vector(434, 16); -- 50000000/115200
      wait for clk_period * 60;
		rst <= '0';
		
		wait until baud = '1';
		t1 := now;
		wait until baud = '0';
		wait until baud = '1';
		t2 := now;
		
		period := t2 - t1;
		
		assert period = 8.68 us report "Wrong period expected 8.68us, got: " & time'image(period) severity failure;
		
		wait until baud_oversample = '1';
   	t1 := now;
		wait until baud_oversample = '0';
		wait until baud_oversample = '1';
		t2 := now;
		
		period := t2 - t1;
		assert period = 1.080 us report "Wrong period expected 1.085us, got: " & time'image(period) severity failure;
		
				
		
		
		-- Stop Simulation
		assert false report "NONE. End of simulation." severity failure;
		
		
	END PROCESS;
  
  
  

 


END;
