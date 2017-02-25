--=================================
-- Hello world
-- Author: J.Savonet
--=================================

--`define ENABLE_HPS
----`define ENABLE_CLK

library ieee;
use     ieee.std_logic_1164.all;
use     ieee.numeric_std.all;


entity hpsfpga is
    port (
	----------/ ADC --------/
	ADC_CONVST  : out std_logic;
	ADC_SCK     : out std_logic;
	ADC_SDI     : out std_logic;
	ADC_SDO     : in  std_logic;

	----------/ ARDUINO --------/
	--ARDUINO_IO      : inout std_logic_vector(16-1 downto 0);
	--ARDUINO_RESET_N : inout std_logic;

--`ifdef ENABLE_CLK
--	--------/ CLK --------/
--	out :             CLK_I2C_SCL,
--	inout :              CLK_I2C_SDA,
--`endif /*ENABLE_CLK*/

	----------/ FPGA --------/
	FPGA_CLK1_50 : in std_logic;
	FPGA_CLK2_50 : in std_logic;
	FPGA_CLK3_50 : in std_logic;

	----------/ GPIO --------/
	--GPIO_0  : inout   std_logic_vector(36-1 downto 0);
	--GPIO_1  : inout   std_logic_vector(36-1 downto 0);


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
    esc0_pwm : in  std_logic;
    esc0_dir : out std_logic;

    esc1_pwm : in  std_logic;
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

--`ifdef ENABLE_HPS
	----------/ HPS --------/
	HPS_CONV_USB_N      : inout std_logic;
	HPS_DDR3_ADDR       : out   std_logic_vector(15-1 downto 0); 
	HPS_DDR3_BA         : out   std_logic_vector(3-1 downto 0);
	HPS_DDR3_CAS_N      : out   std_logic;
	HPS_DDR3_CKE        : out   std_logic;
	HPS_DDR3_CK_N       : out   std_logic;
	HPS_DDR3_CK_P       : out   std_logic;
	HPS_DDR3_CS_N       : out   std_logic;
	HPS_DDR3_DM         : out   std_logic_vector(4-1 downto 0);
	HPS_DDR3_DQ         : inout std_logic_vector(32-1 downto 0);
	HPS_DDR3_DQS_N      : inout std_logic_vector(4-1 downto 0); 
	HPS_DDR3_DQS_P      : inout std_logic_vector(4-1 downto 0);
	HPS_DDR3_ODT        : out   std_logic;
	HPS_DDR3_RAS_N      : out   std_logic;
	HPS_DDR3_RESET_N    : out   std_logic;
	HPS_DDR3_RZQ        : in    std_logic;
	HPS_DDR3_WE_N       : out   std_logic;
	HPS_ENET_GTX_CLK    : out   std_logic;
	HPS_ENET_INT_N      : inout std_logic;
	HPS_ENET_MDC        : out   std_logic;
	HPS_ENET_MDIO       : inout std_logic;
	HPS_ENET_RX_CLK     : in    std_logic;
	HPS_ENET_RX_DATA    : in    std_logic_vector(4-1 downto 0);
	HPS_ENET_RX_DV      : in    std_logic;
	HPS_ENET_TX_DATA    : out   std_logic_vector(4-1 downto 0);
	HPS_ENET_TX_EN      : out   std_logic;
	HPS_GSENSOR_INT     : inout std_logic;
	HPS_I2C0_SCLK       : inout std_logic;
	HPS_I2C0_SDAT       : inout std_logic;
	HPS_I2C1_SCLK       : inout std_logic;
	HPS_I2C1_SDAT       : inout std_logic;
	HPS_KEY             : inout std_logic;
	HPS_LED             : inout std_logic;
	HPS_LTC_GPIO        : inout std_logic;
	HPS_SD_CLK          : out   std_logic;
	HPS_SD_CMD          : inout std_logic;
	HPS_SD_DATA         : inout std_logic_vector(4-1 downto 0);
	HPS_SPIM_CLK        : out   std_logic;
	HPS_SPIM_MISO       : in    std_logic;
	HPS_SPIM_MOSI       : out   std_logic;
	HPS_SPIM_SS         : inout std_logic;
	HPS_UART_RX         : in    std_logic;
	HPS_UART_TX         : out   std_logic;
	HPS_USB_CLKOUT      : in    std_logic;
	HPS_USB_DATA        : inout std_logic_vector(8-1 downto 0);
	HPS_USB_DIR         : in    std_logic;
	HPS_USB_NXT         : in    std_logic;
	HPS_USB_STP         : out   std_logic;
--`endif /*ENABLE_HPS*/

	----------/ KEY --------/
	KEY                 : in    std_logic_vector(2-1 downto 0);

	----------/ LED --------/
	LED                 : out   std_logic_vector(8-1 downto 0);

	----------/ SW --------/
	SW                  : in    std_logic_vector(4-1 downto 0)
);
end entity;

