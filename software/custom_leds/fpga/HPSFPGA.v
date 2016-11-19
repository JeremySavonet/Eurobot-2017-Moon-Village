// =================================================
//
// ================ HPS FPGA    ====================
//
// =================================================

`define ENABLE_HPS

module HPSFPGA(

    ///////// ADC /////////
    output             ADC_CONVST,
    output             ADC_SCK,
    output             ADC_SDI,
    input              ADC_SDO,

    ///////// ARDUINO /////////
    inout       [15:0] ARDUINO_IO,
    inout              ARDUINO_RESET_N,

`ifdef ENABLE_CLK
    ///////// CLK /////////
    output             CLK_I2C_SCL,
    inout              CLK_I2C_SDA,
`endif //ENABLE_CLK

    ///////// FPGA /////////
    input              FPGA_CLK1_50,
    input              FPGA_CLK2_50,
    input              FPGA_CLK3_50,

    ///////// GPIO /////////
    inout       [35:0] GPIO_0,
    inout       [35:0] GPIO_1,

`ifdef ENABLE_HPS
    ///////// HPS /////////
    inout              HPS_CONV_USB_N,
    output      [14:0] HPS_DDR3_ADDR,
    output      [2:0]  HPS_DDR3_BA,
    output             HPS_DDR3_CAS_N,
    output             HPS_DDR3_CKE,
    output             HPS_DDR3_CK_N,
    output             HPS_DDR3_CK_P,
    output             HPS_DDR3_CS_N,
    output      [3:0]  HPS_DDR3_DM,
    inout       [31:0] HPS_DDR3_DQ,
    inout       [3:0]  HPS_DDR3_DQS_N,
    inout       [3:0]  HPS_DDR3_DQS_P,
    output             HPS_DDR3_ODT,
    output             HPS_DDR3_RAS_N,
    output             HPS_DDR3_RESET_N,
    input              HPS_DDR3_RZQ,
    output             HPS_DDR3_WE_N,
    output             HPS_ENET_GTX_CLK,
    inout              HPS_ENET_INT_N,
    output             HPS_ENET_MDC,
    inout              HPS_ENET_MDIO,
    input              HPS_ENET_RX_CLK,
    input       [3:0]  HPS_ENET_RX_DATA,
    input              HPS_ENET_RX_DV,
    output      [3:0]  HPS_ENET_TX_DATA,
    output             HPS_ENET_TX_EN,
    inout              HPS_GSENSOR_INT,
    inout              HPS_I2C0_SCLK,
    inout              HPS_I2C0_SDAT,
    inout              HPS_I2C1_SCLK,
    inout              HPS_I2C1_SDAT,
    inout              HPS_KEY,
    inout              HPS_LED,
    inout              HPS_LTC_GPIO,
    output             HPS_SD_CLK,
    inout              HPS_SD_CMD,
    inout       [3:0]  HPS_SD_DATA,
    output             HPS_SPIM_CLK,
    input              HPS_SPIM_MISO,
    output             HPS_SPIM_MOSI,
    inout              HPS_SPIM_SS,
    input              HPS_UART_RX,
    output             HPS_UART_TX,
    input              HPS_USB_CLKOUT,
    inout       [7:0]  HPS_USB_DATA,
    input              HPS_USB_DIR,
    input              HPS_USB_NXT,
    output             HPS_USB_STP,
