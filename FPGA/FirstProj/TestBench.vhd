-- Copyright (C) 1991-2013 Altera Corporation
-- Your use of Altera Corporation's design tools, logic functions 
-- and other software and tools, and its AMPP partner logic 
-- functions, and any output files from any of the foregoing 
-- (including device programming or simulation files), and any 
-- associated documentation or information are expressly subject 
-- to the terms and conditions of the Altera Program License 
-- Subscription Agreement, Altera MegaCore Function License 
-- Agreement, or other applicable license agreement, including, 
-- without limitation, that your use is for the sole purpose of 
-- programming logic devices manufactured by Altera and sold by 
-- Altera or its authorized distributors.  Please refer to the 
-- applicable agreement for further details.

-- PROGRAM		"Quartus II 64-Bit"
-- VERSION		"Version 13.0.0 Build 156 04/24/2013 SJ Full Version"
-- CREATED		"Sat Feb 06 23:49:23 2016"

LIBRARY ieee;
USE ieee.std_logic_1164.all; 
use IEEE.numeric_std.ALL;
USE work.bcdDigit_type.all;
LIBRARY work;

LIBRARY ieee;
USE ieee.std_logic_1164.all; 
use IEEE.numeric_std.ALL;
USE work.bcdDigit_type.all;

ENTITY TestBench IS 
END TestBench;

ARCHITECTURE bdf_type OF TestBench IS 

COMPONENT lpm_constant2
	PORT(		 result : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END COMPONENT;

COMPONENT bin2bcd_ndigit
	 GENERIC ( N 			: integer := 8;
				  bit_depth : integer := 32);
    PORT    ( bin 		: IN  UNSIGNED (bit_depth-1 downto 0);
				  digit 		: OUT  bcdDigit(N-1 downto 0)
	);
END COMPONENT;

COMPONENT lpm_constant3
	PORT(		 result : OUT STD_LOGIC_VECTOR(0 TO 0)
	);
END COMPONENT;

COMPONENT mux8x4bit_array
	PORT(data : IN  bcdDigit(7 downto 0);
		 sel : IN STD_LOGIC_VECTOR(2 DOWNTO 0);
		 result : OUT STD_LOGIC_VECTOR(3 DOWNTO 0)
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

SIGNAL	SYNTHESIZED_WIRE_0 :  STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_1 :  bcdDigit(7 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_6 :  STD_LOGIC_VECTOR(2 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_3 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_4 :  STD_LOGIC_VECTOR(3 DOWNTO 0);


BEGIN 



b2v_inst : lpm_constant2
PORT MAP(		 result => SYNTHESIZED_WIRE_0);


b2v_inst11 : bin2bcd_ndigit
GENERIC MAP(bit_depth => 8,
			N => 8
			)
PORT MAP(bin => SYNTHESIZED_WIRE_0,
		 digit => SYNTHESIZED_WIRE_1);


b2v_inst3 : lpm_constant3
PORT MAP(		 result(0) => SYNTHESIZED_WIRE_3);


b2v_inst4 : mux8x4bit_array
PORT MAP(data => SYNTHESIZED_WIRE_1,
		 sel => SYNTHESIZED_WIRE_6,
		 result => SYNTHESIZED_WIRE_4);


b2v_inst5 : sevenseg_output
PORT MAP(strobe_in => SYNTHESIZED_WIRE_3,
		 bcd_in => SYNTHESIZED_WIRE_4,
		 mux_in => SYNTHESIZED_WIRE_6);


b2v_inst6 : lpm_constant2
PORT MAP(		 result => SYNTHESIZED_WIRE_6);


END bdf_type;