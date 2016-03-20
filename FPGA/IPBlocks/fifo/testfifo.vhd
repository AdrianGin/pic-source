--! @file
--! @brief Test baud_generator module

--! Use standard library and import the packages (std_logic_1164,std_logic_unsigned,std_logic_arith)
library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.std_logic_arith.all;
use ieee.numeric_std.all;


--! Use Global Definitions package
 
ENTITY testfifo IS
END testfifo;
 
--! @brief Test baud_generator module
--! @details Exercise the baud generator with 50Mhz clock and dividing by 434, finally checking for period of 8.68 us
ARCHITECTURE behavior OF testfifo IS 
      
    COMPONENT fifo
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
    
	 END COMPONENT;

   --Inputs
	
	signal rst : std_logic := '0';																		--! Signal to connect with UUT
	signal clk : std_logic := '0';																		--! Signal to connect with UUT
	
	signal ren : std_logic := '0';																		--! Signal to connect with UUT
	signal wen : std_logic := '0';																		--! Signal to connect with UUT
	
	signal dataIn : std_logic_vector(7 downto 0) := (others => '0');

  
 	--Outputs
   signal empty : std_logic;					--! Signal to connect with UUT
	signal full : std_logic;		--! Signal to connect with UUT
	signal dataOut : std_logic_vector(7 downto 0) := (others => '0');
	  
	  
   -- Clock period definitions (1.8432MHz)
   constant clk_period : time := 20 ns; -- 0.543us (1.8432Mhz) 20ns (50Mhz)
 
BEGIN
  	--! Instantiate the Unit Under Test (UUT)
	uut: fifo PORT MAP (
			 rst => rst,
			 clk => clk,
			 ren => ren, wen => wen, 
			 dataIn => dataIn,
			 empty => empty,
			 full => full,
			 dataOut => dataOut
		  );

	-- Clock process definitions
	clk_process :process
	begin
		clk <= '0';
		wait for clk_period/2;
		clk <= '1';
		wait for clk_period/2;
	end process;
 

  
	rst_proc: PROCESS
	VARIABLE t1 : time;
	VARIABLE t2 : time;
	VARIABLE period : time;	
	
	BEGIN
		rst <= '1';
    wait for clk_period * 10;
		rst <= '0';
    wait for clk_period * 10;
    
    wait;
	

	END PROCESS;
  
  
  
	wen_proc: PROCESS
	VARIABLE dataByte  : std_logic_vector(7 downto 0) := (others => '0');
	
	BEGIN
    wait until rst = '0';
    wait for clk_period * 10;
        
    FOR count IN 0 TO 15 LOOP
       dataIn <= dataByte;       
       wen <= '1';
       wait for clk_period;
       wen <= '0';
       wait for clk_period;       
       dataByte := dataByte + 1;
    
    END LOOP;
  
    wen <= '0';
  
  
	  WAIT;
		-- Stop Simulation
		--assert false report "NONE. End of simulation." severity failure;
		
		
	END PROCESS;
  
 	ren_proc: PROCESS
	VARIABLE dataByte  : std_logic_vector(7 downto 0) := (others => '0');
	
	BEGIN
    wait until rst = '0';
    wait for clk_period * (10 + 16*2);
    
    FOR count IN 0 TO 15 LOOP


       ren <= '1';
       wait for clk_period;
       ren <= '0';
       wait for clk_period;       
       dataByte := dataOut;       
       assert conv_std_logic_vector(count, 8) = dataByte report "Read Fail" severity failure;
    
    END LOOP;
  
    --ren <= '0';
    
  
	
	  wait for clk_period * 10;
	
		-- Stop Simulation
		assert false report "NONE. End of simulation." severity failure;
		
		
	END PROCESS;
  


END;
