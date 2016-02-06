LIBRARY ieee;
USE ieee.std_logic_1164.all;


ENTITY SegmentMapper IS
PORT (d : IN std_logic_vector(7 downto 0);
	   SEG : OUT std_logic_vector(7 downto 0));
END SegmentMapper;

ARCHITECTURE structure OF SegmentMapper IS
	BEGIN
	SEG <= d;
END;