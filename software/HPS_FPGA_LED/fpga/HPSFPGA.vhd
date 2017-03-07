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
            clk_clk            : in  std_logic                      := 'X';             -- clk
            pio_data_in_value  : in  std_logic_vector(511 downto 0) := (others => 'X'); -- data_in_value
            pio_data_in_read   : out std_logic_vector(15 downto 0);                     -- data_in_read
            pio_data_out_value : out std_logic_vector(511 downto 0);                    -- data_out_value
            pio_data_out_write : out std_logic_vector(15 downto 0);                     -- data_out_write
            reset_reset_n      : in  std_logic                      := 'X'              -- reset_n
        );
    end component system;





	component hps_fpga is
		port (
            clk_clk                               : in    std_logic                       := 'X';             -- clk
            hps_arm_h2f_reset_reset_n               : out   std_logic;                                          -- reset_n
            hps_arm_hps_io_hps_io_emac1_inst_TX_CLK : out   std_logic;                                          -- hps_io_emac1_inst_TX_CLK
            hps_arm_hps_io_hps_io_emac1_inst_TXD0   : out   std_logic;                                          -- hps_io_emac1_inst_TXD0
            hps_arm_hps_io_hps_io_emac1_inst_TXD1   : out   std_logic;                                          -- hps_io_emac1_inst_TXD1
            hps_arm_hps_io_hps_io_emac1_inst_TXD2   : out   std_logic;                                          -- hps_io_emac1_inst_TXD2
            hps_arm_hps_io_hps_io_emac1_inst_TXD3   : out   std_logic;                                          -- hps_io_emac1_inst_TXD3
            hps_arm_hps_io_hps_io_emac1_inst_RXD0   : in    std_logic                       := 'X';             -- hps_io_emac1_inst_RXD0
            hps_arm_hps_io_hps_io_emac1_inst_MDIO   : inout std_logic                       := 'X';             -- hps_io_emac1_inst_MDIO
            hps_arm_hps_io_hps_io_emac1_inst_MDC    : out   std_logic;                                          -- hps_io_emac1_inst_MDC
            hps_arm_hps_io_hps_io_emac1_inst_RX_CTL : in    std_logic                       := 'X';             -- hps_io_emac1_inst_RX_CTL
            hps_arm_hps_io_hps_io_emac1_inst_TX_CTL : out   std_logic;                                          -- hps_io_emac1_inst_TX_CTL
            hps_arm_hps_io_hps_io_emac1_inst_RX_CLK : in    std_logic                       := 'X';             -- hps_io_emac1_inst_RX_CLK
            hps_arm_hps_io_hps_io_emac1_inst_RXD1   : in    std_logic                       := 'X';             -- hps_io_emac1_inst_RXD1
            hps_arm_hps_io_hps_io_emac1_inst_RXD2   : in    std_logic                       := 'X';             -- hps_io_emac1_inst_RXD2
            hps_arm_hps_io_hps_io_emac1_inst_RXD3   : in    std_logic                       := 'X';             -- hps_io_emac1_inst_RXD3
            hps_arm_hps_io_hps_io_sdio_inst_CMD     : inout std_logic                       := 'X';             -- hps_io_sdio_inst_CMD
            hps_arm_hps_io_hps_io_sdio_inst_D0      : inout std_logic                       := 'X';             -- hps_io_sdio_inst_D0
            hps_arm_hps_io_hps_io_sdio_inst_D1      : inout std_logic                       := 'X';             -- hps_io_sdio_inst_D1
            hps_arm_hps_io_hps_io_sdio_inst_CLK     : out   std_logic;                                          -- hps_io_sdio_inst_CLK
            hps_arm_hps_io_hps_io_sdio_inst_D2      : inout std_logic                       := 'X';             -- hps_io_sdio_inst_D2
            hps_arm_hps_io_hps_io_sdio_inst_D3      : inout std_logic                       := 'X';             -- hps_io_sdio_inst_D3
            hps_arm_hps_io_hps_io_usb1_inst_D0      : inout std_logic                       := 'X';             -- hps_io_usb1_inst_D0
            hps_arm_hps_io_hps_io_usb1_inst_D1      : inout std_logic                       := 'X';             -- hps_io_usb1_inst_D1
            hps_arm_hps_io_hps_io_usb1_inst_D2      : inout std_logic                       := 'X';             -- hps_io_usb1_inst_D2
            hps_arm_hps_io_hps_io_usb1_inst_D3      : inout std_logic                       := 'X';             -- hps_io_usb1_inst_D3
            hps_arm_hps_io_hps_io_usb1_inst_D4      : inout std_logic                       := 'X';             -- hps_io_usb1_inst_D4
            hps_arm_hps_io_hps_io_usb1_inst_D5      : inout std_logic                       := 'X';             -- hps_io_usb1_inst_D5
            hps_arm_hps_io_hps_io_usb1_inst_D6      : inout std_logic                       := 'X';             -- hps_io_usb1_inst_D6
            hps_arm_hps_io_hps_io_usb1_inst_D7      : inout std_logic                       := 'X';             -- hps_io_usb1_inst_D7
            hps_arm_hps_io_hps_io_usb1_inst_CLK     : in    std_logic                       := 'X';             -- hps_io_usb1_inst_CLK
            hps_arm_hps_io_hps_io_usb1_inst_STP     : out   std_logic;                                          -- hps_io_usb1_inst_STP
            hps_arm_hps_io_hps_io_usb1_inst_DIR     : in    std_logic                       := 'X';             -- hps_io_usb1_inst_DIR
            hps_arm_hps_io_hps_io_usb1_inst_NXT     : in    std_logic                       := 'X';             -- hps_io_usb1_inst_NXT
            hps_arm_hps_io_hps_io_spim1_inst_CLK    : out   std_logic;                                          -- hps_io_spim1_inst_CLK
            hps_arm_hps_io_hps_io_spim1_inst_MOSI   : out   std_logic;                                          -- hps_io_spim1_inst_MOSI
            hps_arm_hps_io_hps_io_spim1_inst_MISO   : in    std_logic                       := 'X';             -- hps_io_spim1_inst_MISO
            hps_arm_hps_io_hps_io_spim1_inst_SS0    : out   std_logic;                                          -- hps_io_spim1_inst_SS0
            hps_arm_hps_io_hps_io_uart0_inst_RX     : in    std_logic                       := 'X';             -- hps_io_uart0_inst_RX
            hps_arm_hps_io_hps_io_uart0_inst_TX     : out   std_logic;                                          -- hps_io_uart0_inst_TX
            hps_arm_hps_io_hps_io_i2c0_inst_SDA     : inout std_logic                       := 'X';             -- hps_io_i2c0_inst_SDA
            hps_arm_hps_io_hps_io_i2c0_inst_SCL     : inout std_logic                       := 'X';             -- hps_io_i2c0_inst_SCL
            hps_arm_hps_io_hps_io_i2c1_inst_SDA     : inout std_logic                       := 'X';             -- hps_io_i2c1_inst_SDA
            hps_arm_hps_io_hps_io_i2c1_inst_SCL     : inout std_logic                       := 'X';             -- hps_io_i2c1_inst_SCL
            memory_mem_a                          : out   std_logic_vector(14 downto 0);                      -- mem_a
            memory_mem_ba                         : out   std_logic_vector(2 downto 0);                       -- mem_ba
            memory_mem_ck                         : out   std_logic;                                          -- mem_ck
            memory_mem_ck_n                       : out   std_logic;                                          -- mem_ck_n
            memory_mem_cke                        : out   std_logic;                                          -- mem_cke
            memory_mem_cs_n                       : out   std_logic;                                          -- mem_cs_n
            memory_mem_ras_n                      : out   std_logic;                                          -- mem_ras_n
            memory_mem_cas_n                      : out   std_logic;                                          -- mem_cas_n
            memory_mem_we_n                       : out   std_logic;                                          -- mem_we_n
            memory_mem_reset_n                    : out   std_logic;                                          -- mem_reset_n
            memory_mem_dq                         : inout std_logic_vector(31 downto 0)   := (others => 'X'); -- mem_dq
            memory_mem_dqs                        : inout std_logic_vector(3 downto 0)    := (others => 'X'); -- mem_dqs
            memory_mem_dqs_n                      : inout std_logic_vector(3 downto 0)    := (others => 'X'); -- mem_dqs_n
            memory_mem_odt                        : out   std_logic;                                          -- mem_odt
            memory_mem_dm                         : out   std_logic_vector(3 downto 0);                       -- mem_dm
            memory_oct_rzqin                      : in    std_logic                       := 'X';             -- oct_rzqin
            reset_reset_n                         : in    std_logic                       := 'X';             -- reset_n
            pio_n_layer1_data_in_value            : in    std_logic_vector(2048-1 downto 0) := (others => 'X'); -- data_in_value
            pio_n_layer1_data_in_read             : out   std_logic_vector(64-1 downto 0);                     -- data_in_read
            pio_n_layer1_data_out_value           : out   std_logic_vector(2048-1 downto 0);                    -- data_out_value
            pio_n_layer1_data_out_write           : out   std_logic_vector(64-1 downto 0);                     -- data_out_write
            pio_n_layer2_data_in_value            : in    std_logic_vector(2048-1 downto 0) := (others => 'X'); -- data_in_value
            pio_n_layer2_data_in_read             : out   std_logic_vector(64-1 downto 0);                     -- data_in_read
            pio_n_layer2_data_out_value           : out   std_logic_vector(2048-1 downto 0);                    -- data_out_value
            pio_n_layer2_data_out_write           : out   std_logic_vector(64-1 downto 0);                     -- data_out_write
            pio_n_layer3_data_in_value            : in    std_logic_vector(2048-1 downto 0) := (others => 'X'); -- data_in_value
            pio_n_layer3_data_in_read             : out   std_logic_vector(64-1 downto 0);                     -- data_in_read
            pio_n_layer3_data_out_value           : out   std_logic_vector(2048-1 downto 0);                    -- data_out_value
            pio_n_layer3_data_out_write           : out   std_logic_vector(64-1 downto 0)                      -- data_out_write
        );
	end component hps_fpga;