architecture hpsfpga_arch of hpsfpga is

	component system is
		port (
			clk_clk        : in  std_logic                     := 'X';             -- clk
			pio_0_in_port  : in  std_logic_vector(31 downto 0) := (others => 'X'); -- in_port
			pio_0_out_port : out std_logic_vector(31 downto 0);                    -- out_port
			pio_1_in_port  : in  std_logic_vector(31 downto 0) := (others => 'X'); -- in_port
			pio_1_out_port : out std_logic_vector(31 downto 0);                    -- out_port
			pio_2_in_port  : in  std_logic_vector(31 downto 0) := (others => 'X'); -- in_port
			pio_2_out_port : out std_logic_vector(31 downto 0);                    -- out_port
			pio_3_in_port  : in  std_logic_vector(31 downto 0) := (others => 'X'); -- in_port
			pio_3_out_port : out std_logic_vector(31 downto 0);                    -- out_port
			pio_4_in_port  : in  std_logic_vector(31 downto 0) := (others => 'X'); -- in_port
			pio_4_out_port : out std_logic_vector(31 downto 0);                    -- out_port
			pio_5_in_port  : in  std_logic_vector(31 downto 0) := (others => 'X'); -- in_port
			pio_5_out_port : out std_logic_vector(31 downto 0);                    -- out_port
			reset_reset_n  : in  std_logic                     := 'X'              -- reset_n
		);
	end component system;



	component hps_fpga is
		port (
			button_pio_external_connection_export : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			clk_clk                               : in    std_logic                     := 'X';             -- clk
			dipsw_pio_external_connection_export  : in    std_logic_vector(3 downto 0)  := (others => 'X'); -- export
			hps_0_h2f_reset_reset_n               : out   std_logic;                                        -- reset_n
			hps_0_hps_io_hps_io_emac1_inst_TX_CLK : out   std_logic;                                        -- hps_io_emac1_inst_TX_CLK
			hps_0_hps_io_hps_io_emac1_inst_TXD0   : out   std_logic;                                        -- hps_io_emac1_inst_TXD0
			hps_0_hps_io_hps_io_emac1_inst_TXD1   : out   std_logic;                                        -- hps_io_emac1_inst_TXD1
			hps_0_hps_io_hps_io_emac1_inst_TXD2   : out   std_logic;                                        -- hps_io_emac1_inst_TXD2
			hps_0_hps_io_hps_io_emac1_inst_TXD3   : out   std_logic;                                        -- hps_io_emac1_inst_TXD3
			hps_0_hps_io_hps_io_emac1_inst_RXD0   : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RXD0
			hps_0_hps_io_hps_io_emac1_inst_MDIO   : inout std_logic                     := 'X';             -- hps_io_emac1_inst_MDIO
			hps_0_hps_io_hps_io_emac1_inst_MDC    : out   std_logic;                                        -- hps_io_emac1_inst_MDC
			hps_0_hps_io_hps_io_emac1_inst_RX_CTL : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RX_CTL
			hps_0_hps_io_hps_io_emac1_inst_TX_CTL : out   std_logic;                                        -- hps_io_emac1_inst_TX_CTL
			hps_0_hps_io_hps_io_emac1_inst_RX_CLK : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RX_CLK
			hps_0_hps_io_hps_io_emac1_inst_RXD1   : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RXD1
			hps_0_hps_io_hps_io_emac1_inst_RXD2   : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RXD2
			hps_0_hps_io_hps_io_emac1_inst_RXD3   : in    std_logic                     := 'X';             -- hps_io_emac1_inst_RXD3
			hps_0_hps_io_hps_io_sdio_inst_CMD     : inout std_logic                     := 'X';             -- hps_io_sdio_inst_CMD
			hps_0_hps_io_hps_io_sdio_inst_D0      : inout std_logic                     := 'X';             -- hps_io_sdio_inst_D0
			hps_0_hps_io_hps_io_sdio_inst_D1      : inout std_logic                     := 'X';             -- hps_io_sdio_inst_D1
			hps_0_hps_io_hps_io_sdio_inst_CLK     : out   std_logic;                                        -- hps_io_sdio_inst_CLK
			hps_0_hps_io_hps_io_sdio_inst_D2      : inout std_logic                     := 'X';             -- hps_io_sdio_inst_D2
			hps_0_hps_io_hps_io_sdio_inst_D3      : inout std_logic                     := 'X';             -- hps_io_sdio_inst_D3
			hps_0_hps_io_hps_io_usb1_inst_D0      : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D0
			hps_0_hps_io_hps_io_usb1_inst_D1      : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D1
			hps_0_hps_io_hps_io_usb1_inst_D2      : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D2
			hps_0_hps_io_hps_io_usb1_inst_D3      : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D3
			hps_0_hps_io_hps_io_usb1_inst_D4      : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D4
			hps_0_hps_io_hps_io_usb1_inst_D5      : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D5
			hps_0_hps_io_hps_io_usb1_inst_D6      : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D6
			hps_0_hps_io_hps_io_usb1_inst_D7      : inout std_logic                     := 'X';             -- hps_io_usb1_inst_D7
			hps_0_hps_io_hps_io_usb1_inst_CLK     : in    std_logic                     := 'X';             -- hps_io_usb1_inst_CLK
			hps_0_hps_io_hps_io_usb1_inst_STP     : out   std_logic;                                        -- hps_io_usb1_inst_STP
			hps_0_hps_io_hps_io_usb1_inst_DIR     : in    std_logic                     := 'X';             -- hps_io_usb1_inst_DIR
			hps_0_hps_io_hps_io_usb1_inst_NXT     : in    std_logic                     := 'X';             -- hps_io_usb1_inst_NXT
			hps_0_hps_io_hps_io_spim1_inst_CLK    : out   std_logic;                                        -- hps_io_spim1_inst_CLK
			hps_0_hps_io_hps_io_spim1_inst_MOSI   : out   std_logic;                                        -- hps_io_spim1_inst_MOSI
			hps_0_hps_io_hps_io_spim1_inst_MISO   : in    std_logic                     := 'X';             -- hps_io_spim1_inst_MISO
			hps_0_hps_io_hps_io_spim1_inst_SS0    : out   std_logic;                                        -- hps_io_spim1_inst_SS0
			hps_0_hps_io_hps_io_uart0_inst_RX     : in    std_logic                     := 'X';             -- hps_io_uart0_inst_RX
			hps_0_hps_io_hps_io_uart0_inst_TX     : out   std_logic;                                        -- hps_io_uart0_inst_TX
			hps_0_hps_io_hps_io_i2c0_inst_SDA     : inout std_logic                     := 'X';             -- hps_io_i2c0_inst_SDA
			hps_0_hps_io_hps_io_i2c0_inst_SCL     : inout std_logic                     := 'X';             -- hps_io_i2c0_inst_SCL
			hps_0_hps_io_hps_io_i2c1_inst_SDA     : inout std_logic                     := 'X';             -- hps_io_i2c1_inst_SDA
			hps_0_hps_io_hps_io_i2c1_inst_SCL     : inout std_logic                     := 'X';             -- hps_io_i2c1_inst_SCL
			led_pio_external_connection_export    : out   std_logic_vector(7 downto 0);                     -- export
			memory_mem_a                          : out   std_logic_vector(14 downto 0);                    -- mem_a
			memory_mem_ba                         : out   std_logic_vector(2 downto 0);                     -- mem_ba
			memory_mem_ck                         : out   std_logic;                                        -- mem_ck
			memory_mem_ck_n                       : out   std_logic;                                        -- mem_ck_n
			memory_mem_cke                        : out   std_logic;                                        -- mem_cke
			memory_mem_cs_n                       : out   std_logic;                                        -- mem_cs_n
			memory_mem_ras_n                      : out   std_logic;                                        -- mem_ras_n
			memory_mem_cas_n                      : out   std_logic;                                        -- mem_cas_n
			memory_mem_we_n                       : out   std_logic;                                        -- mem_we_n
			memory_mem_reset_n                    : out   std_logic;                                        -- mem_reset_n
			memory_mem_dq                         : inout std_logic_vector(31 downto 0) := (others => 'X'); -- mem_dq
			memory_mem_dqs                        : inout std_logic_vector(3 downto 0)  := (others => 'X'); -- mem_dqs
			memory_mem_dqs_n                      : inout std_logic_vector(3 downto 0)  := (others => 'X'); -- mem_dqs_n
			memory_mem_odt                        : out   std_logic;                                        -- mem_odt
			memory_mem_dm                         : out   std_logic_vector(3 downto 0);                     -- mem_dm
			memory_oct_rzqin                      : in    std_logic                     := 'X';             -- oct_rzqin
			reset_reset_n                         : in    std_logic                     := 'X'              -- reset_n
		);
	end component hps_fpga;



