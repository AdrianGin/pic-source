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
-- CREATED		"Sun Feb 07 00:13:00 2016"

LIBRARY ieee;
USE ieee.std_logic_1164.all; 

LIBRARY work;

ENTITY FirstProject IS 
	PORT
	(
		osc_clk :  IN  STD_LOGIC;
		en :  OUT  STD_LOGIC_VECTOR(7 DOWNTO 0);
		SEG :  OUT  STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END FirstProject;

ARCHITECTURE bdf_type OF FirstProject IS 

COMPONENT counter
GENERIC (bit_depth : INTEGER
			);
	PORT(clk : IN STD_LOGIC;
		 qout : OUT STD_LOGIC_VECTOR(7 DOWNTO 0)
	);
END COMPONENT;

COMPONENT pll
	PORT(inclk0 : IN STD_LOGIC;
		 c0 : OUT STD_LOGIC
	);
END COMPONENT;

COMPONENT bin2bcd_ndigit
GENERIC (bit_depth : INTEGER;
			N : INTEGER
			);
	PORT(bin : IN STD_LOGIC_VECTOR(7 DOWNTO 0);
		 digit : OUT STD_LOGIC_VECTOR(7 DOWNTO 0 , 3 DOWNTO 0)
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

COMPONENT mux8x4bit_array
	PORT(data : IN STD_LOGIC_VECTOR(7 DOWNTO 0 , 3 DOWNTO 0);
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

SIGNAL	CLK_MASTER :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_0 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_1 :  STD_LOGIC_VECTOR(7 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_11 :  STD_LOGIC;
SIGNAL	SYNTHESIZED_WIRE_3 :  STD_LOGIC_VECTOR(7 DOWNTO 0 , 3 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_12 :  STD_LOGIC_VECTOR(2 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_6 :  STD_LOGIC_VECTOR(3 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_8 :  STD_LOGIC_VECTOR(15 DOWNTO 0);
SIGNAL	SYNTHESIZED_WIRE_10 :  STD_LOGIC_VECTOR(15 DOWNTO 0);


BEGIN 



b2v_inst : counter
GENERIC MAP(bit_depth => 8
			)
PORT MAP(clk => SYNTHESIZED_WIRE_0,
		 qout => SYNTHESIZED_WIRE_1);


b2v_inst1 : pll
PORT MAP(inclk0 => osc_clk,
		 c0 => CLK_MASTER);


b2v_inst11 : bin2bcd_ndigit
GENERIC MAP(bit_depth => 8,
			N => 8
			)
PORT MAP(bin => SYNTHESIZED_WIRE_1,
		 digit => SYNTHESIZED_WIRE_3);


b2v_inst2 : lpm_constant1
PORT MAP(		 result => SYNTHESIZED_WIRE_10);


b2v_inst3 : counter_3bit
PORT MAP(clock => SYNTHESIZED_WIRE_11,
		 q => SYNTHESIZED_WIRE_12);


b2v_inst4 : mux8x4bit_array
PORT MAP(data => SYNTHESIZED_WIRE_3,
		 sel => SYNTHESIZED_WIRE_12,
		 result => SYNTHESIZED_WIRE_6);


b2v_inst5 : sevenseg_output
PORT MAP(strobe_in => SYNTHESIZED_WIRE_11,
		 bcd_in => SYNTHESIZED_WIRE_6,
		 mux_in => SYNTHESIZED_WIRE_12,
		 segment_out => SEG,
		 sel_out => en);


b2v_inst6 : timer
PORT MAP(clk => CLK_MASTER,
		 div => SYNTHESIZED_WIRE_8,
		 clkOut => SYNTHESIZED_WIRE_11);


b2v_inst7 : lpm_constant0
PORT MAP(		 result => SYNTHESIZED_WIRE_8);


b2v_inst8 : timer
PORT MAP(clk => SYNTHESIZED_WIRE_11,
		 div => SYNTHESIZED_WIRE_10,
		 clkOut => SYNTHESIZED_WIRE_0);


END bdf_type;