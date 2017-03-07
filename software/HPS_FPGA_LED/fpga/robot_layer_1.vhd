library IEEE;
use IEEE.std_logic_1164.all;
use IEEE.numeric_std.all;


library work;
use     work.uart_pkg.all;
use     work.spi_master_pkg.all;
use     work.qei_pkg.all;
use     work.pwm_pkg.all;
use     work.debounce_pkg.all;

entity robot_layer_1 is
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

        ----------- ADC (//) ---------
        ad0_sclk : out std_logic;
        ad0_miso : in  std_logic;
        ad0_drdy : in  std_logic;
        ad0_sync : out std_logic;
        ad0_clk  : out std_logic;

        --------- ADC (muxed) --------
        ad1_sclk : out std_logic;
        ad1_mosi : out std_logic;
        ad1_miso : in  std_logic;
        ad1_ss   : out std_logic;
        ad1_drdy : in  std_logic;
        ad1_rst  : out std_logic;

        ---------- H BRIDGE ----------
        m0_pwma  : out std_logic;
        m0_pwmb  : out std_logic;
        m01_fault: in  std_logic; --m01_fault
        
        m1_pwma  : out std_logic;
        m1_pwmb  : out std_logic;
        m01_resetn: out  std_logic; --m01_resetn

        m2_pwma  : out std_logic;
        m2_pwmb  : out std_logic;

        m3_pwma  : out std_logic;
        m3_pwmb  : out std_logic;

        m2345_fault: in  std_logic; --m2345_fault

        m4_pwma  : out std_logic;
        m4_pwmb  : out std_logic;

        m5_pwma  : out std_logic;
        m5_pwmb  : out std_logic;

        m2345_resetn: out  std_logic; --m2345_resetn

        ---------- QEI ----------    
        qei0_a   : in  std_logic;
        qei0_b   : in  std_logic;

        qei1_a   : in  std_logic;
        qei1_b   : in  std_logic;

        qei2_a   : in  std_logic;
        qei2_b   : in  std_logic;
        qei2_z   : in  std_logic;

        qei3_a   : in  std_logic;
        qei3_b   : in  std_logic;
        qei3_z   : in  std_logic;

        ---------- ESC ----------    
        esc0_pwm : out std_logic;
        esc0_dir : out std_logic;

        esc1_pwm : out std_logic;
        esc1_dir : out std_logic;

        ------- PWM (Servos) ------
        s : out std_logic_vector(8-1 downto 0);    

        --------- IOs ----------
        io_0 : in  std_logic;
        io_1 : in  std_logic;
        io_2 : out std_logic;
        io_3 : out std_logic;
        io_4 : in  std_logic;
        io_5 : in  std_logic;
        io_6 : out std_logic;
        io_7 : out std_logic;

        --------- UART ----------
        uart0_rx     : in  std_logic;
        uart0_tx     : out std_logic;

        uart1_rx     : in  std_logic;
        uart1_tx     : out std_logic;

        uart2_rx     : in  std_logic;
        uart2_tx     : out std_logic;
        uart2_custom : out std_logic;

        uart3_rx     : in  std_logic;
        uart3_tx     : out std_logic;
        uart3_custom : out std_logic;

        --------- I2C ----------
        i2c0_scl     : inout std_logic;
        i2c0_sda     : inout std_logic;
        i2c0_reset   : out   std_logic;

        i2c1_scl     : inout std_logic;
        i2c1_sda     : inout std_logic;
        i2c1_reset   : out   std_logic;

        --------- SPI ----------
        spi0_sclk    : out std_logic;
        spi0_mosi    : out std_logic;
        spi0_miso    : in  std_logic;
        spi0_ss      : out std_logic;

        spi1_sclk    : out std_logic;
        spi1_mosi    : out std_logic;
        spi1_miso    : in  std_logic;
        spi1_ss      : out std_logic;

        --! Use SPI1
        imu_ss       : out std_logic;
        imu_drdy     : in  std_logic;
        imu_fsync    : in  std_logic;

        ---------- LED -----------
        led_green : out std_logic;
        led_red   : out std_logic;

        --------- MGMT -----------
        lv_mux    : out std_logic_vector(2-1 downto 0);
        buzzer    : out std_logic;

	    ----------/ NANO SOC LED --------/
	    LED                 : out   std_logic_vector(8-1 downto 0);

	    ----------/ NANO SOC SW --------/
	    SW                  : in    std_logic_vector(4-1 downto 0)
    );
