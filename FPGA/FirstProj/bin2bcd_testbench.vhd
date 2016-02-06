library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

USE work.bcdDigit_type.all;

ENTITY bin2bcd_tb IS
END bin2bcd_tb;



ARCHITECTURE behavior OF bin2bcd_tb IS
   -- Component Declaration for the Unit Under Test (UUT)
    COMPONENT bin2bcd_Ndigit  --'test' is the name of the module needed to be tested.
--just copy and paste the input and output ports of your module as such.
	 GENERIC ( N 			: integer := 8;
				  bit_depth : integer := 32);
    PORT    ( bin 		: IN  UNSIGNED (bit_depth-1 downto 0);
				  digit 		: OUT  bcdDigit(N-1 downto 0)
				);
    END COMPONENT;
   --declare inputs and initialize them	

 	signal ibin : UNSIGNED (15 downto 0);
	signal idigit : bcdDigit(7 downto 0);
	
BEGIN
  
    -- Instantiate the Unit Under Test (UUT)
   uut: bin2bcd_Ndigit 
			GENERIC MAP (N => 8, bit_depth => 16)
			PORT MAP (
          bin => ibin,
          digit => idigit
        );      

   -- Stimulus process
  stim_proc: process
   begin        
        wait for 7 ns;
        ibin <= to_unsigned(12345, 16);
        wait for 3 ns;
        ibin <= to_unsigned(255, 16);
        wait for 17 ns;
        ibin <= to_unsigned(0, 16);
        wait for 1 ns;
        ibin <= to_unsigned(1, 16);
        wait;
  end process;

END;