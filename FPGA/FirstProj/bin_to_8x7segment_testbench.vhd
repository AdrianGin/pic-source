library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

USE work.bcdDigit_type.all;

ENTITY bin_to_8x7segment_tb IS
END bin_to_8x7segment_tb;



ARCHITECTURE behavior OF bin_to_8x7segment_tb IS
	COMPONENT bin_to_8x7segment
	GENERIC( ndigit : INTEGER := 8 );
	PORT( strobe_clk : IN STD_LOGIC;
			bin		  : IN UNSIGNED(15 downto 0);
			sel_out 			: OUT STD_LOGIC_VECTOR(7 downto 0);
			segment_out 	: OUT STD_LOGIC_VECTOR(7 downto 0));

    END COMPONENT;
   --declare inputs and initialize them	
	
	signal istrobe_clk : STD_LOGIC;
 	signal ibin : UNSIGNED (15 downto 0);
	signal isel_out : STD_LOGIC_VECTOR(7 downto 0);
	signal isegment_out : STD_LOGIC_VECTOR(7 downto 0);
	
	constant clk_period : time := 100 ns;
	
BEGIN
  
    -- Instantiate the Unit Under Test (UUT)
   uut: bin_to_8x7segment 
			GENERIC MAP (ndigit => 8)
			PORT MAP (
          strobe_clk => istrobe_clk,
          bin => ibin,
			 sel_out => isel_out,
			 segment_out => isegment_out
			 
        );      

   -- Stimulus process
  stim_proc: process
   begin        
        ibin <= (to_unsigned(65535, 16));
        wait for 10000 ns;
        ibin <= (to_unsigned(49781, 16));
        wait for 10000 ns;
        ibin <= (to_unsigned(256, 16));
        wait for 10000 ns;
        ibin <= (to_unsigned(0, 16));
        wait for 1000 ns;
        ibin <= (to_unsigned(1, 16));
        wait;
  end process;
  
  
  strobe_proc: process
   begin        
	
        istrobe_clk <= '0';
        wait for clk_period/2;  --for 0.5 ns signal is '0'.
        istrobe_clk <= '1';
        wait for clk_period/2;  --for next 0.5 ns signal is '1'.	

  end process;  

END;