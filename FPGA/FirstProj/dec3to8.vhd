LIBRARY ieee;
USE ieee.std_logic_1164.all;


ENTITY dec3to8 IS

PORT (e   : IN STD_LOGIC;
		sel : IN STD_LOGIC_VECTOR(2 downto 0);
	   q   : OUT STD_LOGIC_VECTOR(7 downto 0));

END;

ARCHITECTURE structure OF dec3to8 IS

BEGIN
	
	
	
	q(0) <= e AND ((NOT sel(2)) AND (NOT sel(1)) AND (NOT sel(0)));
	q(1) <= e AND ((NOT sel(2)) AND (NOT sel(1)) AND (sel(0)));
	q(2) <= e AND ((NOT sel(2)) AND (sel(1)) AND (NOT sel(0)));
	q(3) <= e AND ((NOT sel(2)) AND (sel(1)) AND (sel(0)));
	q(4) <= e AND ((sel(2)) AND (NOT sel(1)) AND (NOT sel(0)));
	q(5) <= e AND ((sel(2)) AND (NOT sel(1)) AND (sel(0)));
	q(6) <= e AND ((sel(2)) AND (sel(1)) AND (NOT sel(0)));
	q(7) <= e AND ((sel(2)) AND (sel(1)) AND (sel(0)));
	
	

END structure;