`endif // ENABLE_HPS

    ///////// KEY /////////
    input       [1:0]  KEY,

    ///////// LED /////////
    output      [7:0]  LED,

    ///////// SW /////////
    input       [3:0]  SW
);


//=======================================================
//  REG/WIRE declarations
//=======================================================
    wire hps_fpga_reset_n;

//=======================================================
//  Structural coding
//=======================================================

soc_system u0 (
    //Clock & Reset
    .clk_clk                               ( FPGA_CLK1_50        ),
    .reset_reset_n                         ( 1'b1                ),

    //HPS ddr3
    .memory_mem_a                          ( HPS_DDR3_ADDR       ),
    .memory_mem_ba                         ( HPS_DDR3_BA         ),
    .memory_mem_ck                         ( HPS_DDR3_CK_P       ),
    .memory_mem_ck_n                       ( HPS_DDR3_CK_N       ),
    .memory_mem_cke                        ( HPS_DDR3_CKE        ),
    .memory_mem_cs_n                       ( HPS_DDR3_CS_N       ),
    .memory_mem_ras_n                      ( HPS_DDR3_RAS_N      ),
    .memory_mem_cas_n                      ( HPS_DDR3_CAS_N      ),
    .memory_mem_we_n                       ( HPS_DDR3_WE_N       ),
    .memory_mem_reset_n                    ( HPS_DDR3_RESET_N    ),
    .memory_mem_dq                         ( HPS_DDR3_DQ         ),
    .memory_mem_dqs                        ( HPS_DDR3_DQS_P      ),
    .memory_mem_dqs_n                      ( HPS_DDR3_DQS_N      ),
    .memory_mem_odt                        ( HPS_DDR3_ODT        ),
    .memory_mem_dm                         ( HPS_DDR3_DM         ),
    .memory_oct_rzqin                      ( HPS_DDR3_RZQ        ),

    //HPS ethernet
    .hps_0_hps_io_hps_io_emac1_inst_TX_CLK ( HPS_ENET_GTX_CLK    ),
    .hps_0_hps_io_hps_io_emac1_inst_TXD0   ( HPS_ENET_TX_DATA[0] ),
    .hps_0_hps_io_hps_io_emac1_inst_TXD1   ( HPS_ENET_TX_DATA[1] ),
    .hps_0_hps_io_hps_io_emac1_inst_TXD2   ( HPS_ENET_TX_DATA[2] ),
    .hps_0_hps_io_hps_io_emac1_inst_TXD3   ( HPS_ENET_TX_DATA[3] ),
    .hps_0_hps_io_hps_io_emac1_inst_RXD0   ( HPS_ENET_RX_DATA[0] ),
    .hps_0_hps_io_hps_io_emac1_inst_MDIO   ( HPS_ENET_MDIO       ),
    .hps_0_hps_io_hps_io_emac1_inst_MDC    ( HPS_ENET_MDC        ),
    .hps_0_hps_io_hps_io_emac1_inst_RX_CTL ( HPS_ENET_RX_DV      ),
    .hps_0_hps_io_hps_io_emac1_inst_TX_CTL ( HPS_ENET_TX_EN      ),
    .hps_0_hps_io_hps_io_emac1_inst_RX_CLK ( HPS_ENET_RX_CLK     ),
    .hps_0_hps_io_hps_io_emac1_inst_RXD1   ( HPS_ENET_RX_DATA[1] ),
    .hps_0_hps_io_hps_io_emac1_inst_RXD2   ( HPS_ENET_RX_DATA[2] ),
    .hps_0_hps_io_hps_io_emac1_inst_RXD3   ( HPS_ENET_RX_DATA[3] ),

    //HPS SD card
    .hps_0_hps_io_hps_io_sdio_inst_CMD     ( HPS_SD_CMD          ),
    .hps_0_hps_io_hps_io_sdio_inst_D0      ( HPS_SD_DATA[0]      ),
    .hps_0_hps_io_hps_io_sdio_inst_D1      ( HPS_SD_DATA[1]      ),
    .hps_0_hps_io_hps_io_sdio_inst_CLK     ( HPS_SD_CLK          ),
    .hps_0_hps_io_hps_io_sdio_inst_D2      ( HPS_SD_DATA[2]      ),
    .hps_0_hps_io_hps_io_sdio_inst_D3      ( HPS_SD_DATA[3]      ),

    //HPS USB
    .hps_0_hps_io_hps_io_usb1_inst_D0      ( HPS_USB_DATA[0]     ),
    .hps_0_hps_io_hps_io_usb1_inst_D1      ( HPS_USB_DATA[1]     ),
    .hps_0_hps_io_hps_io_usb1_inst_D2      ( HPS_USB_DATA[2]     ),
    .hps_0_hps_io_hps_io_usb1_inst_D3      ( HPS_USB_DATA[3]     ),
    .hps_0_hps_io_hps_io_usb1_inst_D4      ( HPS_USB_DATA[4]     ),
    .hps_0_hps_io_hps_io_usb1_inst_D5      ( HPS_USB_DATA[5]     ),
    .hps_0_hps_io_hps_io_usb1_inst_D6      ( HPS_USB_DATA[6]     ),
    .hps_0_hps_io_hps_io_usb1_inst_D7      ( HPS_USB_DATA[7]     ),
    .hps_0_hps_io_hps_io_usb1_inst_CLK     ( HPS_USB_CLKOUT      ),
    .hps_0_hps_io_hps_io_usb1_inst_STP     ( HPS_USB_STP         ),
    .hps_0_hps_io_hps_io_usb1_inst_DIR     ( HPS_USB_DIR         ),
    .hps_0_hps_io_hps_io_usb1_inst_NXT     ( HPS_USB_NXT         ),

    //HPS SPI
    .hps_0_hps_io_hps_io_spim1_inst_CLK    ( HPS_SPIM_CLK        ),
    .hps_0_hps_io_hps_io_spim1_inst_MOSI   ( HPS_SPIM_MOSI       ),
    .hps_0_hps_io_hps_io_spim1_inst_MISO   ( HPS_SPIM_MISO       ),
    .hps_0_hps_io_hps_io_spim1_inst_SS0    ( HPS_SPIM_SS         ),

    //HPS UART
    .hps_0_hps_io_hps_io_uart0_inst_RX     ( HPS_UART_RX         ),
    .hps_0_hps_io_hps_io_uart0_inst_TX     ( HPS_UART_TX         ),

    //HPS I2C1
    .hps_0_hps_io_hps_io_i2c0_inst_SDA     ( HPS_I2C0_SDAT       ),
    .hps_0_hps_io_hps_io_i2c0_inst_SCL     ( HPS_I2C0_SCLK       ),

    //HPS I2C2
    .hps_0_hps_io_hps_io_i2c1_inst_SDA     ( HPS_I2C1_SDAT       ),
    .hps_0_hps_io_hps_io_i2c1_inst_SCL     ( HPS_I2C1_SCLK       ),

    //FPGA Partion
    .custom_leds_0_leds_leds               ( LED                 ),
    .dipsw_pio_external_connection_export  ( SW                  ),
    .button_pio_external_connection_export ( KEY                 ),
    .hps_0_h2f_reset_reset_n               ( hps_fpga_reset_n    )
 );

 endmodule
 