end entity;

architecture rtl of robot_layer_1 is

    signal w_reset_n : std_logic;



    type int8_t   is array (natural range<>) of std_logic_vector(8 -1 downto 0);
    type int16_t  is array (natural range<>) of std_logic_vector(16-1 downto 0);
    type int24_t  is array (natural range<>) of std_logic_vector(24-1 downto 0);
    

    

    signal w_regs_data_in_value      : std_logic_vector(RegCnt*32-1 downto 0);
    signal w_regs_data_in_value_mask : std_logic_vector(RegCnt*4-1 downto 0) := (others=>'0');



    constant MOTOR_COUNT : natural := 6;

    constant REG_MOTOR_OFFSET           : natural := 12;
    constant REG_MOTOR_CURRENT_OFFSET   : natural := REG_MOTOR_OFFSET+MOTOR_COUNT;

    signal w_motor_value    : int16_t(MOTOR_COUNT-1 downto 0);
    signal w_motor_invert   : std_logic_vector(MOTOR_COUNT-1 downto 0);
    signal w_motor_override : std_logic_vector(MOTOR_COUNT-1 downto 0);
    signal w_motor_current  : int24_t(MOTOR_COUNT-1 downto 0);
    signal w_motor_out      : std_logic_vector(MOTOR_COUNT-1 downto 0);
    signal w_motor_dir      : std_logic_vector(MOTOR_COUNT-1 downto 0);


    constant REG_SERVO_OFFSET : natural := 28;

    constant SERVO_COUNT : natural := 8;

    signal w_servo_value    : int8_t(SERVO_COUNT-1 downto 0);
    signal w_servo_enabled  : std_logic_vector(SERVO_COUNT-1 downto 0);
    signal w_servo_override : std_logic_vector(SERVO_COUNT-1 downto 0);
    signal w_servo_current  : int24_t(SERVO_COUNT-1 downto 0);
    signal w_servo_out      : std_logic_vector(SERVO_COUNT-1 downto 0);


    constant REG_ESC_OFFSET : natural := 44;
    constant ESC_COUNT : natural := 2;

    signal w_esc_value    : int16_t(ESC_COUNT-1 downto 0);
    signal w_esc_enabled  : std_logic_vector(ESC_COUNT-1 downto 0);
    signal w_esc_override : std_logic_vector(ESC_COUNT-1 downto 0);
    signal w_esc_current  : int24_t(ESC_COUNT-1 downto 0);
    signal w_esc_out      : std_logic_vector(ESC_COUNT-1 downto 0);
    signal w_esc_dir      : std_logic_vector(ESC_COUNT-1 downto 0);

    constant REG_QEI_OFFSET : natural := 24;
    constant QEI_COUNT : natural := 4;

    signal w_qei_a          : std_logic_vector(QEI_COUNT-1 downto 0);
    signal w_qei_b          : std_logic_vector(QEI_COUNT-1 downto 0);
    signal w_qei_z          : std_logic_vector(QEI_COUNT-1 downto 0);
    signal w_qei_override   : std_logic_vector(QEI_COUNT-1 downto 0);
    signal w_qei_value      : int16_t(QEI_COUNT-1 downto 0);
    signal w_qei_ref        : std_logic_vector(QEI_COUNT-1 downto 0);


    signal r_led_red  : std_logic;

    signal r_clk_25mhz : std_logic;
    signal r_clk_12mhz : std_logic;



    signal r_ad0_miso  : std_logic;
    signal r_ad0_drdy  : std_logic;
    signal r_ad0_en    : std_logic;


    constant MSG_SIZE : natural := 1+2+4*2+2+2;

    signal r_uart_tx_data  : std_logic_vector(MSG_SIZE*8-1 downto 0);
    signal w_uart_tx_valid : std_logic;
    signal w_uart_tx_busy  : std_logic;

    signal w_ad0_rx_busy   : std_logic;
    signal r_ad0_rx_busy   : std_logic;
    signal w_ad0_rx_data   : std_logic_vector(192-1 downto 0);
    signal r_ad0_rx_valid  : std_logic;

    constant VOLTAGE_COUNT : natural := 4;
    constant REG_VOLTAGE_OFFSET : natural := 2;

    signal r_voltage      : int24_t(VOLTAGE_COUNT-1 downto 0); 
    signal r_lv_mux       : std_logic_vector(2-1 downto 0);
    signal r_voltage_cnt  : std_logic_vector(2-1 downto 0); 

    constant REG_BUZZER_OFFSET : natural := 6;

    signal r_buzzer            : std_logic;
    signal w_buzzer_override   : std_logic;
    signal w_buzzer_out        : std_logic;

    constant REG_IO_OFFSET : natural := 7;
    constant IO_COUNT : natural := 4;

    signal w_input_in      : std_logic_vector(IO_COUNT-1 downto 0);
    signal w_input_override: std_logic_vector(IO_COUNT-1 downto 0);
    signal w_input_value   : std_logic_vector(IO_COUNT-1 downto 0);

    signal w_output_value  : std_logic_vector(IO_COUNT-1 downto 0);
    signal w_output_override: std_logic_vector(IO_COUNT-1 downto 0);
    signal w_output_out    : std_logic_vector(IO_COUNT-1 downto 0);


