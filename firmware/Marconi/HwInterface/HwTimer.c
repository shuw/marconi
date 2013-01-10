//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:   Implements interface to hardware timer with callback functions.
//
//******************************************************************************


#define __HWTIMER_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "HwInterface.h"
#include "HwConfig.h"
#include "Dsp281x_Device.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************


#define MS_PER_SECOND   1000
#define TICKS_PER_MS    (HW_DSP_SYSCLK_HZ / MS_PER_SECOND)


//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


static interrupt void TimerISR(void);


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


// Pointer to function to call when timer fires
static void (*timerCallback)(void);
static BOOL isConfigured;


//******************************************************************************
//  C O D E
//******************************************************************************


void HwTimerInit(void)
{
    // Register Timer0 ISR
    EALLOW;
    PieVectTable.TINT0 = &TimerISR;
    EDIS;
    
    // Disable prescale
    CpuTimer0Regs.TPR.all = 0;
    CpuTimer0Regs.TPRH.all = 0;

    // Stop timer and enable interrupts
    CpuTimer0Regs.TCR.bit.TSS = 1;
    CpuTimer0Regs.TCR.bit.TIE = 1;

    // Enable interrupts (PIE and CPU) for Timer0
    PieCtrlRegs.PIEIER1.bit.INTx7 = 1;
    IER |= M_INT1;

    isConfigured = FALSE;
}


void HwTimerConfigure(WORD delayInMS, void (*functionPtr)(void))
{
    // Stop timer if running
    CpuTimer0Regs.TCR.bit.TSS = 1;

    // Configure new period and reload counter
    CpuTimer0Regs.PRD.all = TICKS_PER_MS * delayInMS;
    CpuTimer0Regs.TCR.bit.TRB = 1;

    // Save pointer to callback function
    timerCallback = functionPtr;

    isConfigured = TRUE;
}


void HwTimerEnable(void)
{
    if(isConfigured) {
        // Clear stop flag
        CpuTimer0Regs.TCR.bit.TSS = 0;
    }
}


void HwTimerDisable(void)
{
    // Set stop flag
    CpuTimer0Regs.TCR.bit.TSS = 1;
}


DWORD HwTimerGetCurrentTicks(void)
{
    // 'Ticks' is difference between period and countdown
    return CpuTimer0Regs.PRD.all - CpuTimer0Regs.TIM.all;
}


DWORD HwTimerGetTicksPerMS(void)
{
    return TICKS_PER_MS;
}


static interrupt void TimerISR(void)
{
    // Re-enable interrupts for CPU
    EINT;
    
    // NOTE: At this point interrupts from other groups can be handled, but
    // the TimerISR is not re-entrant (even if timerCallback is too slow) since
    // interrupts for this group remain disabled until the ISR returns.

    // Call timer callback
    (*timerCallback)(); 

    // Acknowledge PIE interrupt
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP1;
}

/***********************************  END  ************************************/