----=======================================================
----  REG/WIRE declarations
----=======================================================
    signal hps_fpga_reset_n : std_logic;


	signal cnt : std_logic_vector(32-4-1 downto 0);
	signal dir : std_logic;


    signal w_qei0_cnt : std_logic_vector(16-1 downto 0);
    signal w_qei1_cnt : std_logic_vector(16-1 downto 0);

    signal r_qei0_cnt : std_logic_vector(16-1 downto 0);
    signal r_qei1_cnt : std_logic_vector(16-1 downto 0);

    signal r_led_red  : std_logic;

    signal r_clk_25mhz : std_logic;
    signal r_clk_12mhz : std_logic;



    signal r_ad0_miso  : std_logic;
    signal r_ad0_drdy  : std_logic;
    signal r_ad0_en    : std_logic;

	 
    signal r_m0_duty : std_logic_vector(16-1 downto 0);
    signal r_m0_dir  : std_logic;

    signal r_m1_duty : std_logic_vector(16-1 downto 0);
    signal r_m1_dir  : std_logic;


    signal w_m0_pwm : std_logic;
    signal w_m1_pwm : std_logic;
    signal w_m2_pwm : std_logic;
    signal w_m3_pwm : std_logic;
    signal w_m4_pwm : std_logic;
    signal w_m5_pwm : std_logic;

    constant MSG_SIZE : natural := 1+2+4*2+2+2;

    signal r_uart_tx_data  : std_logic_vector(MSG_SIZE*8-1 downto 0);
    signal w_uart_tx_valid : std_logic;
    signal w_uart_tx_busy  : std_logic;

    signal w_ad0_rx_busy   : std_logic;
    signal r_ad0_rx_busy   : std_logic;
    signal w_ad0_rx_data   : std_logic_vector(192-1 downto 0);
    signal r_ad0_rx_valid  : std_logic;
    signal r_pid_state     : std_logic;

    function reverse_vector (a: in std_logic_vector) return std_logic_vector is
        variable result: std_logic_vector(a'RANGE);
        alias aa: std_logic_vector(a'REVERSE_RANGE) is a;
    begin
        for i in aa'RANGE loop
            result(i) := aa(i);
        end loop;
        return result;
    end; -- function reverse_any_vector

    -- changes the endianess BIG <-> LITTLE
    function ChangeEndian(vec : std_ulogic_vector) return std_ulogic_vector is
        variable vRet      : std_ulogic_vector(vec'range);
        constant cNumBytes : natural := vec'length / 8;
    begin
        for i in 0 to cNumBytes-1 loop
          for j in 7 downto 0 loop
            vRet(8*i + j) := vec(8*(cNumBytes-1-i) + j);
          end loop;  -- j
        end loop;  -- i

        return vRet;
    end function ChangeEndian;


    function ChangeEndian(vec : std_logic_vector) return std_logic_vector is
    begin
        return std_logic_vector(ChangeEndian(std_ulogic_vector(vec)));
    end function ChangeEndian;

    signal w_m0_pio_0_in  : std_logic_vector(32-1 downto 0);
    signal w_m0_pio_0_out : std_logic_vector(32-1 downto 0);

    signal w_m1_pio_0_in  : std_logic_vector(32-1 downto 0);
    signal w_m1_pio_0_out : std_logic_vector(32-1 downto 0);


begin
	
    p_sync_led: process(FPGA_CLK1_50, hps_fpga_reset_n) is
    begin
        if (hps_fpga_reset_n = '0') then
            cnt  <= (others=>'0');
            dir  <= '0';

            m01_resetn <= '0';
            m2345_resetn <= '0';


        elsif rising_edge(FPGA_CLK1_50) then
            m01_resetn <= '1';
            m2345_resetn <= '1';


            if (dir = '0') then
                cnt <= std_logic_vector(unsigned(cnt) + 1);
                if (cnt = X"FFFFFFE") then
                    dir <= '1';
                end if;
            else
            cnt <= std_logic_vector(unsigned(cnt) - 1);
	             if (cnt = X"0000001") then
	                 dir <= '0';
	             end if;		    
            end if;
        end if;
    end process;

    LED(0) <= '0';    

    inst_pwm_0: entity work.pwm 
    generic map(
        sys_clk         => 50_000_000, --system clock frequency in Hz
        pwm_freq        => 20_000,     --PWM switching frequency in Hz
        bits_resolution => 16,         --bits of resolution setting the duty cycle
        phases          => 1           --number of out : pwms and phases
    )
    port map (
        clk       => FPGA_CLK1_50,
        reset_n   => hps_fpga_reset_n,                             
        ena       => '1',                               
        duty      => cnt(31-4 downto 16-4),
        pwm_out(0)=> led_green    
    );


    inst_pwm_1: entity work.pwm 
    generic map(
        sys_clk         => 50_000_000, --system clock frequency in Hz
        pwm_freq        => 20_000,     --PWM switching frequency in Hz
        bits_resolution => 15,         --bits of resolution setting the duty cycle
        phases          => 1           --number of out : pwms and phases
    )
    port map (
        clk       => FPGA_CLK1_50,
        reset_n   => hps_fpga_reset_n,                             
        ena       => '1',                               
        duty      => cnt(30-4 downto 16-4),
        pwm_out   => LED(1 downto 1)    
    );

    inst_pwm_2: entity work.pwm 
    generic map(
        sys_clk         => 50_000_000, --system clock frequency in Hz
        pwm_freq        => 20_000,     --PWM switching frequency in Hz
        bits_resolution => 14,         --bits of resolution setting the duty cycle
        phases          => 1           --number of out : pwms and phases
    )
    port map (
        clk       => FPGA_CLK1_50,
        reset_n   => hps_fpga_reset_n,                             
        ena       => '1',                               
        duty      => cnt(29-4 downto 16-4),
        pwm_out   => LED(2 downto 2)    
    );

    inst_pwm_3: entity work.pwm 
    generic map(
        sys_clk         => 50_000_000, --system clock frequency in Hz
        pwm_freq        => 20_000,     --PWM switching frequency in Hz
        bits_resolution => 13,         --bits of resolution setting the duty cycle
        phases          => 1           --number of out : pwms and phases
    )
    port map (
        clk       => FPGA_CLK1_50,
        reset_n   => hps_fpga_reset_n,                             
        ena       => '1',                               
        duty      => cnt(28-4 downto 16-4),
        pwm_out   => LED(3 downto 3)    
    );
	 
	 
    p_async: process(SW,dir,r_m0_dir,r_m1_dir,w_m0_pwm,w_m1_pwm,w_m2_pwm,w_m3_pwm,w_m4_pwm,w_m5_pwm) is
    begin
        m0_pwma <= '0';
        m1_pwma <= '0';
        m2_pwma <= '0';
        m3_pwma <= '0';
        m4_pwma <= '0';
        m5_pwma <= '0';

        m0_pwmb <= '0';
        m1_pwmb <= '0';
        m2_pwmb <= '0';
        m3_pwmb <= '0';
        m4_pwmb <= '0';
        m5_pwmb <= '0';		
	 
        if (dir = '0') then
            --m0_pwma <= w_m0_pwm;
            --m1_pwma <= w_m1_pwm;
            m2_pwma <= w_m2_pwm;
            m3_pwma <= w_m3_pwm;
            m4_pwma <= w_m4_pwm;
            m5_pwma <= w_m5_pwm;

            if (SW(1) = '1') then
                --m0_pwmb <= not w_m0_pwm;
                --m1_pwmb <= not w_m1_pwm;
                m2_pwmb <= not w_m2_pwm;
                m3_pwmb <= not w_m3_pwm;
                m4_pwmb <= not w_m4_pwm;
                m5_pwmb <= not w_m5_pwm;
            end if;
        else
            --m0_pwmb <= w_m0_pwm;
            --m1_pwmb <= w_m1_pwm;
            m2_pwmb <= w_m2_pwm;
            m3_pwmb <= w_m3_pwm;
            m4_pwmb <= w_m4_pwm;
            m5_pwmb <= w_m5_pwm;	
	  
            if (SW(1) = '1') then
                --m0_pwma <= not w_m0_pwm;
                --m1_pwma <= not w_m1_pwm;
                m2_pwma <= not w_m2_pwm;
                m3_pwma <= not w_m3_pwm;
                m4_pwma <= not w_m4_pwm;
                m5_pwma <= not w_m5_pwm;
            end if;
        end if;



        if (r_m0_dir = '0') then
            m0_pwma <= w_m0_pwm;
            if (SW(1) = '1') then
                m0_pwmb <= not w_m0_pwm;
            end if;
        else
            m0_pwmb <= w_m0_pwm;	
            if (SW(1) = '1') then
                m0_pwma <= not w_m0_pwm;
            end if;
        end if;

        if (r_m1_dir = '0') then
            m1_pwma <= w_m1_pwm;
            if (SW(1) = '1') then
                m1_pwmb <= not w_m1_pwm;
            end if;
        else
            m1_pwmb <= w_m1_pwm;	
            if (SW(1) = '1') then
                m1_pwma <= not w_m1_pwm;
            end if;
        end if;



	 end process;


    inst_pwm_m0: entity work.pwm 
    generic map(
        sys_clk         => 50_000_000, --system clock frequency in Hz
        pwm_freq        => 20_000,     --PWM switching frequency in Hz
        bits_resolution => 16,         --bits of resolution setting the duty cycle
        phases          => 1           --number of out : pwms and phases
    )
    port map (
        clk       => FPGA_CLK1_50,
        reset_n   => hps_fpga_reset_n,                             
        ena       => '1',                               
        duty      => r_m0_duty,
        pwm_out(0)=> w_m0_pwm
    );	 
	 
	 
    inst_pwm_m1: entity work.pwm 
    generic map(
        sys_clk         => 50_000_000, --system clock frequency in Hz
        pwm_freq        => 20_000,     --PWM switching frequency in Hz
        bits_resolution => 16,         --bits of resolution setting the duty cycle
        phases          => 1           --number of out : pwms and phases
    )
    port map (
        clk       => FPGA_CLK1_50,
        reset_n   => hps_fpga_reset_n,                             
        ena       => '1',                               
        duty      => r_m1_duty,
        pwm_out(0)=> w_m1_pwm   
    );	 	 


    inst_pwm_m2: entity work.pwm 
    generic map(
        sys_clk         => 50_000_000, --system clock frequency in Hz
        pwm_freq        => 20_000,     --PWM switching frequency in Hz
        bits_resolution => 16,         --bits of resolution setting the duty cycle
        phases          => 1           --number of out : pwms and phases
    )
    port map (
        clk       => FPGA_CLK1_50,
        reset_n   => hps_fpga_reset_n,                             
        ena       => '1',                               
        duty      => X"5555",
        pwm_out(0)=> w_m2_pwm    
    );	 

    inst_pwm_m3: entity work.pwm 
    generic map(
        sys_clk         => 50_000_000, --system clock frequency in Hz
        pwm_freq        => 20_000,     --PWM switching frequency in Hz
        bits_resolution => 16,         --bits of resolution setting the duty cycle
        phases          => 1           --number of out : pwms and phases
    )
    port map (
        clk       => FPGA_CLK1_50,
        reset_n   => hps_fpga_reset_n,                             
        ena       => '1',                               
        duty      => X"5555",
        pwm_out(0)=> w_m3_pwm    
    );	 

    inst_pwm_m4: entity work.pwm 
    generic map(
        sys_clk         => 50_000_000, --system clock frequency in Hz
        pwm_freq        => 20_000,     --PWM switching frequency in Hz
        bits_resolution => 16,         --bits of resolution setting the duty cycle
        phases          => 1           --number of out : pwms and phases
    )
    port map (
        clk       => FPGA_CLK1_50,
        reset_n   => hps_fpga_reset_n,                             
        ena       => '1',                               
        duty      => X"5555",
        pwm_out(0)=> w_m4_pwm    
    );	 

    inst_pwm_m5: entity work.pwm 
    generic map(
        sys_clk         => 50_000_000, --system clock frequency in Hz
        pwm_freq        => 20_000,     --PWM switching frequency in Hz
        bits_resolution => 16,         --bits of resolution setting the duty cycle
        phases          => 1           --number of out : pwms and phases
    )
    port map (
        clk       => FPGA_CLK1_50,
        reset_n   => hps_fpga_reset_n,                             
        ena       => '1',                               
        duty      => X"5555",
        pwm_out(0)=> w_m5_pwm    
    );	 


	 
    inst_qei0: entity work.QuadratureCounterPorts 
    port map (
        clock     => FPGA_CLK1_50,
        QuadA     => qei0_a,                             
        QuadB     => qei0_b,                               
        CounterValue => w_qei0_cnt
    );	 		 


    inst_qei1: entity work.QuadratureCounterPorts 
    port map (
        clock     => FPGA_CLK1_50,
        QuadA     => qei1_a,                             
        QuadB     => qei1_b,                               
        CounterValue => w_qei1_cnt
    );	 		

    led_red <= r_led_red;
    
    p_sync: process(FPGA_CLK1_50) is
    begin
        if rising_edge(FPGA_CLK1_50) then
            r_qei0_cnt <= w_qei0_cnt;
            r_qei1_cnt <= w_qei1_cnt;

            if w_qei0_cnt /= r_qei0_cnt and unsigned(w_qei0_cnt) = 0 then
                r_led_red <= not r_led_red;
            end if; 

            if w_qei1_cnt /= r_qei1_cnt and unsigned(w_qei1_cnt) = 0 then
                r_led_red <= not r_led_red;                
            end if; 
        end if;
    end process;


    p_sync_spi: process(FPGA_CLK1_50) is
    begin
        if rising_edge(FPGA_CLK1_50) then
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


    --ad0_sclk : out std_logic;
    --ad0_miso : in  std_logic;
    --ad0_drdy : in  std_logic;
    --ad0_sync : in  std_logic;
    --ad0_clk  : out std_logic;

    ad0_sync <= '1';

    inst_spi_ad0: entity work.spi_master
    generic map(
        slaves => 1,
        d_width => 192
    )
    port map (
        clock     => FPGA_CLK1_50,
        reset_n => hps_fpga_reset_n,                             
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

    p_sync_ad0_output: process(FPGA_CLK1_50) is
    begin
        if rising_edge(FPGA_CLK1_50) then
            r_ad0_miso <= ad0_miso;
    
            r_ad0_rx_busy <= w_ad0_rx_busy;
            r_ad0_rx_valid<= '0';

            if w_ad0_rx_busy = '0' and r_ad0_rx_busy = '1' then
                r_ad0_rx_valid <= '1';
                r_pid_state <= not r_pid_state;
            end if;
        end if;
    end process;



    inst_uart0: entity work.uart
    generic map(
        CLK_FREQ => 50e6,
        BAUD_RATE => 1_000_000,
        PARITY_BIT => "none"
    )
    port map (
        CLK     => FPGA_CLK1_50,
        RST     => not hps_fpga_reset_n,    
                         
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


    p_sync_uart_tx: process(FPGA_CLK1_50, hps_fpga_reset_n) is
    begin
        if (hps_fpga_reset_n = '0') then
            r_uart_tx_data <= X"A5" & std_logic_vector(to_unsigned(MSG_SIZE,8)) & X"00" & X"00000000" & X"00000000" & X"0000" & X"0000"; 
        elsif rising_edge(FPGA_CLK1_50) then
            if w_uart_tx_busy = '0' then
                r_uart_tx_data <= r_uart_tx_data(r_uart_tx_data'high-8 downto 0) & r_uart_tx_data(r_uart_tx_data'high downto r_uart_tx_data'length-8);
            end if;
            --if r_ad0_rx_valid = '1' then
            --    r_uart_tx_data <= X"A5" & std_logic_vector(to_unsigned(MSG_SIZE,8)) & X"00" & X"00000000" & X"00000000" & X"0000" & X"0000";                 
            --end if;




        end if;
    end process;

    w_m0_pio_0_in <= r_pid_state & "0000000" & w_ad0_rx_data(24-1 downto  0);
    w_m1_pio_0_in <= r_pid_state & "0000000" & w_ad0_rx_data(48-1 downto 24);



    p_sync_pid: process(FPGA_CLK1_50, hps_fpga_reset_n) is
        variable v: integer;
    begin
        if (hps_fpga_reset_n = '0') then
            r_m0_duty <= (others=>'0');
            r_m0_dir  <= '0';

            r_m1_duty <= (others=>'0');
            r_m1_dir  <= '0';
        elsif rising_edge(FPGA_CLK1_50) then
            v := to_integer(signed(w_m0_pio_0_out));
            if v >= 0 then
                r_m0_duty <= std_logic_vector(to_unsigned(abs(v),16));
                r_m0_dir <= '1';
            elsif v < 0 then
                r_m0_duty <= std_logic_vector(to_unsigned(abs(v),16));
                r_m0_dir <= '0';
            end if;

            v := to_integer(signed(w_m1_pio_0_out));
            if v >= 0 then
                r_m1_duty <= std_logic_vector(to_unsigned(abs(v),16));
                r_m1_dir <= '0';
            elsif v < 0 then
                r_m1_duty <= std_logic_vector(to_unsigned(abs(v),16));
                r_m1_dir <= '1';
            end if;

        end if;
    end process;



    --led(8-1 downto 4) <= w_ledg_out(4-1 downto 0);



    inst_m0_pid_rv : component system
    port map (
        clk_clk                => FPGA_CLK1_50,
        reset_reset_n          => hps_fpga_reset_n,
		pio_0_in_port          => w_m0_pio_0_in,
        pio_0_out_port         => w_m0_pio_0_out,
		pio_1_in_port          => (others=>'0'),
        pio_1_out_port         => open,
		pio_2_in_port          => (others=>'0'),
        pio_2_out_port         => open,
		pio_3_in_port          => (others=>'0'),
        pio_3_out_port         => open,
		pio_4_in_port          => (others=>'0'),
        pio_4_out_port         => open,
		pio_5_in_port          => (others=>'0'),
        pio_5_out_port         => open
    );

    inst_m1_pid_rv : component system
    port map (
        clk_clk                => FPGA_CLK1_50,
        reset_reset_n          => hps_fpga_reset_n,
		pio_0_in_port          => w_m1_pio_0_in,
        pio_0_out_port         => w_m1_pio_0_out,
		pio_1_in_port          => (others=>'0'),
        pio_1_out_port         => open,
		pio_2_in_port          => (others=>'0'),
        pio_2_out_port         => open,
		pio_3_in_port          => (others=>'0'),
        pio_3_out_port         => open,
		pio_4_in_port          => (others=>'0'),
        pio_4_out_port         => open,
		pio_5_in_port          => (others=>'0'),
        pio_5_out_port         => open
    );




----=======================================================
----  Structural coding
----=======================================================
    inst_hps: hps_fpga 
    port map(
	    -- CLK & RESET
	    clk_clk                               =>  FPGA_CLK1_50 ,                               --                            clkclk
	    reset_reset_n                         =>  '1' ,                         --                          resetreset_n

	    -- FPGA PARTITION
	    button_pio_external_connection_export(2-1 downto 0) =>  KEY , -- button_pio_external_connectionexport
	    button_pio_external_connection_export(4-1 downto 2) =>  (others=>'0') , -- button_pio_external_connectionexport

	    dipsw_pio_external_connection_export  =>  SW ,  --  dipsw_pio_external_connectionexport
	    hps_0_h2f_reset_reset_n               =>  hps_fpga_reset_n ,               --                hps_0_h2f_resetreset_n
	    --led_pio_external_connection_export    =>  LED ,    --    led_pio_external_connectionexport

	    -- HPS ETHERNET
	    hps_0_hps_io_hps_io_emac1_inst_TX_CLK =>  HPS_ENET_GTX_CLK    ,   --                             hps_0_hps_iohps_io_emac1_inst_TX_CLK
	    hps_0_hps_io_hps_io_emac1_inst_TXD0   =>  HPS_ENET_TX_DATA(0) ,   --                             hps_io_emac1_inst_TXD0
	    hps_0_hps_io_hps_io_emac1_inst_TXD1   =>  HPS_ENET_TX_DATA(1) ,   --                             hps_io_emac1_inst_TXD1
	    hps_0_hps_io_hps_io_emac1_inst_TXD2   =>  HPS_ENET_TX_DATA(2),   --                             hps_io_emac1_inst_TXD2
	    hps_0_hps_io_hps_io_emac1_inst_TXD3   =>  HPS_ENET_TX_DATA(3),   --                             hps_io_emac1_inst_TXD3
	    hps_0_hps_io_hps_io_emac1_inst_RXD0   =>  HPS_ENET_RX_DATA(0),   --                             hps_io_emac1_inst_RXD0
	    hps_0_hps_io_hps_io_emac1_inst_MDIO   =>  HPS_ENET_MDIO       ,   --                             hps_io_emac1_inst_MDIO
	    hps_0_hps_io_hps_io_emac1_inst_MDC    =>  HPS_ENET_MDC        ,   --                             hps_io_emac1_inst_MDC
	    hps_0_hps_io_hps_io_emac1_inst_RX_CTL =>  HPS_ENET_RX_DV      ,   --                             hps_io_emac1_inst_RX_CTL
	    hps_0_hps_io_hps_io_emac1_inst_TX_CTL =>  HPS_ENET_TX_EN      ,   --                             hps_io_emac1_inst_TX_CTL
	    hps_0_hps_io_hps_io_emac1_inst_RX_CLK =>  HPS_ENET_RX_CLK     ,   --                             hps_io_emac1_inst_RX_CLK
	    hps_0_hps_io_hps_io_emac1_inst_RXD1   =>  HPS_ENET_RX_DATA(1),   --                             hps_io_emac1_inst_RXD1
	    hps_0_hps_io_hps_io_emac1_inst_RXD2   =>  HPS_ENET_RX_DATA(2),   --                             hps_io_emac1_inst_RXD2
	    hps_0_hps_io_hps_io_emac1_inst_RXD3   =>  HPS_ENET_RX_DATA(3),   --                             hps_io_emac1_inst_RXD3		  
	
	    -- HPS SD CARD
	    hps_0_hps_io_hps_io_sdio_inst_CMD     =>  HPS_SD_CMD     ,        --                               hps_io_sdio_inst_CMD
	    hps_0_hps_io_hps_io_sdio_inst_D0      =>  HPS_SD_DATA(0),        --                               hps_io_sdio_inst_D0
	    hps_0_hps_io_hps_io_sdio_inst_D1      =>  HPS_SD_DATA(1),        --                               hps_io_sdio_inst_D1
	    hps_0_hps_io_hps_io_sdio_inst_CLK     =>  HPS_SD_CLK     ,        --                               hps_io_sdio_inst_CLK
	    hps_0_hps_io_hps_io_sdio_inst_D2      =>  HPS_SD_DATA(2),        --                               hps_io_sdio_inst_D2
	    hps_0_hps_io_hps_io_sdio_inst_D3      =>  HPS_SD_DATA(3),        --                               hps_io_sdio_inst_D3
	
	    -- HPS USB
	    hps_0_hps_io_hps_io_usb1_inst_D0      =>  HPS_USB_DATA(0),      --                               hps_io_usb1_inst_D0
	    hps_0_hps_io_hps_io_usb1_inst_D1      =>  HPS_USB_DATA(1),      --                               hps_io_usb1_inst_D1
	    hps_0_hps_io_hps_io_usb1_inst_D2      =>  HPS_USB_DATA(2),      --                               hps_io_usb1_inst_D2
	    hps_0_hps_io_hps_io_usb1_inst_D3      =>  HPS_USB_DATA(3),      --                               hps_io_usb1_inst_D3
	    hps_0_hps_io_hps_io_usb1_inst_D4      =>  HPS_USB_DATA(4),      --                               hps_io_usb1_inst_D4
	    hps_0_hps_io_hps_io_usb1_inst_D5      =>  HPS_USB_DATA(5),      --                               hps_io_usb1_inst_D5
	    hps_0_hps_io_hps_io_usb1_inst_D6      =>  HPS_USB_DATA(6),      --                               hps_io_usb1_inst_D6
	    hps_0_hps_io_hps_io_usb1_inst_D7      =>  HPS_USB_DATA(7),      --                               hps_io_usb1_inst_D7
	    hps_0_hps_io_hps_io_usb1_inst_CLK     =>  HPS_USB_CLKOUT  ,      --                               hps_io_usb1_inst_CLK
	    hps_0_hps_io_hps_io_usb1_inst_STP     =>  HPS_USB_STP     ,      --                               hps_io_usb1_inst_STP
	    hps_0_hps_io_hps_io_usb1_inst_DIR     =>  HPS_USB_DIR     ,      --                               hps_io_usb1_inst_DIR
	    hps_0_hps_io_hps_io_usb1_inst_NXT     =>  HPS_USB_NXT     ,      --                               hps_io_usb1_inst_NXT
	      
	    -- HPS SPI
	    hps_0_hps_io_hps_io_spim1_inst_CLK    =>  HPS_SPIM_CLK  ,        --                               hps_io_spim1_inst_CLK
	    hps_0_hps_io_hps_io_spim1_inst_MOSI   =>  HPS_SPIM_MOSI ,        --                               hps_io_spim1_inst_MOSI
	    hps_0_hps_io_hps_io_spim1_inst_MISO   =>  HPS_SPIM_MISO ,        --                               hps_io_spim1_inst_MISO
	    hps_0_hps_io_hps_io_spim1_inst_SS0    =>  HPS_SPIM_SS   ,        --                               hps_io_spim1_inst_SS0
	
	    -- HPS UART
	    hps_0_hps_io_hps_io_uart0_inst_RX     =>  HPS_UART_RX ,     --                               hps_io_uart0_inst_RX
	    hps_0_hps_io_hps_io_uart0_inst_TX     =>  HPS_UART_TX ,     --                               hps_io_uart0_inst_TX
	
	    -- HPS I2C0
	    hps_0_hps_io_hps_io_i2c0_inst_SDA     =>  HPS_I2C0_SDAT ,     --                               hps_io_i2c0_inst_SDA
	    hps_0_hps_io_hps_io_i2c0_inst_SCL     =>  HPS_I2C0_SCLK ,     --                               hps_io_i2c0_inst_SCL
	
	    -- HPS I2C1
	    hps_0_hps_io_hps_io_i2c1_inst_SDA     =>  HPS_I2C1_SDAT ,     --                               hps_io_i2c1_inst_SDA
	    hps_0_hps_io_hps_io_i2c1_inst_SCL     =>  HPS_I2C1_SCLK ,     --                               hps_io_i2c1_inst_SCL
	
	    -- HPS DDR3
	    memory_mem_a                          =>  HPS_DDR3_ADDR ,                       --                memorymem_a
	    memory_mem_ba                         =>  HPS_DDR3_BA ,                         --                mem_ba
	    memory_mem_ck                         =>  HPS_DDR3_CK_P ,                       --                mem_ck
	    memory_mem_ck_n                       =>  HPS_DDR3_CK_N ,                       --                mem_ck_n
	    memory_mem_cke                        =>  HPS_DDR3_CKE ,                        --                mem_cke
	    memory_mem_cs_n                       =>  HPS_DDR3_CS_N ,                       --                mem_cs_n
	    memory_mem_ras_n                      =>  HPS_DDR3_RAS_N ,                      --                mem_ras_n
	    memory_mem_cas_n                      =>  HPS_DDR3_CAS_N ,                      --                mem_cas_n
	    memory_mem_we_n                       =>  HPS_DDR3_WE_N ,                       --                mem_we_n
	    memory_mem_reset_n                    =>  HPS_DDR3_RESET_N ,                    --                mem_reset_n
	    memory_mem_dq                         =>  HPS_DDR3_DQ ,                         --                mem_dq
	    memory_mem_dqs                        =>  HPS_DDR3_DQS_P ,                      --                mem_dqs
	    memory_mem_dqs_n                      =>  HPS_DDR3_DQS_N ,                      --                mem_dqs_n
	    memory_mem_odt                        =>  HPS_DDR3_ODT ,                        --                mem_odt
	    memory_mem_dm                         =>  HPS_DDR3_DM ,                         --                mem_dm
	    memory_oct_rzqin                      =>  HPS_DDR3_RZQ                          --                .oct_rzqin      
    );


end architecture;
