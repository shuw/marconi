//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:   Interface to audio codec.
//
// Important notes on the assumptions implicit in this code:
//
// The codec is always operated in the 15 + 1 bit mode for both DIN and DOUT,
// which is the default setting upon codec reset. This allows 15 bits of audio 
// precision as well as software request for secondary communication via the
// LSB of DIN. Additionally, the LSB of DOUT provides a handy way to identify 
// which received words are from the secondary communication cycle, so they
// can be eliminated from the audio stream. In 15 + 1 mode, the codec sets the
// LSB of DOUT when configured as 'master' (which is what we use). However, when a 
// secondary communication WRITE cycle is initiated, the least significant 8
// bits of DOUT are guaranteed to be zero. Thus, in the Rx ISR we simply ignore 
// any incoming words that do not have the LSB set. Note that this CANNOT be
// guaranteed to work if secondary communication READ cycles are issued, since the
// least significant 8 bits of DOUT will then contain register data, which may
// have the LSB set! Codec register reads are thus not supported.
//
//******************************************************************************


#define __HWCODEC_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************


#include "HwInterface.h"
#include "HwConfig.h"
#include "DSP281x_Device.h"
#include "GlobalDefs.h"
#include "HwFifo.h"


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

// Codec register definitions (for secondary communications cycle)

#define CODEC_DEVICE_ID             0x00
#define CODEC_DEVICE_ID_SHIFT       13
#define CODEC_REG_RD                0x1000
#define CODEC_REG_SHIFT             9

typedef enum {
    CODEC_REG_1,
    CODEC_REG_2,
    CODEC_REG_3,
    CODEC_REG_4,
    NUM_CODEC_REG
} CODEC_REG_TYPE;

#define CODEC_REG_1_BYPASS_FILTER   0x20
#define CODEC_REG_1_BYPASS_FIR      0x04
#define CODEC_REG_1_16_BIT_DAC      0x01 
#define CODEC_REG_1_AUX_IN_EN       0x10

#define CODEC_REG_2_LOW_POWER_EN    0x80
#define CODEC_REG_2_MODULATOR_STOP  0x40
#define CODEC_REG_2_DIVIDER_MASK    0x1F

#define CODEC_REG_3_ANALOG_LB_EN    0x40
#define CODEC_REG_3_DIGITAL_LB_EN   0x80
#define CODEC_REG_3_16_BIT_ADC      0x01

#define CODEC_REG_4_RX_GAIN_SHIFT   4
#define CODEC_REG_4_GAIN_MASK       0x0F

#define CODEC_SAMPLE_MASK           0xFFFE
#define CODEC_SECONDARY_FLAG        0x0001
#define CODEC_MASTER_FLAG           0x0001

// ISR group and hardware FIFO sizes

#define MCBSP_ISR_GROUP             0x20
#define MCBSP_FIFO_MAX_LEN          16
#define MCBSP_FIFO_LEN              16


//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


static interrupt void CodecRxISR(void);
static interrupt void CodecTxISR(void);
static inline BYTE CodecRegRd(CODEC_REG_TYPE reg);
static inline BOOL CodecRegWr(CODEC_REG_TYPE reg, BYTE value);


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


static HW_FIFO_TYPE rxFifo;
static HW_FIFO_TYPE txFifo;
static HW_FIFO_TYPE cmdFifo;

static WORD rxFifoBuffer[HW_CODEC_RX_FIFO_LEN];
static WORD txFifoBuffer[HW_CODEC_TX_FIFO_LEN];
static WORD cmdFifoBuffer[HW_CODEC_CMD_FIFO_LEN];

static BOOL codecOpened;

// Scratch pad buffer for ISRs (we only need one, since tx and
// rx ISR are guaranteed to not interrupt each other)
static WORD scratchPad[MCBSP_FIFO_LEN];

// Store local copies of codec registers for easy modification
static BYTE codecRegData[NUM_CODEC_REG];


//******************************************************************************
//  C O D E
//******************************************************************************

