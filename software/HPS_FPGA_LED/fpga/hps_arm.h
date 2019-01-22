#ifndef _ALTERA_HPS_ARM_H_
#define _ALTERA_HPS_ARM_H_

/*
 * This file was automatically generated by the swinfo2header utility.
 * 
 * Created from SOPC Builder system 'hps_fpga' in
 * file '/home/laptop/robot/Eurobot-2017-Moon-Village/software/HPS_FPGA_LED/fpga/hps_fpga.sopcinfo'.
 */

/*
 * This file contains macros for module 'hps_arm' and devices
 * connected to the following master:
 *   h2f_lw_axi_master
 * 
 * Do not include this header file and another header file created for a
 * different module or master group at the same time.
 * Doing so may result in duplicate macro names.
 * Instead, use the system header file which has macros with unique names.
 */

/*
 * Macros for device 'pio_n_layer1', class 'pio_n'
 * The macros are prefixed with 'PIO_N_LAYER1_'.
 * The prefix is the slave descriptor.
 */
#define PIO_N_LAYER1_COMPONENT_TYPE pio_n
#define PIO_N_LAYER1_COMPONENT_NAME pio_n_layer1
#define PIO_N_LAYER1_BASE 0x0
#define PIO_N_LAYER1_SPAN 1024
#define PIO_N_LAYER1_END 0x3ff

/*
 * Macros for device 'pio_n_layer2', class 'pio_n'
 * The macros are prefixed with 'PIO_N_LAYER2_'.
 * The prefix is the slave descriptor.
 */
#define PIO_N_LAYER2_COMPONENT_TYPE pio_n
#define PIO_N_LAYER2_COMPONENT_NAME pio_n_layer2
#define PIO_N_LAYER2_BASE 0x400
#define PIO_N_LAYER2_SPAN 1024
#define PIO_N_LAYER2_END 0x7ff

/*
 * Macros for device 'pio_n_layer3', class 'pio_n'
 * The macros are prefixed with 'PIO_N_LAYER3_'.
 * The prefix is the slave descriptor.
 */
#define PIO_N_LAYER3_COMPONENT_TYPE pio_n
#define PIO_N_LAYER3_COMPONENT_NAME pio_n_layer3
#define PIO_N_LAYER3_BASE 0x800
#define PIO_N_LAYER3_SPAN 1024
#define PIO_N_LAYER3_END 0xbff

/*
 * Macros for device 'sysid_qsys', class 'altera_avalon_sysid_qsys'
 * The macros are prefixed with 'SYSID_QSYS_'.
 * The prefix is the slave descriptor.
 */
#define SYSID_QSYS_COMPONENT_TYPE altera_avalon_sysid_qsys
#define SYSID_QSYS_COMPONENT_NAME sysid_qsys
#define SYSID_QSYS_BASE 0x10000
#define SYSID_QSYS_SPAN 8
#define SYSID_QSYS_END 0x10007
#define SYSID_QSYS_ID 2899645186
#define SYSID_QSYS_TIMESTAMP 1548076927


#endif /* _ALTERA_HPS_ARM_H_ */