----=======================================================
----  REG/WIRE declarations
----=======================================================
    signal hps_fpga_reset_n : std_logic;




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

    signal w_odometry_pio_in : std_logic_vector(32-1 downto 0);


    signal w_pio_n_layer1_data_in_value            : std_logic_vector(2048-1 downto 0) := (others => 'X'); -- data_in_value
    signal w_pio_n_layer1_data_in_read             : std_logic_vector(64-1 downto 0);                     -- data_in_read
    signal w_pio_n_layer1_data_out_value           : std_logic_vector(2048-1 downto 0);                    -- data_out_value
    signal w_pio_n_layer1_data_out_write           : std_logic_vector(64-1 downto 0);                     -- data_out_write
    signal w_pio_n_layer2_data_in_value            : std_logic_vector(2048-1 downto 0) := (others => 'X'); -- data_in_value
    signal w_pio_n_layer2_data_in_read             : std_logic_vector(64-1 downto 0);                     -- data_in_read
    signal w_pio_n_layer2_data_out_value           : std_logic_vector(2048-1 downto 0);                    -- data_out_value
    signal w_pio_n_layer2_data_out_write           : std_logic_vector(64-1 downto 0);                     -- data_out_write
    signal w_pio_n_layer3_data_in_value            : std_logic_vector(2048-1 downto 0) := (others => 'X'); -- data_in_value
    signal w_pio_n_layer3_data_in_read             : std_logic_vector(64-1 downto 0);                     -- data_in_read
    signal w_pio_n_layer3_data_out_value           : std_logic_vector(2048-1 downto 0);                    -- data_out_value
    signal w_pio_n_layer3_data_out_write           : std_logic_vector(64-1 downto 0);                      -- data_out_write

