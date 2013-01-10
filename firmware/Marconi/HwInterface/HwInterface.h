//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:		Enter Description here
//
//
//
//******************************************************************************

#ifndef __HWINTERFACE_H__
	#define __HWINTERFACE_H__

#include "GlobalDefs.h"

//******************************************************************************
//  D E F I N E S
//******************************************************************************


//******************************************************************************
//  E N U M S
//******************************************************************************


//******************************************************************************
//  G L O B A L    D E F I N I T I O N S
//******************************************************************************


typedef enum
{
	HW_UART_ID_A,
	HW_UART_ID_B,
	NUM_HW_UART_ID
} HW_UART_ID_TYPE;


typedef enum
{
    HW_UART_PARITY_NONE,
    HW_UART_PARITY_EVEN,
    HW_UART_PARITY_ODD,
    NUM_HW_UART_PARITY
} HW_UART_PARITY_TYPE;


typedef enum
{
	HW_GPIO_ID_A,
	HW_GPIO_ID_B,
	NUM_HW_GPIO_ID
} HW_GPIO_ID_TYPE;


typedef enum {
    HW_CODEC_GAIN_0_DB,
    HW_CODEC_GAIN_MINUS_36_DB,
    HW_CODEC_GAIN_MINUS_30_DB,
    HW_CODEC_GAIN_MINUS_24_DB,
    HW_CODEC_GAIN_MINUS_18_DB,
    HW_CODEC_GAIN_MINUS_12_DB,
    HW_CODEC_GAIN_MINUS_9_DB,
    HW_CODEC_GAIN_MINUS_6_DB,
    HW_CODEC_GAIN_MINUS_3_DB,
    HW_CODEC_GAIN_PLUS_3_DB,
    HW_CODEC_GAIN_PLUS_6_DB,
    HW_CODEC_GAIN_PLUS_9_DB,
    HW_CODEC_GAIN_PLUS_12_DB,
    HW_CODEC_GAIN_PLUS_18_DB,
    HW_CODEC_GAIN_PLUS_24_DB,
    HW_CODEC_GAIN_MUTE,
    NUM_HW_CODEC_GAIN
} HW_CODEC_GAIN_TYPE;


typedef enum {
    HW_CODEC_SAMPLE_RATE_4_KHZ,
    HW_CODEC_SAMPLE_RATE_8_KHZ,
    HW_CODEC_SAMPLE_RATE_16_KHZ,
    NUM_HW_CODEC_SAMPLE_RATE
} HW_CODEC_SAMPLE_RATE_TYPE;


typedef enum {
    HW_CODEC_RX_CHANNEL_1,
    HW_CODEC_RX_CHANNEL_2,
    NUM_HW_CODEC_RX_CHANNEL
} HW_CODEC_RX_CHANNEL_TYPE;


//******************************************************************************
//  E X T E R N A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

//******************************************************************************
//
// FUNCTION:    HwInit - Initializes DSP hardware (PLL, flash wait states, 
//                  clock enables, etc.) 
//
// USAGE:       Must be called at startup before any hardware peripherals 
//              are accessed.
//				
//
// INPUT:       None
//
// OUTPUT:      None
//
//******************************************************************************
void HwInit(void);


//******************************************************************************
//
// FUNCTION:    HwUartOpen - opens the specified hardware UART with a 
//              specific baud rate and parity setting.
//
// USAGE:       Must be called before any other calls are made for the 
//              specified UART. After closing with HwUartClose(...), a
//              UART may be reopened with different settings.
//
// INPUT:       uartId - one of HW_UART_ID_A or HW_UART_ID_B
//              rate - the baud rate in bits / second
//              parity - one of HW_UART_PARITY_NONE, HW_UART_PARITY_EVEN,
//                  or HW_UART_PARITY_ODD
//
// OUTPUT:      none 
//
//******************************************************************************
void HwUartOpen(HW_UART_ID_TYPE uartId, DWORD baud, HW_UART_PARITY_TYPE parity);


//******************************************************************************
//
// FUNCTION:    HwUartClose - closes the specified hardware UART.
//
// INPUT:       uartId - one of HW_UART_ID_A or HW_UART_ID_B
//
// USAGE:       Call when a UART is no longer in use or before
//              reconfiguring.
//
// OUTPUT:      none 
//
//******************************************************************************
void HwUartClose(HW_UART_ID_TYPE uartId);


//******************************************************************************
//
// FUNCTION:    HwUartRd - reads one or more byes from the specified UART. 
//
// INPUT:       uartId - one of HW_UART_ID_A or HW_UART_ID_B
//              buffer - pointer to buffer in which to store received data
//              length - number of bytes to read 
//
// USAGE:       This function is non-blocking and returns immediately even
//              if an insufficient number of bytes are available for reading.
//
// OUTPUT:      return value - the actual number of bytes stored in buffer
//
//******************************************************************************
WORD HwUartRd(HW_UART_ID_TYPE uartID, char *buffer, WORD length);