void HwCodecInit(void)
{
    // Reset McBsp
    McbspaRegs.SPCR2.all = 0;
    McbspaRegs.SPCR1.all = 0;
    McbspaRegs.RCR2.all = 0;
    McbspaRegs.RCR1.all = 0;
    McbspaRegs.XCR2.all = 0;
    McbspaRegs.XCR1.all = 0;
    McbspaRegs.MCR1.all = 0;
    McbspaRegs.MCR2.all = 0;
    McbspaRegs.SRGR2.all = 0;                      
    McbspaRegs.SRGR1.all = 0;
    McbspaRegs.PCR.all = 0;
    McbspaRegs.MFFTX.all = 0;
    McbspaRegs.MFFRX.all = 0;
    
    // Set McBsp pins in GPIOF to peripheral mode
    EALLOW;
    GpioMuxRegs.GPFMUX.bit.MCLKXA_GPIOF8 = 1;
    GpioMuxRegs.GPFMUX.bit.MCLKRA_GPIOF9 = 1;
    GpioMuxRegs.GPFMUX.bit.MFSXA_GPIOF10 = 1;
    GpioMuxRegs.GPFMUX.bit.MFSRA_GPIOF11 = 1;
    GpioMuxRegs.GPFMUX.bit.MDXA_GPIOF12 = 1;
    GpioMuxRegs.GPFMUX.bit.MDRA_GPIOF13 = 1;
    EDIS;

    // 16-bit word size for samples
    McbspaRegs.RCR1.bit.RWDLEN1 = 2;
    McbspaRegs.XCR1.bit.XWDLEN1 = 2;

    // Clock polarities and data delays
    McbspaRegs.PCR.bit.CLKRP = 0;
    McbspaRegs.PCR.bit.CLKXP = 0;
    McbspaRegs.RCR2.bit.RDATDLY = 1;
    McbspaRegs.XCR2.bit.XDATDLY = 1;

    // Configure McBsp FIFO
    McbspaRegs.MFFTX.bit.MFFENA = 1;
    McbspaRegs.MFFTX.bit.TXFFIENA = 1;
    McbspaRegs.MFFTX.bit.TXFFIL = MCBSP_FIFO_MAX_LEN - MCBSP_FIFO_LEN;
    McbspaRegs.MFFRX.bit.RXFFIENA = 1;
    McbspaRegs.MFFRX.bit.RXFFIL = MCBSP_FIFO_LEN;

    // Register ISRs and enable McBsp interrupts
    EALLOW;
    PieVectTable.MRINTA = &CodecRxISR; 
    PieVectTable.MXINTA = &CodecTxISR;
    EDIS;

    PieCtrlRegs.PIEIER6.bit.INTx5 = 1;
    PieCtrlRegs.PIEIER6.bit.INTx6 = 1;
    IER |= MCBSP_ISR_GROUP;

    codecOpened = FALSE;
}


void HwCodecOpen(HW_CODEC_SAMPLE_RATE_TYPE sampleRate)
{
    BYTE clockDivider;

    if(codecOpened) {
        return;
    }

    HwFifoInit(&rxFifo, rxFifoBuffer, HW_CODEC_RX_FIFO_LEN);
    HwFifoInit(&txFifo, txFifoBuffer, HW_CODEC_TX_FIFO_LEN);
    HwFifoInit(&cmdFifo, cmdFifoBuffer, HW_CODEC_CMD_FIFO_LEN);

    CodecRegWr(CODEC_REG_1, 0);

    // Calculate clock divider from input clock frequency and desired sample rate
    switch(sampleRate) {
        case HW_CODEC_SAMPLE_RATE_4_KHZ:
            clockDivider = HW_CODEC_CLK_HZ / (HW_CODEC_CLK_DIVIDE * 4000UL);
            break;
        case HW_CODEC_SAMPLE_RATE_8_KHZ:
            clockDivider = HW_CODEC_CLK_HZ / (HW_CODEC_CLK_DIVIDE * 8000UL);
            break;
        case HW_CODEC_SAMPLE_RATE_16_KHZ:
        default:
            clockDivider = HW_CODEC_CLK_HZ / (HW_CODEC_CLK_DIVIDE * 16000UL);
            break;
    }    

    CodecRegWr(CODEC_REG_2, clockDivider); 
    CodecRegWr(CODEC_REG_3, 0);
    CodecRegWr(CODEC_REG_4, HW_CODEC_GAIN_0_DB << CODEC_REG_4_RX_GAIN_SHIFT | 
        HW_CODEC_GAIN_0_DB);

    // Enable FIFO
    McbspaRegs.MFFTX.bit.TXFIFO_RESET = 1;
    McbspaRegs.MFFRX.bit.RXFIFO_RESET = 1;

    // Take McBsp out of reset
    McbspaRegs.SPCR2.bit.XRST = 1;
    McbspaRegs.SPCR1.bit.RRST = 1;

    codecOpened = TRUE;
}


void HwCodecClose(void)
{
    // Hold McBsp in reset 
    McbspaRegs.SPCR2.bit.XRST = 0;
    McbspaRegs.SPCR1.bit.RRST = 0;

    // Reset FIFO
    McbspaRegs.MFFTX.bit.TXFIFO_RESET = 0;
    McbspaRegs.MFFRX.bit.RXFIFO_RESET = 0;

    codecOpened = FALSE;
}


BOOL HwCodecSetRxPGAGain(HW_CODEC_GAIN_TYPE gain)
{
    if(!codecOpened) {
        return FALSE;
    }

    return CodecRegWr(CODEC_REG_4, 
        (CodecRegRd(CODEC_REG_4) & CODEC_REG_4_GAIN_MASK) | 
        (gain << CODEC_REG_4_RX_GAIN_SHIFT));
}


