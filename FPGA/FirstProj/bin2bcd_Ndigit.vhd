library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;


PACKAGE bcdDigit_type IS
	 type bcdDigit is array(natural range <>) of STD_LOGIC_VECTOR(3 downto 0);
END PACKAGE;

library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;
USE work.bcdDigit_type.all;

entity bin2bcd_Ndigit is
	 GENERIC ( N 			: integer := 8;
				  bit_depth : integer := 32);
    PORT    ( bin 		: IN  UNSIGNED (bit_depth-1 downto 0);
				  digit 		: OUT  bcdDigit(N-1 downto 0)
				);
end bin2bcd_Ndigit;


ARCHITECTURE behaviour of bin2bcd_Ndigit IS
BEGIN
	BCD1: PROCESS ( bin )
	
	variable bcd  : UNSIGNED ((N*4) - 1 downto 0);
	variable temp : UNSIGNED ( bit_depth-1 downto 0);

	BEGIN
		
	--    -- read input into temp variable
    temp(bit_depth-1 downto 0) := bin;
    bcd := (others => '0');
    -- cycle 32 times as we have 32 input bits, ensure the bin does not exceed digit count.
    -- this could be optimized, we dont need to check and add 3 for the 
    -- first 3 iterations as the number can never be >4
    for i in 0 to bit_depth-1 loop
      for j in 0 to N-1 loop
        if bcd((4*j)+3 downto 4*j) > 4 then 
          bcd((4*j)+3 downto 4*j) := bcd((4*j)+3 downto 4*j) + 3;
        end if;
      end loop;
    

      -- shift bcd left by 1 bit, copy MSB of temp into LSB of bcd
      bcd := bcd((N*4)-2 downto 0) & temp(bit_depth-1);
    
      -- shift temp left by 1 bit
      temp := temp(bit_depth-2 downto 0) & '0';
    
    end loop;
 
   FOR I IN 0 TO N-1 LOOP
		IF 4*I < bit_depth THEN
			digit(I) <= STD_LOGIC_VECTOR(bcd((4*I)+3 downto 4*I));
		END IF;
   END LOOP;
    
	
	END PROCESS BCD1;
	
END behaviour;