//******************************************************************************
//
// FUNCTION:    HwUartRdAvail - returns the number of received bytes
//              available for reading.
//
// INPUT:       uartId - one of HW_UART_ID_A or HW_UART_ID_B
//
// OUTPUT:      return value - number of received bytes available
//
//******************************************************************************
WORD HwUartRdAvail(HW_UART_ID_TYPE uartId);


//******************************************************************************
//
// FUNCTION:    HwUartWr - writes one or more bytes to the specified UART.
//
// INPUT:       uartId - one of HW_UART_ID_A or HW_UART_ID_B
//              buffer - pointer to buffer from which to write data 
//              length - number of bytes to write
//
// USAGE:       This function is non-blocking and returns immediately even
//              if insufficient space exists for writing data.
//
// OUTPUT:      return value - number of bytes written from buffer 
//
//******************************************************************************
WORD HwUartWr(HW_UART_ID_TYPE uartID, const char *buffer, WORD length);


//******************************************************************************
//
// FUNCTION:    HwUartWrAvail - returns number of bytes that can be written.
//
// INPUT:       uartId - one of HW_UART_ID_A or HW_UART_ID_B
//
// OUTPUT:      return value - number of bytes that may be written
//
//******************************************************************************
WORD HwUartWrAvail(HW_UART_ID_TYPE uartId);



// TODO
void HwGpioSetPTT(BOOL enabled);
void HwGpioSetLEDs(WORD state);


//******************************************************************************
//
// FUNCTION:    Sets bits on the specified GPIO pin set.
//
// INPUT:       gpioId - one of HW_GPIO_ID_TYPE indicating the port to use
//              pattern - a 16-bit pattern corresponding to the 16 bits of the
//                  given GPIO port. If bit n is set to 1, GPIO pin n will be
//                  set. If bit n is set to 0, no change is made to pin state.
//
// OUTPUT:      none
//
//******************************************************************************
void HwGpioSet(HW_GPIO_ID_TYPE gpioId, WORD pattern);


//******************************************************************************
//
// FUNCTION:    Clears bits on the specified GPIO pin set.
//
// INPUT:       gpioId - one of HW_GPIO_ID_TYPE indicating the port to use
//              pattern - a 16-bit pattern corresponding to the 16 bits of the
//                  given GPIO port. If bit n is set to 1, GPIO pin n will be
//                  cleared. If bit n is set to 0, no change is made to pin 
//                  state.
//
// OUTPUT:      none
//
//******************************************************************************
void HwGpioClear(HW_GPIO_ID_TYPE gpioId, WORD pattern);


//******************************************************************************
//
// FUNCTION:    HwGpioSetOutputEnable - sets the specified GPIO pins for output
//              mode (as opposed to input mode).
//
// INPUT:       gpioId - one of HW_GPIO_ID_TYPE indicating the port to use
//              pattern - a 16-bit pattern corresponding to the 16 bits of the
//                  given GPIO port. If bit n is set to 1, GPIO pin n will be
//                  enabled for output. If bit n is set to 0, no change is 
//                  made.
//
// OUTPUT:      none
//
//******************************************************************************
void HwGpioSetOutputEnable(HW_GPIO_ID_TYPE gpioId, WORD pattern);


//******************************************************************************
//
// FUNCTION:    HwGpioClearOutputEnable - sets the specified GPIO pins for
//              input mode (as opposed to output mode).
//
// INPUT:       gpioId - one of HW_GPIO_ID_TYPE indicating the port to use
//              pattern - a 16-bit pattern corresponding to the 16 bits of the
//                  given GPIO port. If bit n is set to 1, GPIO pin n will be
//                  enabled for input. If bit n is set to 0, no change is 
//                  made.
//
// OUTPUT:      none
//
//******************************************************************************
void HwGpioClearOutputEnable(HW_GPIO_ID_TYPE gpioId, WORD pattern);


//******************************************************************************
//
// FUNCTION:    HwGpioRead - returns the current GPIO input logic state.
//
// INPUT:       gpioId - one of HW_GPIO_ID_TYPE indicating the port to use
//
// OUTPUT:      return value - the input state of each pin of the specified
//              GPIO port, where bit n corresponds to pin n
//
//******************************************************************************
WORD HwGpioRead(HW_GPIO_ID_TYPE gpioId);


//
// Codec Interface
//


void HwCodecOpen(HW_CODEC_SAMPLE_RATE_TYPE sampleRate);
void HwCodecClose(void);
BOOL HwCodecSetRxPGAGain(HW_CODEC_GAIN_TYPE gain);
BOOL HwCodecSetTxPGAGain(HW_CODEC_GAIN_TYPE gain);
BOOL HwCodecSetRxChannel(HW_CODEC_RX_CHANNEL_TYPE channel);
BOOL HwCodecSetLoopback(BOOL enable);
WORD HwCodecRdAvail(void);
WORD HwCodecRd(WORD *buffer, WORD count);
WORD HwCodecWrAvail(void);
WORD HwCodecWr(WORD *buffer, WORD count);


//
// Timer Interface
//


void HwTimerConfigure(WORD delayInMS, void (*funcPtr)(void));
void HwTimerEnable(void);
void HwTimerDisable(void);
DWORD HwTimerGetCurrentTicks(void);
DWORD HwTimerGetTicksPerMS(void);

#endif
