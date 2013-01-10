//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Responsible for all hardware and firmware initialization upon reset. 
//
//******************************************************************************


#define __HWINITHW_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "HwInterface.h"
#include "HwConfig.h"
#include "GlobalDefs.h"
#include "DSP281x_Device.h"


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************


#define PLL_LOCK_DELAY  5000


//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


void HwGpioInit(void);
void HwTimerInit(void);
void HwUartInit(void);
void HwCodecInit(void);


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


void InitFlash(void);


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************


extern WORD RamCodeLoadStart;
extern WORD RamCodeLoadEnd;
extern WORD RamCodeRunStart;


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  C O D E
//******************************************************************************


void HwInit(void)
{
    volatile WORD i;

    EALLOW;

    // Disable watchdog
    SysCtrlRegs.WDCR = 0x0068;

    // Place device in unsecure mode with dummy reads from PWL locations
    i = CsmPwl.PSWD0;
    i = CsmPwl.PSWD1;
    i = CsmPwl.PSWD2;
    i = CsmPwl.PSWD3;
    i = CsmPwl.PSWD4;
    i = CsmPwl.PSWD5;
    i = CsmPwl.PSWD6;
    i = CsmPwl.PSWD7;

    // Initalize PLL and wait for lock
    SysCtrlRegs.PLLCR.bit.DIV = HW_DSP_CLKIN_MULT << 1;
    for(i = 0; i < PLL_LOCK_DELAY; ++i);
       
    // Lo-speed, hi-speed peripheral clock prescale settings
    SysCtrlRegs.HISPCP.all = HW_DSP_HSPCLK_DIV >> 1;
    SysCtrlRegs.LOSPCP.all = HW_DSP_LSPCLK_DIV >> 1;
    
    // Copy RAM code into RAM from FLASH
    memmove(&RamCodeRunStart, &RamCodeLoadStart, &RamCodeLoadEnd - &RamCodeLoadStart);

    // Initialize FLASH (jump to code in RAM so we can change FLASH settings)
    InitFlash();

    // Enable all clocks (full blast)
    SysCtrlRegs.PCLKCR.bit.EVAENCLK = 1;
    SysCtrlRegs.PCLKCR.bit.EVBENCLK = 1;
    SysCtrlRegs.PCLKCR.bit.SCIAENCLK = 1;
    SysCtrlRegs.PCLKCR.bit.SCIBENCLK = 1;
    SysCtrlRegs.PCLKCR.bit.MCBSPENCLK = 1;
    SysCtrlRegs.PCLKCR.bit.SPIENCLK = 1;
    SysCtrlRegs.PCLKCR.bit.ECANENCLK = 1;
    SysCtrlRegs.PCLKCR.bit.ADCENCLK = 1;

    // Disable all maskable CPU interrupts and clear interrupt flags
    DINT;
    IER = 0x0000;
    IFR = 0x0000;

    // Clear interrupt enable and flag registers
    PieCtrlRegs.PIECRTL.bit.ENPIE = 0;
    PieCtrlRegs.PIEIER1.all = 0;
    PieCtrlRegs.PIEIER2.all = 0;
    PieCtrlRegs.PIEIER3.all = 0;	
    PieCtrlRegs.PIEIER4.all = 0;
    PieCtrlRegs.PIEIER5.all = 0;
    PieCtrlRegs.PIEIER6.all = 0;
    PieCtrlRegs.PIEIER7.all = 0;
    PieCtrlRegs.PIEIER8.all = 0;
    PieCtrlRegs.PIEIER9.all = 0;
    PieCtrlRegs.PIEIER10.all = 0;
    PieCtrlRegs.PIEIER11.all = 0;
    PieCtrlRegs.PIEIER12.all = 0;
    PieCtrlRegs.PIEIFR1.all = 0;
    PieCtrlRegs.PIEIFR2.all = 0;
    PieCtrlRegs.PIEIFR3.all = 0;	
    PieCtrlRegs.PIEIFR4.all = 0;
    PieCtrlRegs.PIEIFR5.all = 0;
    PieCtrlRegs.PIEIFR6.all = 0;
    PieCtrlRegs.PIEIFR7.all = 0;
    PieCtrlRegs.PIEIFR8.all = 0;
    PieCtrlRegs.PIEIFR9.all = 0;
    PieCtrlRegs.PIEIFR10.all = 0;
    PieCtrlRegs.PIEIFR11.all = 0;
    PieCtrlRegs.PIEIFR12.all = 0;

    // Enable PIE
    PieCtrlRegs.PIEACK.all = 0xFFFF;
    PieCtrlRegs.PIECRTL.bit.ENPIE = 1;

    EDIS;

    HwGpioInit();
    HwTimerInit();
    HwUartInit();
    HwCodecInit();

    // Enable maskable CPU interrupts
    EINT;
}

#pragma CODE_SECTION(InitFlash, RAM_CODE_SECTION)
void InitFlash(void)
{
    FlashRegs.FPWR.bit.PWR = 3;
    FlashRegs.FSTATUS.bit.V3STAT = 1;
    FlashRegs.FSTDBYWAIT.bit.STDBYWAIT = HW_FLASH_STANDBY_WAIT;
    FlashRegs.FACTIVEWAIT.bit.ACTIVEWAIT = HW_FLASH_ACTIVE_WAIT;
    FlashRegs.FBANKWAIT.bit.RANDWAIT = HW_FLASH_RANDOM_WAIT;
    FlashRegs.FBANKWAIT.bit.PAGEWAIT = HW_FLASH_PAGE_WAIT;
    FlashRegs.FOTPWAIT.bit.OTPWAIT = HW_FLASH_OTP_WAIT;
    FlashRegs.FOPT.bit.ENPIPE = 1;

    // Force a pipeline flush before we return (to ensure registers are
    // written before jumping back to code in FLASH)
    asm("   rpt #8 || nop");
}


/***********************************  END  ************************************/
