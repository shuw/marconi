//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:   Implements a (file-like) read/write interface to the UARTs.
//
//******************************************************************************


#define __HWUART_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "HwInterface.h"
#include "HwConfig.h"
#include "HwFifo.h"
#include "Dsp281x_Device.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************


#define UART_INT_GROUP      0x100

// Stores state for each UART 'file'
typedef struct
{
    volatile struct SCI_REGS    *regs;
    HW_FIFO_TYPE                rxFifo;
    HW_FIFO_TYPE                txFifo;
    BOOL                        uartIsOpened;
    volatile BOOL               txIsIdle;
} UART_FILE_TYPE;


//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


static interrupt void UartARxIsr(void);
static interrupt void UartATxIsr(void);
static interrupt void UartBRxIsr(void);
static interrupt void UartBTxIsr(void);


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


// State for all UARTs
static UART_FILE_TYPE uartFile[NUM_HW_UART_ID];

// FIFO buffers
static WORD uartARxBuffer[HW_UART_A_RX_FIFO_LEN];
static WORD uartATxBuffer[HW_UART_A_TX_FIFO_LEN];
static WORD uartBRxBuffer[HW_UART_B_RX_FIFO_LEN];
static WORD uartBTxBuffer[HW_UART_B_TX_FIFO_LEN];


//******************************************************************************
//  C O D E
//******************************************************************************


void HwUartInit(void)
{
    // Configure UART register pointers and default to UART closed
    uartFile[HW_UART_ID_A].regs = &SciaRegs;
    uartFile[HW_UART_ID_A].uartIsOpened = FALSE;
    uartFile[HW_UART_ID_B].regs = &ScibRegs;
    uartFile[HW_UART_ID_B].uartIsOpened = FALSE;

    // Reset and disable both UARTs
    uartFile[HW_UART_ID_A].regs->SCICTL1.all = 0;
    uartFile[HW_UART_ID_A].regs->SCICTL2.all = 0;
    uartFile[HW_UART_ID_B].regs->SCICTL1.all = 0;
    uartFile[HW_UART_ID_B].regs->SCICTL2.all = 0;

    // Initialize the FIFOs
    HwFifoInit(&uartFile[HW_UART_ID_A].rxFifo, uartARxBuffer, HW_UART_A_RX_FIFO_LEN);
    HwFifoInit(&uartFile[HW_UART_ID_A].txFifo, uartATxBuffer, HW_UART_A_TX_FIFO_LEN);
    HwFifoInit(&uartFile[HW_UART_ID_B].rxFifo, uartBRxBuffer, HW_UART_B_RX_FIFO_LEN);
    HwFifoInit(&uartFile[HW_UART_ID_B].txFifo, uartBTxBuffer, HW_UART_B_TX_FIFO_LEN);

    // Enable interrupts in PIE for SCI_A and SCI_B
    PieCtrlRegs.PIEIER9.bit.INTx1 = 1;
    PieCtrlRegs.PIEIER9.bit.INTx2 = 1;
    PieCtrlRegs.PIEIER9.bit.INTx3 = 1;
    PieCtrlRegs.PIEIER9.bit.INTx4 = 1;

    // Enable interrupts in CPU for UART
    IER |= UART_INT_GROUP;
    
    EALLOW;

    // Register ISRs with PIE
    PieVectTable.RXAINT = &UartARxIsr;
    PieVectTable.TXAINT = &UartATxIsr;
    PieVectTable.RXBINT = &UartBRxIsr;
    PieVectTable.TXBINT = &UartBTxIsr;

    // Set appropriate GPIO pins to function as SCIA, SCIB
    GpioMuxRegs.GPFMUX.bit.SCITXDA_GPIOF4 = 1;
    GpioMuxRegs.GPFMUX.bit.SCIRXDA_GPIOF5 = 1;
    GpioMuxRegs.GPGMUX.bit.SCITXDB_GPIOG4 = 1;
    GpioMuxRegs.GPGMUX.bit.SCIRXDB_GPIOG5 = 1;

    EDIS;
}


