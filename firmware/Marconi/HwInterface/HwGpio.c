//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// USAGE:   Implements interface to GPIO pins.
//
//******************************************************************************


#define __HWGPIO_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************

#include "GlobalDefs.h"
#include "HwInterface.h"
#include "DSP281x_Device.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

#define PTT_GPIO_PIN    0x0100
#define LED_GPIO_MASK   0x00FF

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  S T A T I C    F U N C T I O N    P R O T O T Y P E S
//******************************************************************************


//******************************************************************************
//  G L O B A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  L O C A L    V A R I A B L E S
//******************************************************************************


//******************************************************************************
//  C O D E
//******************************************************************************




void HwGpioInit(void)
{
    // Set GPIOA and GPIOB pins to function as GPIOs
    GpioMuxRegs.GPAMUX.all = 0;
    GpioMuxRegs.GPBMUX.all = 0;

    // Enable GPIO A as output
    HwGpioSetOutputEnable(HW_GPIO_ID_A, 0xFFFF);
    HwGpioClear(HW_GPIO_ID_A, 0xFFFF);
}


void HwGpioSetPTT(BOOL enabled)
{
    if(enabled) {
        HwGpioSet(HW_GPIO_ID_A, PTT_GPIO_PIN);
    }
    else {
        HwGpioClear(HW_GPIO_ID_A, PTT_GPIO_PIN);
    }
}


void HwGpioSetLEDs(WORD state)
{
    HwGpioClear(HW_GPIO_ID_A, LED_GPIO_MASK);
    HwGpioSet(HW_GPIO_ID_A, state & LED_GPIO_MASK);
}


void HwGpioSet(HW_GPIO_ID_TYPE gpioId, WORD pattern)
{
    EALLOW;

    if(gpioId == HW_GPIO_ID_A) 
    {
        GpioDataRegs.GPASET.all = pattern;
    }
    else 
    {
        GpioDataRegs.GPBSET.all = pattern;
    }

    EDIS;
}


void HwGpioClear(HW_GPIO_ID_TYPE gpioId, WORD pattern)
{
    EALLOW;

    if(gpioId == HW_GPIO_ID_A) 
    {
        GpioDataRegs.GPACLEAR.all = pattern;
    }
    else 
    {
        GpioDataRegs.GPBCLEAR.all = pattern;
    }

    EDIS;
}


void HwGpioSetOutputEnable(HW_GPIO_ID_TYPE gpioId, WORD pattern)
{
    WORD state;

    EALLOW;

    if(gpioId == HW_GPIO_ID_A) 
    {
        state = GpioMuxRegs.GPADIR.all;
        state = state | pattern;
        GpioMuxRegs.GPADIR.all = state;
    }
    else 
    {
        state = GpioMuxRegs.GPBDIR.all;
        state = state | pattern;
        GpioMuxRegs.GPBDIR.all = state;
    }

    EDIS;
}


void HwGpioClearOutputEnable(HW_GPIO_ID_TYPE gpioId, WORD pattern)
{
    WORD state;

    EALLOW;

    if(gpioId == HW_GPIO_ID_A) 
    {
        state = GpioMuxRegs.GPADIR.all;
        state = state & ~pattern;
        GpioMuxRegs.GPADIR.all = state;
    }
    else 
    {
        state = GpioMuxRegs.GPBDIR.all;
        state = state & ~pattern;
        GpioMuxRegs.GPBDIR.all = state;
    }

    EDIS;

}


WORD HwGpioRead(HW_GPIO_ID_TYPE gpioId)
{
    WORD state;

    EALLOW;

    if(gpioId == HW_GPIO_ID_A) 
    {
        state = GpioDataRegs.GPADAT.all;
    }
    else 
    {
        state = GpioDataRegs.GPBDAT.all;
    }

    EDIS;

    return state;
}



/***********************************  END  ************************************/