begin
	
    w_reset_n <= not reset;

    --! we return for read the same written data, expect for some bytes (noted masked) where we compute the value internally
    g_reg: for i in 0 to w_regs_data_in_value_mask'length-1 generate
        regs_data_in_value((i+1)*8-1 downto i*8) <= regs_data_out_value((i+1)*8-1 downto i*8) when w_regs_data_in_value_mask(i) = '0' else w_regs_data_in_value((i+1)*8-1 downto i*8);
    end generate;

    p_sync_spi: process(clk) is
    begin
        if rising_edge(clk) then
            r_clk_25mhz <= not r_clk_25mhz;
            r_ad0_drdy  <= ad0_drdy;

            if r_clk_25mhz = '1' then
                r_clk_12mhz <= not r_clk_12mhz;
            end if;
            
            r_ad0_en <= '0';

            if ad0_drdy = '0' and r_ad0_drdy = '1' then
                r_ad0_en <= '1';
            end if;
        end if;
    end process;

    ad0_clk <= r_clk_12mhz;

    ad0_sync <= '1';

    inst_spi_ad0: spi_master
    generic map(
        slaves => 1,
        d_width => w_ad0_rx_data'length
    )
    port map (
        clock     => clk,
        reset_n => w_reset_n,                             
        enable  => r_ad0_en,                               
        cpol    => '0',
        cpha    => '0',
        cont    => '0',
        clk_div => 2,
        addr    => 0,
        tx_data => (others=>'0'),
        miso    => ad0_miso,
        sclk    => ad0_sclk,
        ss_n    => open,
        mosi    => open,
        ss_n    => open,
        busy    => w_ad0_rx_busy,
        rx_data => w_ad0_rx_data

    );	 

    p_sync_ad0_output: process(clk,reset) is
    begin
        if reset = '1' then
            r_ad0_rx_valid <= '0';
        elsif rising_edge(clk) then
            r_ad0_miso <= ad0_miso;
    
            r_ad0_rx_busy <= w_ad0_rx_busy;
            r_ad0_rx_valid<= '0';

            if w_ad0_rx_busy = '0' and r_ad0_rx_busy = '1' then
                r_ad0_rx_valid <= '1';
            end if;
        end if;
    end process;

    p_sync_voltage_mux: process(clk,reset) is
    begin
        if reset = '1' then
            r_voltage_cnt <= (others=>'0');
            r_lv_mux      <= (others=>'0');
            r_buzzer      <= '0';
        elsif rising_edge(clk) then
            if r_ad0_rx_valid = '1' then
                r_voltage_cnt <= std_logic_vector(unsigned(r_voltage_cnt)+1);
                if r_voltage_cnt = (r_voltage_cnt'range=>'1') then
                    r_lv_mux <= std_logic_vector(unsigned(r_lv_mux)+1);
                    r_voltage(to_integer(unsigned(r_lv_mux))) <= w_ad0_rx_data(7*24-1 downto 6*24);
                end if;
            end if;
            
            r_buzzer <= '0';
            for i in 0 to VOLTAGE_COUNT-1 loop
                if unsigned(w_regs_data_in_value(((i+1)+REG_VOLTAGE_OFFSET)*32-1 downto (i+REG_VOLTAGE_OFFSET)*32)) < unsigned(regs_data_out_value(((i+1)+REG_VOLTAGE_OFFSET)*32-1 downto (i+REG_VOLTAGE_OFFSET)*32)) then
                    r_buzzer <= '1';
                end if;
            end loop;
        end if;
    end process;

    b_buzzer: block
        signal w_reg     : std_logic_vector(32-1 downto 0);
    begin
        w_reg <= regs_data_out_value((REG_BUZZER_OFFSET+1)*32-1 downto REG_BUZZER_OFFSET*32);

        w_buzzer_override <= w_reg(8);

        w_buzzer_out <= r_buzzer when w_buzzer_override = '0' else w_reg(0);

        w_regs_data_in_value((REG_BUZZER_OFFSET+1)*32-1 downto REG_BUZZER_OFFSET*32) <= X"0000" & X"00" & "0000000" & w_buzzer_out;  
        w_regs_data_in_value_mask((REG_BUZZER_OFFSET+1)*4-1 downto REG_BUZZER_OFFSET*4) <= "0001";

        g_voltage_mux: for i in 0 to VOLTAGE_COUNT-1 generate
        begin    
            w_regs_data_in_value(((i+1)+REG_VOLTAGE_OFFSET)*32-1 downto (i+REG_VOLTAGE_OFFSET)*32) <= (8-1 downto 0=>r_voltage(i)(r_voltage(i)'high)) & r_voltage(i);
            w_regs_data_in_value_mask(((i+1)+REG_VOLTAGE_OFFSET)*4-1 downto (i+REG_VOLTAGE_OFFSET)*4) <= (others=>'1');
        end generate;
    end block;

    buzzer <= w_buzzer_out;


    w_input_in(0) <= io_0;
    w_input_in(1) <= io_1;
    w_input_in(2) <= io_4;
    w_input_in(3) <= io_5;

    b_io: block
        signal w_input_in_filtered   : std_logic_vector(IO_COUNT-1 downto 0);

        signal w_input_value_reg     : std_logic_vector(32-1 downto 0);
        signal w_input_override_reg  : std_logic_vector(32-1 downto 0);

        signal w_output_value_reg     : std_logic_vector(32-1 downto 0);
        signal w_output_override_reg  : std_logic_vector(32-1 downto 0);
    begin
     

        w_input_value_reg    <= regs_data_out_value(((0+1)+REG_IO_OFFSET)*32-1 downto (0+REG_IO_OFFSET)*32);
        w_input_override_reg <= regs_data_out_value(((1+1)+REG_IO_OFFSET)*32-1 downto (1+REG_IO_OFFSET)*32);

        w_output_value_reg    <= regs_data_out_value(((2+1)+REG_IO_OFFSET)*32-1 downto (2+REG_IO_OFFSET)*32);
        w_output_override_reg <= regs_data_out_value(((3+1)+REG_IO_OFFSET)*32-1 downto (3+REG_IO_OFFSET)*32);


        w_regs_data_in_value_mask(((2+1)+REG_IO_OFFSET)*4-1 downto (2+REG_IO_OFFSET)*4) <= (others=>'1');

        g_io: for i in 0 to IO_COUNT-1 generate
        begin
            inst_in_filter: debounce
            generic map(
                counter_size  => 19 --counter size (19 bits gives 10.5ms with 50MHz clock)
            ) 
            port map(
                clk     => clk,
                button  => w_input_in(i),
                result  => w_input_in_filtered(i)
            );

            w_input_override(i) <= w_input_override_reg(i*8);
            w_input_value(i) <= w_input_in_filtered(i) when w_input_override(i) = '0' else w_input_value_reg(i*8);

            w_output_override(i) <= w_output_override_reg(i*8);
            w_output_out(i)      <= w_output_value(i) when w_output_override(i) = '0' else w_output_value_reg(i*8);

            w_regs_data_in_value((2+REG_IO_OFFSET)*32+(i+1)*8-1 downto (2+REG_IO_OFFSET)*32+i*8) <= "0000000" & w_output_value(i);

        end generate;

    end block;


    io_2 <= w_output_out(0);
    io_3 <= w_output_out(1);
    io_6 <= w_output_out(2);
    io_7 <= w_output_out(3);




    w_motor_current(0) <= w_ad0_rx_data(1*24-1 downto 0*24);
    w_motor_current(1) <= w_ad0_rx_data(2*24-1 downto 1*24);
    w_motor_current(2) <= w_ad0_rx_data(3*24-1 downto 2*24);
    w_motor_current(3) <= w_ad0_rx_data(4*24-1 downto 3*24);
    w_motor_current(4) <= w_ad0_rx_data(5*24-1 downto 4*24);
    w_motor_current(5) <= w_ad0_rx_data(6*24-1 downto 5*24);



    g_motor: for i in 0 to MOTOR_COUNT-1 generate
        signal w_duty : std_logic_vector(15-1 downto 0);
        signal w_reg  : std_logic_vector(32-1 downto 0);
    begin
        
        w_regs_data_in_value(((i+1)+REG_MOTOR_CURRENT_OFFSET)*32-1 downto (i+REG_MOTOR_CURRENT_OFFSET)*32) <= (8-1 downto 0=>w_motor_current(i)(w_motor_current(i)'high)) & w_motor_current(i);
        w_regs_data_in_value_mask(((i+1)+REG_MOTOR_CURRENT_OFFSET)*4-1 downto (i+REG_MOTOR_CURRENT_OFFSET)*4) <= (others=>'1');

        w_reg <= regs_data_out_value(((i+1)+REG_MOTOR_OFFSET)*32-1 downto (i+REG_MOTOR_OFFSET)*32);

        w_motor_invert(i) <= w_reg(16);
        w_motor_override(i) <= w_reg(24);

        w_motor_value(i) <= w_reg(16-1 downto 0) when w_motor_override(i) = '1' else (others=>'0');



        w_duty <= std_logic_vector(abs(signed(w_motor_value(i))))(w_duty'range);

        inst_pwm_motor: pwm 
        generic map(
            sys_clk         => CLK_FREQUENCY_HZ, --system clock frequency in Hz
            pwm_freq        => 20_000,     --PWM switching frequency in Hz
            bits_resolution => w_duty'length,         --bits of resolution setting the duty cycle
            phases          => 1           --number of out : pwms and phases
        )
        port map (
            clk       => clk,
            reset_n   => w_reset_n,                             
            ena       => '1',                               
            duty      => w_duty,
            pwm_out(0)=> w_motor_out(i)
        );
        
        w_motor_dir(i) <= w_motor_value(i)(w_motor_value(i)'high) when w_motor_invert(i) = '0' else not w_motor_value(i)(w_motor_value(i)'high);

    end generate;

    LED(0) <= w_motor_out(0) when w_motor_dir(0) = '0' else not w_motor_out(0);
    LED(1) <= not w_motor_out(0) when w_motor_dir(0) = '0' else w_motor_out(0);
    LED(2) <= w_motor_out(1) when w_motor_dir(1) = '0' else not w_motor_out(1);
    LED(3) <= w_motor_out(2) when w_motor_dir(2) = '0' else not w_motor_out(2);
    LED(4) <= w_motor_out(3) when w_motor_dir(3) = '0' else not w_motor_out(3);
    LED(5) <= w_motor_out(4) when w_motor_dir(4) = '0' else not w_motor_out(4);
    LED(6) <= w_motor_out(5) when w_motor_dir(5) = '0' else not w_motor_out(5);

    m0_pwma  <= w_motor_out(0) and not w_motor_dir(0);
    m0_pwmb  <= w_motor_out(0) and w_motor_dir(0);

    m1_pwma  <= w_motor_out(1) and not w_motor_dir(1);
    m1_pwmb  <= w_motor_out(1) and w_motor_dir(1);

    m2_pwma  <= w_motor_out(2) and not w_motor_dir(2);
    m2_pwmb  <= w_motor_out(2) and w_motor_dir(2);

    m3_pwma  <= w_motor_out(3) and not w_motor_dir(3);
    m3_pwmb  <= w_motor_out(3) and w_motor_dir(3);

    m4_pwma  <= w_motor_out(4) and not w_motor_dir(4);
    m4_pwmb  <= w_motor_out(4) and w_motor_dir(4);

    m5_pwma  <= w_motor_out(5) and not w_motor_dir(5);
    m5_pwmb  <= w_motor_out(5) and w_motor_dir(5);


    p_sync_reset: process(clk, w_reset_n) is
    begin
        if (w_reset_n = '0') then
            m01_resetn <= '0';
            m2345_resetn <= '0';
        elsif rising_edge(clk) then
            m01_resetn <= '1';
            m2345_resetn <= '1';
        end if;
    end process;


    g_servo: for i in 0 to SERVO_COUNT-1 generate
        signal w_duty : std_logic_vector(12-1 downto 0);
        signal w_reg  : std_logic_vector(32-1 downto 0);
    begin

        w_reg <= regs_data_out_value(((i+1)+REG_SERVO_OFFSET)*32-1 downto (i+REG_SERVO_OFFSET)*32);

        w_servo_enabled(i) <= w_reg(8);
        w_servo_override(i) <= w_reg(16);

        w_servo_value(i) <= w_reg(8-1 downto 0) when w_servo_override(i) = '1' else (others=>'0');

        w_duty <= std_logic_vector(to_unsigned(256,w_duty'length)+unsigned(w_servo_value(i)))(w_duty'range);

        inst_pwm_servo: pwm 
        generic map(
            sys_clk         => CLK_FREQUENCY_HZ, --system clock frequency in Hz
            pwm_freq        => 62_500,     --PWM switching frequency in Hz
            bits_resolution => w_duty'length,         --bits of resolution setting the duty cycle
            phases          => 1           --number of out : pwms and phases
        )
        port map (
            clk       => clk,
            reset_n   => w_reset_n,                             
            ena       => w_servo_enabled(i),                               
            duty      => w_duty,
            pwm_out(0)=> w_servo_out(i)
        );
    
    end generate;


    s <= w_servo_out;


    g_esc: for i in 0 to ESC_COUNT-1 generate
        signal w_duty : std_logic_vector(12-1 downto 0);
        signal w_reg  : std_logic_vector(32-1 downto 0);
    begin

        w_reg <= regs_data_out_value(((i+1)+REG_ESC_OFFSET)*32-1 downto (i+REG_ESC_OFFSET)*32);

        w_esc_enabled(i) <= w_reg(8);
        w_esc_override(i) <= w_reg(16);

        w_esc_value(i) <= w_reg(16-1 downto 0) when w_esc_override(i) = '1' else (others=>'0');

        w_duty <= std_logic_vector(to_unsigned(256,w_duty'length)+unsigned(abs(signed(w_esc_value(i)))))(w_duty'range);

        inst_pwm_esc: pwm 
        generic map(
            sys_clk         => CLK_FREQUENCY_HZ,   --system clock frequency in Hz
            pwm_freq        => 62_500,             --PWM switching frequency in Hz
            bits_resolution => w_duty'length,      --bits of resolution setting the duty cycle
            phases          => 1                   --number of out : pwms and phases
        )
        port map (
            clk       => clk,
            reset_n   => w_reset_n,                             
            ena       => w_esc_enabled(i),                               
            duty      => w_duty,
            pwm_out(0)=> w_esc_out(i)
        );

        w_esc_dir(i) <= w_esc_value(i)(w_esc_value(i)'high);
    
    end generate;



    lv_mux <= r_lv_mux;
    
    esc0_pwm <= w_esc_out(0);
    esc1_pwm <= w_esc_out(1);

    
    esc0_dir <= w_esc_dir(0);
    esc1_dir <= w_esc_dir(1);

	 
    w_qei_a(0) <= qei0_a;
    w_qei_b(0) <= qei0_b;
    w_qei_z(0) <= '0';

    w_qei_a(1) <= qei1_a;
    w_qei_b(1) <= qei1_b;
    w_qei_z(1) <= '0';

    w_qei_a(2) <= qei2_a;
    w_qei_b(2) <= qei2_b;
    w_qei_z(2) <= qei2_z;

    w_qei_a(3) <= qei3_a;
    w_qei_b(3) <= qei3_b;
    w_qei_z(3) <= qei3_z;





    g_qei: for i in 0 to QEI_COUNT-1 generate
        signal w_reg    : std_logic_vector(32-1 downto 0);
        signal w_cnt    : std_logic_vector(16-1 downto 0);
        signal r_qei_z  : std_logic;
        signal r2_qei_z : std_logic;
        signal r_ref    : std_logic;
    begin

        w_regs_data_in_value(((i+1)+REG_QEI_OFFSET)*32-1 downto (i+REG_QEI_OFFSET)*32) <= "0000000" & w_qei_override(i) & "0000000" & w_qei_ref(i) & w_qei_value(i);

        w_regs_data_in_value_mask(((i+1)+REG_QEI_OFFSET)*4-1 downto (i+REG_QEI_OFFSET)*4) <= (others=>'1');

        w_reg <= regs_data_out_value(((i+1)+REG_QEI_OFFSET)*32-1 downto (i+REG_QEI_OFFSET)*32);

        w_qei_override(i) <= w_reg(24);


        inst_qei: QuadratureCounterPorts 
        port map (
            clock     => clk,
            QuadA     => w_qei_a(i),                             
            QuadB     => w_qei_b(i),                               
            CounterValue => w_cnt
        );	 	

        w_qei_value(i) <= w_cnt when w_qei_override(i) = '0' else w_reg(16-1 downto 0);
    
        p_sync: process(clk,reset) is
        begin
            if reset = '1' then
                r_ref <= '0';
                r_qei_z <= '0';
            elsif rising_edge(clk) then
                r_qei_z  <= w_qei_z(i);
                r2_qei_z <= r_qei_z;

                if r_qei_z = '1' and r2_qei_z = '0' then
                    r_ref <= '1';
                end if; 
                if r_qei_z = '0' and r_qei_z = '1' then
                    r_ref <= '0';
                end if; 
            end if;
        end process;

        w_qei_ref(i) <= r_ref when w_qei_override(i) = '0' else w_reg(16);

    end generate;



    inst_uart0: uart
    generic map(
        CLK_FREQ => 50e6,
        BAUD_RATE => 1_000_000,
        PARITY_BIT => "none"
    )
    port map (
        CLK     => clk,
        RST     => not w_reset_n,    
                         
        -- UART INTERFACE
        UART_TXD    => uart0_tx,
        UART_RXD    => uart0_rx,
        -- USER DATA INPUT INTERFACE
        DATA_IN     => r_uart_tx_data(r_uart_tx_data'high downto r_uart_tx_data'length-8),
        DATA_SEND   => w_uart_tx_valid, -- when DATA_SEND = 1, data on DATA_IN will be transmit, DATA_SEND can set to 1 only when BUSY = 0
        BUSY        => w_uart_tx_busy, -- when BUSY = 1 transiever is busy, you must not set DATA_SEND to 1
        -- USER DATA OUTPUT INTERFACE
        DATA_OUT    => open,
        DATA_VLD    => open,
        FRAME_ERROR => open

    );	

    w_uart_tx_valid <= not w_uart_tx_busy;


    p_sync_uart_tx: process(clk, w_reset_n) is
    begin
        if (w_reset_n = '0') then
            r_uart_tx_data <= X"A5" & std_logic_vector(to_unsigned(MSG_SIZE,8)) & X"00" & X"00000000" & X"00000000" & X"0000" & X"0000"; 
        elsif rising_edge(clk) then
            if w_uart_tx_busy = '0' then
                r_uart_tx_data <= r_uart_tx_data(r_uart_tx_data'high-8 downto 0) & r_uart_tx_data(r_uart_tx_data'high downto r_uart_tx_data'length-8);
            end if;
            --if r_ad0_rx_valid = '1' then
            --    r_uart_tx_data <= X"A5" & std_logic_vector(to_unsigned(MSG_SIZE,8)) & X"00" & X"00000000" & X"00000000" & X"0000" & X"0000";                 
            --end if;

        end if;
    end process;

    --w_m0_pio_0_in <= r_pid_state & "0000000" & w_ad0_rx_data(24-1 downto  0);
    --w_m1_pio_0_in <= r_pid_state & "0000000" & w_ad0_rx_data(48-1 downto 24);


end architecture;

