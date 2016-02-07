library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use IEEE.numeric_std.all;

USE work.bcdDigit_type.all;


ENTITY bin_to_8x7segment IS
	GENERIC( ndigit : INTEGER := 8 );
	
	PORT( strobe_clk : IN STD_LOGIC;
			bin		  : IN UNSIGNED(31 downto 0);
			sel_out 			: OUT STD_LOGIC_VECTOR(7 downto 0);
			segment_out 	: OUT STD_LOGIC_VECTOR(7 downto 0));

END;


ARCHITECTURE structure OF bin_to_8x7segment IS


COMPONENT counter
GENERIC (bit_depth : INTEGER
			);
	PORT(clk : IN STD_LOGIC;
		 qout : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END COMPONENT;

COMPONENT bin2bcd_ndigit
	GENERIC (N : INTEGER;
		   bit_depth : INTEGER
			);
	PORT(bin : IN UNSIGNED(31 DOWNTO 0);
		 digit : OUT bcdDigit(7 DOWNTO 0)
	);
END COMPONENT;

COMPONENT lpm_constant1
	PORT(		 result : OUT STD_LOGIC_VECTOR(15 DOWNTO 0)
	);
END COMPONENT;

COMPONENT counter_3bit
	PORT(clock : IN STD_LOGIC;
		 q : OUT STD_LOGIC_VECTOR(2 DOWNTO 0)
	);
END COMPONENT;

COMPONENT sevenseg_output
	PORT(strobe_in : IN STD_LOGIC;
		 bcd_in : IN STD_LOGIC_VECTOR(3 DOWNTO 0);
		 mux_in : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
		 segment_out : OUT STD_LOGIC_VECTOR(7 DOWNTO 0);
		 sel_out : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END COMPONENT;

COMPONENT timer
	PORT(clk : IN STD_LOGIC;
		 div : IN STD_LOGIC_VECTOR(15 DOWNTO 0);
		 clkOut : OUT STD_LOGIC
	);
END COMPONENT;

COMPONENT lpm_constant0
	PORT(		 result : OUT STD_LOGIC_VECTOR(15 DOWNTO 0)
	);
END COMPONENT;

COMPONENT mux8x4bit_array
	PORT(data : IN bcdDigit(7 DOWNTO 0);
		 sel : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
		 result : OUT STD_LOGIC_VECTOR(3 DOWNTO 0)
	);
END COMPONENT;

SIGNAL	CLK_MASTER :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_9 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_1 :  STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_10 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_4 :  STD_LOGIC_VECTOR(3 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_5 :  STD_LOGIC_VECTOR(2 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_6 :  STD_LOGIC_VECTOR(3 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_8 :  STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_3 :  bcdDigit(7 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_12 :  STD_LOGIC_VECTOR(2 DOWNTO 0);

BEGIN 


b2v_inst11 : bin2bcd_Ndigit
GENERIC MAP(N => 8, bit_depth => 32
			)
PORT MAP(bin => bin,
		 digit => SYNTHESIZED_WIRE_3);



b2v_inst3 : counter_3bit
PORT MAP(clock => strobe_clk,
		 q => SYNTHESIZED_WIRE_5);


b2v_inst5 : sevenseg_output
PORT MAP(strobe_in => SYNTHESIZED_WIRE_5(0),
		 bcd_in => SYNTHESIZED_WIRE_6,
		 mux_in => SYNTHESIZED_WIRE_5,
		 segment_out => segment_out,
		 sel_out => sel_out);


b2v_inst4 : mux8x4bit_array
PORT MAP(data => SYNTHESIZED_WIRE_3,
		 sel => SYNTHESIZED_WIRE_5,
		 result => SYNTHESIZED_WIRE_6);





END;