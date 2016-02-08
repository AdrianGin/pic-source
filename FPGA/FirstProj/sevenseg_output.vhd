LIBRARY ieee;
USE ieee.std_logic_1164.all;
use IEEE.numeric_std.ALL;

ENTITY sevenseg_output IS
PORT (
		strobe_in		: IN STD_LOGIC;
		mux_in    		: IN STD_LOGIC_VECTOR(2 downto 0);
		bcd_in 			: IN STD_LOGIC_VECTOR(3 downto 0);
		sel_out 			: OUT STD_LOGIC_VECTOR(7 downto 0);
		segment_out 	: OUT STD_LOGIC_VECTOR(7 downto 0)
		);
END sevenseg_output;


ARCHITECTURE firstBuff OF sevenseg_output IS

COMPONENT BCD_to_7Seg
	PORT (d : IN STD_LOGIC_VECTOR(3 downto 0);
			q : OUT STD_LOGIC_VECTOR(7 downto 0));
END COMPONENT;

COMPONENT dec3to8
	PORT (e   : IN STD_LOGIC;
			sel : IN STD_LOGIC_VECTOR(2 downto 0);
			q   : OUT STD_LOGIC_VECTOR(7 downto 0));
END COMPONENT;

	SIGNAL sub_e : STD_LOGIC := '1';
	SIGNAL sub_mux    : STD_LOGIC_VECTOR(2 downto 0);
	SIGNAL sub_dec_out    : STD_LOGIC_VECTOR(7 downto 0);
	SIGNAL SEG7_OUTPUT : STD_LOGIC_VECTOR(7 downto 0);
	
	SIGNAL sub_strobe_index : UNSIGNED(2 downto 0) := "000";
	
BEGIN

	PROCESS( strobe_in )
		BEGIN
		IF (strobe_in'EVENT AND strobe_in = '1') THEN
			--sub_load <= sub_load xor '1';
			sub_strobe_index <= sub_strobe_index + 1;
		END IF;
	
	END PROCESS;

	sub_mux <= STD_LOGIC_VECTOR(sub_strobe_index);
	sel_out <= NOT sub_dec_out ;	
	segment_out <= NOT SEG7_OUTPUT;
	sub_e <= '1';
	
	DEC0 : dec3to8 PORT MAP(e => sub_e, sel => mux_in, q => sub_dec_out);
	SEG7_0: BCD_to_7Seg PORT MAP (d => bcd_in, q => SEG7_OUTPUT );
	
END;