BOOL HwCodecSetTxPGAGain(HW_CODEC_GAIN_TYPE gain)
{    
    if(!codecOpened) {
        return FALSE;
    }

    return CodecRegWr(CODEC_REG_4, 
        (CodecRegRd(CODEC_REG_4) & ~CODEC_REG_4_GAIN_MASK) | gain);
}


BOOL HwCodecSetRxChannel(HW_CODEC_RX_CHANNEL_TYPE channel)
{
    if(!codecOpened) {
        return FALSE;
    }

    if(channel == HW_CODEC_RX_CHANNEL_1) {
        CodecRegWr(CODEC_REG_1, CodecRegRd(CODEC_REG_1) & ~CODEC_REG_1_AUX_IN_EN);
    }
    else {
        CodecRegWr(CODEC_REG_1, CodecRegRd(CODEC_REG_1) | CODEC_REG_1_AUX_IN_EN);
    }

    return FALSE;
}


WORD HwCodecWrAvail(void)
{
    return codecOpened ? HwFifoWrAvail(&txFifo) : 0;
}


WORD HwCodecWr(WORD *buffer, WORD count)
{
    return codecOpened ? HwFifoWr(&txFifo, buffer, count) : 0;
}


WORD HwCodecRdAvail(void)
{
    return codecOpened ? HwFifoRdAvail(&rxFifo) : 0;
}


WORD HwCodecRd(WORD *buffer, WORD count)
{
    return codecOpened ? HwFifoRd(&rxFifo, buffer, count) : 0;
}


// TODO: See if there are any other possible interrupt conditions we should 
// worry about here...


// McBsp receive FIFO interrupt handler
static interrupt void CodecRxISR(void)
{
    WORD i;
    WORD count;

    for(i = 0; i < MCBSP_FIFO_LEN; ++i) {
        scratchPad[i] = McbspaRegs.DRR1.all;
    }

    // Only copy words with LSB set to the audio stream:
    // clear the LSB for audio samples since flag not part of sample
    count = 0;
    for(i = 0; i < MCBSP_FIFO_LEN; ++i) {
        if((scratchPad[i] & CODEC_MASTER_FLAG) == CODEC_MASTER_FLAG) 
        {
            scratchPad[count++] = scratchPad[i] & CODEC_SAMPLE_MASK;
        }
    }

    HwFifoWr(&rxFifo, scratchPad, count);

    // Ignore (and clear) any error flags and acknowledge interrupt
    McbspaRegs.MFFRX.bit.RXFFOVF_CLEAR = 1;
    McbspaRegs.MFFRX.bit.RXFFINT_CLEAR = 1;
    PieCtrlRegs.PIEACK.all |= MCBSP_ISR_GROUP;
}


// McBsp transmit FIFO interrupt handler
static interrupt void CodecTxISR(void)
{
    WORD i;

    // Zero scratch pad
    for(i = 0; i < MCBSP_FIFO_LEN - 1; ++i) {
        scratchPad[i] = 0;
    }

    // The last word we transmit in every 16-word group is for
    // the (secondary communication) command cycle: leave room for this!
    HwFifoRd(&txFifo, scratchPad, MCBSP_FIFO_LEN - 1);

    for(i = 0; i < MCBSP_FIFO_LEN - 1; ++i) {
        scratchPad[i] &= CODEC_SAMPLE_MASK;
    }

    // Insert secondary communication write request and data 
    scratchPad[MCBSP_FIFO_LEN - 2] |= CODEC_SECONDARY_FLAG;
    scratchPad[MCBSP_FIFO_LEN - 1] = 0;
    HwFifoRd(&cmdFifo, &scratchPad[MCBSP_FIFO_LEN - 1], 1);

    for(i = 0; i < MCBSP_FIFO_LEN; ++i) {
        McbspaRegs.DXR1.all = scratchPad[i];
    }

    // Acknowledge interrupt
    McbspaRegs.MFFTX.bit.TXFFINT_CLEAR = 1;
    PieCtrlRegs.PIEACK.all |= MCBSP_ISR_GROUP;
}


// Read local copy of codec register data
static inline BYTE CodecRegRd(CODEC_REG_TYPE reg)
{
    return codecRegData[reg];
}


// Enqueue a codec register write command
static inline BOOL CodecRegWr(CODEC_REG_TYPE reg, BYTE value)
{
    WORD command;

    // Update local copy
    codecRegData[reg] = value;

    // We add 1 to reg since codec register zero is actually 'NOP'
    command = 
        (CODEC_DEVICE_ID << CODEC_DEVICE_ID_SHIFT) | 
        ((reg + 1) << CODEC_REG_SHIFT) | 
        value;

    return HwFifoWr(&cmdFifo, &command, 1) == 1 ? TRUE : FALSE;
}


/***********************************  END  ************************************/