void HwUartOpen(HW_UART_ID_TYPE uart, DWORD rate, HW_UART_PARITY_TYPE parity)
{
    if(uart >= NUM_HW_UART_ID || uartFile[uart].uartIsOpened) {
        return;
    }

    // Reset SCI
    uartFile[uart].regs->SCICTL1.all = 0;
    uartFile[uart].regs->SCICTL2.all = 0;
 
    // Don't use SCI hardware FIFOs
    uartFile[uart].regs->SCIFFTX.bit.TXFIFOXRESET = 0;
    uartFile[uart].regs->SCIFFRX.bit.RXFIFORESET = 0;
    uartFile[uart].regs->SCIFFCT.all = 0;

    // NOTE: This must be cleared before Tx interupts will work!
    // TI's Bootrom fails to do this after autobaud lock
    uartFile[uart].regs->SCIFFCT.bit.ABDCLR = 1;

    // Character length and parity settings
 	uartFile[uart].regs->SCICCR.bit.SCICHAR = 7;
    switch(parity) 
    {
        case HW_UART_PARITY_EVEN:
            uartFile[uart].regs->SCICCR.bit.PARITYENA = 1;
            uartFile[uart].regs->SCICCR.bit.PARITY = 1;
            break;
        case HW_UART_PARITY_ODD:
            uartFile[uart].regs->SCICCR.bit.PARITYENA = 1;
            uartFile[uart].regs->SCICCR.bit.PARITY = 0;
            break;
        case HW_UART_PARITY_NONE:
            uartFile[uart].regs->SCICCR.bit.PARITYENA = 0;
            break;
    }

    // Calculate and set baud rate register
    rate = HW_DSP_LSPCLK_HZ / (rate * 8) - 1;
    uartFile[uart].regs->SCIHBAUD = (rate >> 8) & 0xFF;
    uartFile[uart].regs->SCILBAUD = rate & 0xFF;
    
    // Set UART state to opened and reset FIFOs
    uartFile[uart].uartIsOpened = TRUE;
    uartFile[uart].txIsIdle = TRUE;
    HwFifoReset(&uartFile[uart].rxFifo);
    HwFifoReset(&uartFile[uart].txFifo);

    // Enable SCI Rx and Tx
    uartFile[uart].regs->SCICTL1.bit.TXENA = 1;
    uartFile[uart].regs->SCICTL1.bit.RXENA = 1;
    uartFile[uart].regs->SCICTL2.bit.TXINTENA = 1;
    uartFile[uart].regs->SCICTL2.bit.RXBKINTENA = 1;
    uartFile[uart].regs->SCICTL1.bit.RXERRINTENA = 1;
	uartFile[uart].regs->SCICTL1.bit.SWRESET = 1; 
}


void HwUartClose(HW_UART_ID_TYPE uart)
{
    if(uart >= NUM_HW_UART_ID || !uartFile[uart].uartIsOpened) {
        return;
    }

    // Reset SCI
    uartFile[uart].regs->SCICTL1.all = 0;
    uartFile[uart].regs->SCICTL2.all = 0;

    // Set UART state to closed
    uartFile[uart].uartIsOpened = FALSE;
}


WORD HwUartRd(HW_UART_ID_TYPE uart, char *buffer, WORD length)
{
    if(uart >= NUM_HW_UART_ID || !uartFile[uart].uartIsOpened) {
        return 0;
    }

    // NOTE: This cast from BYTE* to WORD* is only safe because
    // the C2000 compiler produces 16-bit chars!
    return HwFifoRd(&uartFile[uart].rxFifo, (WORD*)buffer, length);
}


WORD HwUartRdAvail(HW_UART_ID_TYPE uart)
{
    if(uart >= NUM_HW_UART_ID || !uartFile[uart].uartIsOpened) {
        return 0;
    }

    return HwFifoRdAvail(&uartFile[uart].rxFifo);
}