begin



    --led(8-1 downto 4) <= w_ledg_out(4-1 downto 0);


    inst_layer_1: entity work.robot_layer_1
    generic map (
        CLK_FREQUENCY_HZ => 50_000_000,
        RegCnt => 64
    )
    port map (

        clk     => FPGA_CLK1_50,
        reset   => not hps_fpga_reset_n,

        regs_data_in_value      => w_pio_n_layer1_data_in_value,
        regs_data_in_read       => w_pio_n_layer1_data_in_read,              
        regs_data_out_value     => w_pio_n_layer1_data_out_value,
        regs_data_out_write     => w_pio_n_layer1_data_out_write,

        ----------- ADC (//) ---------
        ad0_sclk => ad0_sclk,
        ad0_miso => ad0_miso,
        ad0_drdy => ad0_drdy,
        ad0_sync => ad0_sync,
        ad0_clk  => ad0_clk ,

        --------- ADC (muxed) --------
        ad1_sclk => ad1_sclk,
        ad1_mosi => ad1_mosi,
        ad1_miso => ad1_miso,
        ad1_ss   => ad1_ss,
        ad1_drdy => ad1_drdy,
        ad1_rst  => ad1_rst,

        ---------- H BRIDGE ----------
        m0_pwma  => m0_pwma,
        m0_pwmb  => m0_pwmb,
        m01_fault=> m01_fault,
        
        m1_pwma  => m1_pwma,
        m1_pwmb  => m1_pwmb,
        m01_resetn=> m01_resetn,

        m2_pwma  => m2_pwma,
        m2_pwmb  => m2_pwmb,

        m3_pwma  => m3_pwma,
        m3_pwmb  => m3_pwmb,

        m2345_fault=> m2345_fault,

        m4_pwma  => m4_pwma,
        m4_pwmb  => m4_pwmb,

        m5_pwma  => m5_pwma,
        m5_pwmb  => m5_pwmb,

        m2345_resetn=> m2345_resetn,

        ---------- QEI ----------    
        qei0_a   => qei0_a,
        qei0_b   => qei0_b,

        qei1_a   => qei1_a,
        qei1_b   => qei1_b,

        qei2_a   => qei2_a,
        qei2_b   => qei2_b,
        qei2_z   => qei2_z,

        qei3_a   => qei3_a,
        qei3_b   => qei3_b,
        qei3_z   => qei3_z,

        ---------- ESC ----------    
        esc0_pwm => esc0_pwm,
        esc0_dir => esc0_dir,

        esc1_pwm => esc1_pwm,
        esc1_dir => esc1_dir,

        ------- PWM (Servos) ------
        s => s,

        --------- IOs ----------
        io_0 => io_0,
        io_1 => io_1,
        io_2 => io_2,
        io_3 => io_3,
        io_4 => io_4,
        io_5 => io_5,
        io_6 => io_6,
        io_7 => io_7,

        --------- UART ----------
        uart0_rx     => uart0_rx,
        uart0_tx     => uart0_tx,

        uart1_rx     => uart1_rx,
        uart1_tx     => uart1_tx,

        uart2_rx     => uart2_rx,
        uart2_tx     => uart2_tx,
        uart2_custom => uart2_custom,

        uart3_rx     => uart3_rx,
        uart3_tx     => uart3_tx,
        uart3_custom => uart3_custom,

        --------- I2C ----------
        i2c0_scl     => i2c0_scl,
        i2c0_sda     => i2c0_sda,
        i2c0_reset   => i2c0_reset,

        i2c1_scl     => i2c1_scl,
        i2c1_sda     => i2c1_sda,
        i2c1_reset   => i2c1_reset,

        --------- SPI ----------
        spi0_sclk    => spi0_sclk,
        spi0_mosi    => spi0_mosi,
        spi0_miso    => spi0_miso,
        spi0_ss      => spi0_ss,

        spi1_sclk    => spi1_sclk,
        spi1_mosi    => spi1_mosi,
        spi1_miso    => spi1_miso,
        spi1_ss      => spi1_ss,

        --! Use SPI1
        imu_ss       => imu_ss,
        imu_drdy     => imu_drdy,
        imu_fsync    => imu_fsync,

        ---------- LED -----------
        led_green => led_green,
        led_red   => led_red,

        --------- MGMT -----------
        lv_mux    => lv_mux,
        buzzer    => buzzer,

	    ----------/ NANO SOC LED --------/
	    LED                 => LED,

	    ----------/ NANO SOC SW --------/
	    SW                  => SW


    );


--    inst_m0_pid_rv : component system
--    port map (
--        clk_clk                 => FPGA_CLK1_50,
--        reset_reset_n           => hps_fpga_reset_n,
--        pio_data_in_value(32-1 downto 0) => w_m0_pio_0_in,  --   pio.data_in_value
--        pio_data_in_read        => open,   --      .data_in_read
--        pio_data_out_value(32-1 downto 0)  => w_m0_pio_0_out, --      .data_out_value
--        pio_data_out_write      => open --      .data_out_write
--    );

--    inst_m1_pid_rv : component system
--    port map (
--        clk_clk                 => FPGA_CLK1_50,
--        reset_reset_n           => hps_fpga_reset_n,
--        pio_data_in_value(32-1 downto 0) => w_m1_pio_0_in,  --   pio.data_in_value
--        pio_data_in_read        => open,   --      .data_in_read
--        pio_data_out_value(32-1 downto 0)  => w_m1_pio_0_out, --      .data_out_value
--        pio_data_out_write      => open --      .data_out_write
--    );


    --w_odometry_pio_in <= w_qei1_cnt & w_qei0_cnt;

--    inst_odometry_rv : component system
--    port map (
--        clk_clk                => FPGA_CLK1_50,
--        reset_reset_n          => hps_fpga_reset_n,
--        pio_data_in_value(32-1 downto 0) => w_odometry_pio_in,  --   pio.data_in_value
--        pio_data_in_read        => open,   --      .data_in_read
--        pio_data_out_value      => open, --      .data_out_value
--        pio_data_out_write      => open --      .data_out_write
--    );


    --w_pio_n_layer1_data_in_value <= w_pio_n_layer1_data_out_value;
    w_pio_n_layer2_data_in_value <= not w_pio_n_layer2_data_out_value;
    w_pio_n_layer3_data_in_value <= w_pio_n_layer3_data_out_value;
----=======================================================
----  Structural coding
----=======================================================
    inst_hps: hps_fpga 
    port map(
	    -- CLK & RESET
	    clk_clk                               =>  FPGA_CLK1_50 ,                               --                            clkclk
	    reset_reset_n                         =>  '1' ,                         --                          resetreset_n

            pio_n_layer1_data_in_value            => w_pio_n_layer1_data_in_value,            --    pio_n_layer1.data_in_value
            pio_n_layer1_data_in_read             => w_pio_n_layer1_data_in_read,             --                .data_in_read
            pio_n_layer1_data_out_value           => w_pio_n_layer1_data_out_value,           --                .data_out_value
            pio_n_layer1_data_out_write           => w_pio_n_layer1_data_out_write,           --                .data_out_write
            pio_n_layer2_data_in_value            => w_pio_n_layer2_data_in_value,            --    pio_n_layer2.data_in_value
            pio_n_layer2_data_in_read             => w_pio_n_layer2_data_in_read,             --                .data_in_read
            pio_n_layer2_data_out_value           => w_pio_n_layer2_data_out_value,           --                .data_out_value
            pio_n_layer2_data_out_write           => w_pio_n_layer2_data_out_write,           --                .data_out_write
            pio_n_layer3_data_in_value            => w_pio_n_layer3_data_in_value,            --    pio_n_layer3.data_in_value
            pio_n_layer3_data_in_read             => w_pio_n_layer3_data_in_read,             --                .data_in_read
            pio_n_layer3_data_out_value           => w_pio_n_layer3_data_out_value,           --                .data_out_value
            pio_n_layer3_data_out_write           => w_pio_n_layer3_data_out_write,            --                .data_out_write



	    -- HPS ETHERNET
	    hps_arm_hps_io_hps_io_emac1_inst_TX_CLK =>  HPS_ENET_GTX_CLK    ,   --                             hps_arm_hps_iohps_io_emac1_inst_TX_CLK
	    hps_arm_hps_io_hps_io_emac1_inst_TXD0   =>  HPS_ENET_TX_DATA(0) ,   --                             hps_io_emac1_inst_TXD0
	    hps_arm_hps_io_hps_io_emac1_inst_TXD1   =>  HPS_ENET_TX_DATA(1) ,   --                             hps_io_emac1_inst_TXD1
	    hps_arm_hps_io_hps_io_emac1_inst_TXD2   =>  HPS_ENET_TX_DATA(2),   --                             hps_io_emac1_inst_TXD2
	    hps_arm_hps_io_hps_io_emac1_inst_TXD3   =>  HPS_ENET_TX_DATA(3),   --                             hps_io_emac1_inst_TXD3
	    hps_arm_hps_io_hps_io_emac1_inst_RXD0   =>  HPS_ENET_RX_DATA(0),   --                             hps_io_emac1_inst_RXD0
	    hps_arm_hps_io_hps_io_emac1_inst_MDIO   =>  HPS_ENET_MDIO       ,   --                             hps_io_emac1_inst_MDIO
	    hps_arm_hps_io_hps_io_emac1_inst_MDC    =>  HPS_ENET_MDC        ,   --                             hps_io_emac1_inst_MDC
	    hps_arm_hps_io_hps_io_emac1_inst_RX_CTL =>  HPS_ENET_RX_DV      ,   --                             hps_io_emac1_inst_RX_CTL
	    hps_arm_hps_io_hps_io_emac1_inst_TX_CTL =>  HPS_ENET_TX_EN      ,   --                             hps_io_emac1_inst_TX_CTL
	    hps_arm_hps_io_hps_io_emac1_inst_RX_CLK =>  HPS_ENET_RX_CLK     ,   --                             hps_io_emac1_inst_RX_CLK
	    hps_arm_hps_io_hps_io_emac1_inst_RXD1   =>  HPS_ENET_RX_DATA(1),   --                             hps_io_emac1_inst_RXD1
	    hps_arm_hps_io_hps_io_emac1_inst_RXD2   =>  HPS_ENET_RX_DATA(2),   --                             hps_io_emac1_inst_RXD2
	    hps_arm_hps_io_hps_io_emac1_inst_RXD3   =>  HPS_ENET_RX_DATA(3),   --                             hps_io_emac1_inst_RXD3		  
	
	    -- HPS SD CARD
	    hps_arm_hps_io_hps_io_sdio_inst_CMD     =>  HPS_SD_CMD     ,        --                               hps_io_sdio_inst_CMD
	    hps_arm_hps_io_hps_io_sdio_inst_D0      =>  HPS_SD_DATA(0),        --                               hps_io_sdio_inst_D0
	    hps_arm_hps_io_hps_io_sdio_inst_D1      =>  HPS_SD_DATA(1),        --                               hps_io_sdio_inst_D1
	    hps_arm_hps_io_hps_io_sdio_inst_CLK     =>  HPS_SD_CLK     ,        --                               hps_io_sdio_inst_CLK
	    hps_arm_hps_io_hps_io_sdio_inst_D2      =>  HPS_SD_DATA(2),        --                               hps_io_sdio_inst_D2
	    hps_arm_hps_io_hps_io_sdio_inst_D3      =>  HPS_SD_DATA(3),        --                               hps_io_sdio_inst_D3
	
	    -- HPS USB
	    hps_arm_hps_io_hps_io_usb1_inst_D0      =>  HPS_USB_DATA(0),      --                               hps_io_usb1_inst_D0
	    hps_arm_hps_io_hps_io_usb1_inst_D1      =>  HPS_USB_DATA(1),      --                               hps_io_usb1_inst_D1
	    hps_arm_hps_io_hps_io_usb1_inst_D2      =>  HPS_USB_DATA(2),      --                               hps_io_usb1_inst_D2
	    hps_arm_hps_io_hps_io_usb1_inst_D3      =>  HPS_USB_DATA(3),      --                               hps_io_usb1_inst_D3
	    hps_arm_hps_io_hps_io_usb1_inst_D4      =>  HPS_USB_DATA(4),      --                               hps_io_usb1_inst_D4
	    hps_arm_hps_io_hps_io_usb1_inst_D5      =>  HPS_USB_DATA(5),      --                               hps_io_usb1_inst_D5
	    hps_arm_hps_io_hps_io_usb1_inst_D6      =>  HPS_USB_DATA(6),      --                               hps_io_usb1_inst_D6
	    hps_arm_hps_io_hps_io_usb1_inst_D7      =>  HPS_USB_DATA(7),      --                               hps_io_usb1_inst_D7
	    hps_arm_hps_io_hps_io_usb1_inst_CLK     =>  HPS_USB_CLKOUT  ,      --                               hps_io_usb1_inst_CLK
	    hps_arm_hps_io_hps_io_usb1_inst_STP     =>  HPS_USB_STP     ,      --                               hps_io_usb1_inst_STP
	    hps_arm_hps_io_hps_io_usb1_inst_DIR     =>  HPS_USB_DIR     ,      --                               hps_io_usb1_inst_DIR
	    hps_arm_hps_io_hps_io_usb1_inst_NXT     =>  HPS_USB_NXT     ,      --                               hps_io_usb1_inst_NXT
	      
	    -- HPS SPI
	    hps_arm_hps_io_hps_io_spim1_inst_CLK    =>  HPS_SPIM_CLK  ,        --                               hps_io_spim1_inst_CLK
	    hps_arm_hps_io_hps_io_spim1_inst_MOSI   =>  HPS_SPIM_MOSI ,        --                               hps_io_spim1_inst_MOSI
	    hps_arm_hps_io_hps_io_spim1_inst_MISO   =>  HPS_SPIM_MISO ,        --                               hps_io_spim1_inst_MISO
	    hps_arm_hps_io_hps_io_spim1_inst_SS0    =>  HPS_SPIM_SS   ,        --                               hps_io_spim1_inst_SS0
	
	    -- HPS UART
	    hps_arm_hps_io_hps_io_uart0_inst_RX     =>  HPS_UART_RX ,     --                               hps_io_uart0_inst_RX
	    hps_arm_hps_io_hps_io_uart0_inst_TX     =>  HPS_UART_TX ,     --                               hps_io_uart0_inst_TX
	
	    -- HPS I2C0
	    hps_arm_hps_io_hps_io_i2c0_inst_SDA     =>  HPS_I2C0_SDAT ,     --                               hps_io_i2c0_inst_SDA
	    hps_arm_hps_io_hps_io_i2c0_inst_SCL     =>  HPS_I2C0_SCLK ,     --                               hps_io_i2c0_inst_SCL
	
	    -- HPS I2C1
	    hps_arm_hps_io_hps_io_i2c1_inst_SDA     =>  HPS_I2C1_SDAT ,     --                               hps_io_i2c1_inst_SDA
	    hps_arm_hps_io_hps_io_i2c1_inst_SCL     =>  HPS_I2C1_SCLK ,     --                               hps_io_i2c1_inst_SCL
	
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
