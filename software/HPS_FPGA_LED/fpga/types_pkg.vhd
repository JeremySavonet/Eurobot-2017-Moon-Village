library IEEE;
use     IEEE.STD_LOGIC_1164.ALL;
use     IEEE.NUMERIC_STD.ALL;

package types_pkg is

    type int8_t   is array (natural range<>) of std_logic_vector(8 -1 downto 0);
    type int16_t  is array (natural range<>) of std_logic_vector(16-1 downto 0);
    type int24_t  is array (natural range<>) of std_logic_vector(24-1 downto 0);
    type int32_t  is array (natural range<>) of std_logic_vector(32-1 downto 0);
    
end package;
