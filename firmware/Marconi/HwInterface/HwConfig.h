//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Holds all settings for compile-time configuration of hardware and firmware 
// settings (e.g. clock dividers, wait states, buffer sizes, etc.)
//
//******************************************************************************

#ifndef __HWCONFIG_H__
#define __HWCONFIG_H__

//******************************************************************************
//  D E F I N E S
//******************************************************************************

// DSP clock settings (everything is derived from HW_DSP_CLKIN_MHZ, which
// corresponds to the DSP's external crystal, and the appropriate divider
// settings)
#define HW_DSP_CLKIN_MHZ        27
#define HW_DSP_CLKIN_HZ         (HW_DSP_CLKIN_MHZ * 1000000UL) 
#define HW_DSP_CLKIN_MULT       5
#define HW_DSP_SYSCLK_HZ        (HW_DSP_CLKIN_HZ * HW_DSP_CLKIN_MULT)
#define HW_DSP_HSPCLK_DIV       2 
#define HW_DSP_LSPCLK_DIV       4
#define HW_DSP_HSPCLK_HZ        (HW_DSP_SYSCLK_HZ / HW_DSP_HSPCLK_DIV)
#define HW_DSP_LSPCLK_HZ        (HW_DSP_SYSCLK_HZ / HW_DSP_LSPCLK_DIV)

// Enforce safety checks on clock settings
#if(HW_DSP_CLKIN_MULT < 1 || HW_DSP_CLKIN_MULT > 5)
    #error HW_DSP_CLKIN_MULT must be between 1 and 5
#endif
#if(HW_DSP_HSPCLK_DIV % 2 != 0 || HW_DSP_HSHPCLK_DIV > 14 || HW_DSP_HSPCLK_DIV < 2)
    #error HW_DSP_HSPCLK_DIV must be a multiple of 2 between 2 and 14
#endif
#if(HW_DSP_LSPCLK_DIV % 2 != 0 || HW_DSP_LSHPCLK_DIV > 14 || HW_DSP_LSPCLK_DIV < 2)
    #error HW_DSP_LSPCLK_DIV must be a multiple of 2 between 2 and 14
#endif

// Audio codec clock settings (derived from HW_CODEC_CLK_KHZ, which corresponds
// to the clock input of the codec)
#define HW_CODEC_CLK_KHZ        8192
#define HW_CODEC_CLK_HZ         (HW_CODEC_CLK_KHZ * 1000UL)
#define HW_CODEC_CLK_DIVIDE     256

// DSP Flash wait states (valid up to 135 MHz, see TMS320F2811 Datasheet)
#define HW_FLASH_PAGE_WAIT      4
#define HW_FLASH_RANDOM_WAIT    4
#define HW_FLASH_OTP_WAIT       8
// These are "magic" numbers takes from TI app note SPRA958D.PDF:
#define HW_FLASH_STANDBY_WAIT   0x01FF; 
#define HW_FLASH_ACTIVE_WAIT    0x01FF; 

// Buffer (FIFO) sizes for both UARTs (plus one)
#define HW_UART_A_RX_FIFO_LEN   (128 + 1)
#define HW_UART_A_TX_FIFO_LEN   (128 + 1)
#define HW_UART_B_RX_FIFO_LEN   (128 + 1)
#define HW_UART_B_TX_FIFO_LEN   (128 + 1)

// Buffer (FIFO) sizes for codec audio samples and commands (plus one)
#define HW_CODEC_RX_FIFO_LEN    (256 + 1)
#define HW_CODEC_TX_FIFO_LEN    (256 + 1)
#define HW_CODEC_CMD_FIFO_LEN   (16 + 1)

#endif