WORD HwUartWr(HW_UART_ID_TYPE uart, const char *buffer, WORD length)
{
    WORD count;
    WORD read;

    if(uart >= NUM_HW_UART_ID || !uartFile[uart].uartIsOpened) {
        return 0;
    }

    // NOTE: This cast from BYTE* to WORD* is only safe because
    // the C2000 compiler produces 16-bit chars!
    count = HwFifoWr(&uartFile[uart].txFifo, (WORD*)buffer, length);

    // Manually trigger a Tx interrupt if idle to start transmission
    if(uartFile[uart].txIsIdle && length > 0) {
        // Reading from this FIFO here is safe since we are guaranteed
        // that an interrupt cannot fire until we write to SCITXBUF
        HwFifoRd(&uartFile[uart].txFifo, &read, 1);
        uartFile[uart].regs->SCITXBUF = read;
    }

    return count;
}


WORD HwUartWrAvail(HW_UART_ID_TYPE uart)
{
    if(uart >= NUM_HW_UART_ID || !uartFile[uart].uartIsOpened) {
        return 0;
    }

    return HwFifoWrAvail(&uartFile[uart].txFifo);
}


static interrupt void UartARxIsr(void)
{
    WORD received;

    if(uartFile[HW_UART_ID_A].regs->SCIRXST.bit.RXERROR) {
        // Simply ignore: break detect, framing error, (hardware) overrun, parity error
        // Reset the SCI and continue
        uartFile[HW_UART_ID_A].regs->SCICTL1.bit.SWRESET = 0;
        uartFile[HW_UART_ID_A].regs->SCICTL1.bit.SWRESET = 1;
    }
    else {
        received = uartFile[HW_UART_ID_A].regs->SCIRXBUF.all & 0xFF;

        if(HwFifoWr(&uartFile[HW_UART_ID_A].rxFifo, &received, 1) != 1)
        {
            // TODO: We have receiver overrun!
        }
    }

    // Acknowledge PIE interrupt
    PieCtrlRegs.PIEACK.all |= UART_INT_GROUP;
}


static interrupt void UartATxIsr(void)
{
    WORD transmitted;

    if(HwFifoRd(&uartFile[HW_UART_ID_A].txFifo, &transmitted, 1) == 1) 
    {
        uartFile[HW_UART_ID_A].regs->SCITXBUF = transmitted;
        uartFile[HW_UART_ID_A].txIsIdle = FALSE;
    }
    else
    {
        uartFile[HW_UART_ID_A].txIsIdle = TRUE;
    }

    // Acknowledge PIE interrupt
    PieCtrlRegs.PIEACK.all |= UART_INT_GROUP;
}


static interrupt void UartBRxIsr(void)
{
    WORD received;

    if(uartFile[HW_UART_ID_B].regs->SCIRXST.bit.RXERROR) {
        // Simply ignore: break detect, framing error, (hardware) overrun, parity error
        // Reset the SCI and continue
        uartFile[HW_UART_ID_B].regs->SCICTL1.bit.SWRESET = 0;
        uartFile[HW_UART_ID_B].regs->SCICTL1.bit.SWRESET = 1;
    }
    else {
        received = uartFile[HW_UART_ID_B].regs->SCIRXBUF.all & 0xFF;

        if(HwFifoWr(&uartFile[HW_UART_ID_B].rxFifo, &received, 1) != 1)
        {
            // TODO: We have receiver overrun!
        }
    }

    // Acknowledge PIE interrupt
    PieCtrlRegs.PIEACK.all |= UART_INT_GROUP;
}


static interrupt void UartBTxIsr(void)
{
    WORD transmitted;

    if(HwFifoRd(&uartFile[HW_UART_ID_B].txFifo, &transmitted, 1) == 1) 
    {
        uartFile[HW_UART_ID_B].regs->SCITXBUF = transmitted;
        uartFile[HW_UART_ID_B].txIsIdle = FALSE;
    }
    else
    {
        uartFile[HW_UART_ID_B].txIsIdle = TRUE;
    }

    // Acknowledge PIE interrupt
    PieCtrlRegs.PIEACK.all |= UART_INT_GROUP;
}


/***********************************  END  ************************************/
