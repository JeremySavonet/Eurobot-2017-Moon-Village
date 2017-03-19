library IEEE;
use     IEEE.STD_LOGIC_1164.ALL;
use     IEEE.NUMERIC_STD.ALL;

library work;
use     work.types_pkg.all;

package robot_layer_2_pkg is

    constant MOTOR_COUNT : natural := 6;
    constant QEI_COUNT   : natural := 4;

    component robot_layer_2 is
    generic (
        CLK_FREQUENCY_HZ : positive;
        RegCnt : positive
    );
    port (
        clk                     : in  std_logic;             
        reset                   : in  std_logic;             

        regs_data_in_value      : out  std_logic_vector(RegCnt*32-1 downto 0) := (others => '0'); 
        regs_data_in_read       : in std_logic_vector(RegCnt-1 downto 0);                       
        regs_data_out_value     : in std_logic_vector(RegCnt*32-1 downto 0);                    
        regs_data_out_write     : in std_logic_vector(RegCnt-1 downto 0);

        ---------------------------------
        -------- TO/FROM LAYER 1 --------
        ---------------------------------

        motor_value   : out  int16_t(MOTOR_COUNT-1 downto 0);
        motor_current : in   int24_t(MOTOR_COUNT-1 downto 0);
        motor_fault   : in   std_logic_vector(MOTOR_COUNT-1 downto 0);

        qei_value     : in   int16_t(QEI_COUNT-1 downto 0);
        qei_ref       : in   std_logic_vector(QEI_COUNT-1 downto 0)

        ---------------------------------
        -------- TO/FROM LAYER 3 --------
        ---------------------------------

    );   
    end component;


end package;
