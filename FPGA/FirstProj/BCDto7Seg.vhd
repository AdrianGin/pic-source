LIBRARY ieee;
USE ieee.std_logic_1164.all;

ENTITY BCD_to_7Seg IS
PORT (d : IN std_logic_vector(3 downto 0);
	   q : OUT std_logic_vector(7 downto 0));
END BCD_to_7Seg;


ARCHITECTURE behaviour OF BCD_to_7Seg IS

BEGIN
	SevenSegDecode: PROCESS(d)
	BEGIN 
		CASE d IS
			WHEN "0000" => 
				q <= X"3F";
			WHEN "0001" => 
				q <= X"06";				
			WHEN "0010" =>
				q <= X"5B";
			WHEN "0011" =>
				q <= X"4F";		
			WHEN "0100" =>
				q <= X"66";	
			WHEN "0101" =>
				q <= X"6D";	
			WHEN "0110" =>
				q <= X"7D";
			WHEN "0111" =>
				q <= X"07";
			WHEN "1000" =>
				q <= X"7F";
			WHEN "1001" =>
				q <= X"6F";
			WHEN "1010" =>
				q <= X"77";
			WHEN "1011" =>
				q <= X"7C";				
			WHEN "1100" =>
				q <= X"39";
			WHEN "1101" =>
				q <= X"5E";
			WHEN "1110" =>
				q <= X"79";
			WHEN "1111" =>
				q <= X"71";								
			WHEN OTHERS => NULL;
		END CASE;
	END PROCESS SevenSegDecode;
END behaviour;

