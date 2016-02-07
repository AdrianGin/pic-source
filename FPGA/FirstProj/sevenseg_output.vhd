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

COMPONENT mux8x4bit
	PORT
	(
		data0x		: IN STD_LOGIC_VECTOR (3 DOWNTO 0);
		data1x		: IN STD_LOGIC_VECTOR (3 DOWNTO 0);
		data2x		: IN STD_LOGIC_VECTOR (3 DOWNTO 0);
		data3x		: IN STD_LOGIC_VECTOR (3 DOWNTO 0);
		data4x		: IN STD_LOGIC_VECTOR (3 DOWNTO 0);
		data5x		: IN STD_LOGIC_VECTOR (3 DOWNTO 0);
		data6x		: IN STD_LOGIC_VECTOR (3 DOWNTO 0);
		data7x		: IN STD_LOGIC_VECTOR (3 DOWNTO 0);
		sel		   : IN STD_LOGIC_VECTOR (2 DOWNTO 0);
		result		: OUT STD_LOGIC_VECTOR (3 DOWNTO 0)
	);
END COMPONENT;

COMPONENT dec3to8
	PORT (e   : IN STD_LOGIC;
			sel : IN STD_LOGIC_VECTOR(2 downto 0);
			q   : OUT STD_LOGIC_VECTOR(7 downto 0));
END COMPONENT;

COMPONENT latch_4bit
	PORT ( din  : IN STD_LOGIC_VECTOR(3 downto 0); 
		 load : IN STD_LOGIC;
		 dout : OUT STD_LOGIC_VECTOR(3 downto 0));
END COMPONENT;

	SIGNAL sub_e : STD_LOGIC := '1';
	SIGNAL sub_mux    : STD_LOGIC_VECTOR(2 downto 0);
	SIGNAL sub_dec_out    : STD_LOGIC_VECTOR(7 downto 0);
	SIGNAL SEG7_OUTPUT : STD_LOGIC_VECTOR(7 downto 0);
	
	TYPE sub_din_array IS ARRAY (0 to 7) of STD_LOGIC_VECTOR(3 downto 0); 
	TYPE sub_dout_array IS ARRAY (0 to 7) of STD_LOGIC_VECTOR(3 downto 0); 
	
	SIGNAL sub_din : sub_din_array;
	SIGNAL sub_dout : sub_dout_array;
	
	SIGNAL dummydigit : STD_LOGIC_VECTOR(3 downto 0);
	
	SIGNAL sub_bcd_in : STD_LOGIC_VECTOR(3 downto 0);
	
	SIGNAL sub_load : STD_LOGIC_VECTOR(7 downto 0);
	
	SIGNAL sub_strobe_index : UNSIGNED(2 downto 0) := "000";
	
BEGIN

	PROCESS( strobe_in )
		BEGIN
		IF (strobe_in'EVENT AND strobe_in = '1') THEN
			--sub_load <= sub_load xor '1';
			sub_strobe_index <= sub_strobe_index + 1;
		END IF;
	
	END PROCESS;

   GEN_LATCH: 
   for I in 0 to 7 generate
      LATCHX : latch_4bit PORT MAP(din => bcd_in, load => sub_load(I), dout => sub_dout(I));
   END GENERATE GEN_LATCH;
	
	sub_mux <= STD_LOGIC_VECTOR(sub_strobe_index);
	sel_out <= NOT sub_dec_out ;	
	segment_out <= NOT SEG7_OUTPUT;
	sub_e <= '1';
	
	DEC0 : dec3to8 PORT MAP(e => sub_e, sel => mux_in, q => sub_dec_out);
	
	DEC1 : dec3to8 PORT MAP(e => sub_e, sel => mux_in, q => sub_load);
	
	
	MUX0 : mux8x4bit PORT MAP( 
										data0x => sub_dout(0),
										data1x => sub_dout(1),
										data2x => sub_dout(2),
										data3x => sub_dout(3),
										data4x => sub_dout(4),
										data5x => sub_dout(5),
										data6x => sub_dout(6),
										data7x => sub_dout(7),
										sel => mux_in,
										result => sub_bcd_in);
	SEG7_1: BCD_to_7Seg PORT MAP (d => bcd_in, q => SEG7_OUTPUT );
	
END;
