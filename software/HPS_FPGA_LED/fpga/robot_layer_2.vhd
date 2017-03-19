library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


library work;
use     work.types_pkg.all;
use     work.robot_layer_2_pkg.all;

entity robot_layer_2 is
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
end entity;

architecture rtl of robot_layer_2 is

    component system is
        port (
            clk_clk            : in  std_logic                      := 'X';             -- clk
            pio_data_in_value  : in  std_logic_vector(511 downto 0) := (others => 'X'); -- data_in_value
            pio_data_in_read   : out std_logic_vector(15 downto 0);                     -- data_in_read
            pio_data_out_value : out std_logic_vector(511 downto 0);                    -- data_out_value
            pio_data_out_write : out std_logic_vector(15 downto 0);                     -- data_out_write
            reset_reset_n      : in  std_logic                      := 'X'              -- reset_n
        );
    end component system;


    signal w_reset_n : std_logic;


    signal w_regs_data_in_value      : std_logic_vector(RegCnt*32-1 downto 0);
    signal w_regs_data_in_value_mask : std_logic_vector(RegCnt*4-1 downto 0) := (others=>'0');

--    constant MSG_SIZE : natural := 1+2+4*2+2+2;

--    signal r_uart_tx_data  : std_logic_vector(MSG_SIZE*8-1 downto 0);
--    signal w_uart_tx_valid : std_logic;
--    signal w_uart_tx_busy  : std_logic;


    constant REG_STATE_CONFIG_OFFSET : natural := 1;
    signal w_sim_mode : std_logic;

begin
	
    w_reset_n <= not reset;

    --! we return for read the same written data, expect for some bytes (noted masked) where we compute the value internally
    g_reg: for i in 0 to w_regs_data_in_value_mask'length-1 generate
        regs_data_in_value((i+1)*8-1 downto i*8) <= regs_data_out_value((i+1)*8-1 downto i*8) when w_regs_data_in_value_mask(i) = '0' else w_regs_data_in_value((i+1)*8-1 downto i*8);
    end generate;

    motor_value <= (others=>(others=>'0'));

    b_odometry: block

        signal w_input_value_reg     : std_logic_vector(32-1 downto 0);

        signal w_pio_data_in_value   :  std_logic_vector(511 downto 0);
        signal w_pio_data_in_read    :  std_logic_vector(15 downto 0);
        signal w_pio_data_out_value  :  std_logic_vector(511 downto 0);
        signal w_pio_data_out_write  :  std_logic_vector(15 downto 0);
    begin


        w_regs_data_in_value_mask((8+1)*4-1 downto (0+8)*4) <= "0011";
        w_regs_data_in_value_mask((7+9)*4-1 downto (0+9)*4) <= (others=>'1');

        p_async: process(regs_data_out_value,w_pio_data_out_value,qei_value) is
        begin
            w_pio_data_in_value(2*32-1 downto 0*32)      <= X"00000000" & X"00000000";
            w_pio_data_in_value((2+14)*32-1 downto 2*32) <= regs_data_out_value((2+14)*32-1 downto 2*32);

            --! we override the values for register 9 & 10 to give QEI inputs
            w_pio_data_in_value((9+2)*32-1 downto 9*32) <= qei_value(3) & qei_value(2) & qei_value(1) & qei_value(0);


            w_regs_data_in_value((8+8)*32-1 downto (0+8)*32) <= w_pio_data_out_value((8+8)*32-1 downto (0+8)*32); 
            
        end process;

        --! disable warnings
        assert w_pio_data_in_read = w_pio_data_in_read;
        assert w_pio_data_out_write = w_pio_data_out_write;

        inst_odometry_rv : component system
        port map (
            clk_clk                 => clk,
            reset_reset_n           => w_reset_n,
            pio_data_in_value       => w_pio_data_in_value,--regs_data_out_value((2+16)*32-1 downto 2*32),  --   pio.data_in_value
            pio_data_in_read        => w_pio_data_in_read,
            pio_data_out_value      => w_pio_data_out_value,
            pio_data_out_write      => w_pio_data_out_write
        );



    end block;


end architecture;

