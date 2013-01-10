//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// DSP routines for use with modem.
//
//******************************************************************************


#define __DSP_C__


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************
#include "All.h"

//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************

//******************************************************************************
//  E X T E R N A L     F U N C T I O N    P R O T O T Y P E S
//******************************************************************************

DWORD ConvASM(const SWORD *x, const SWORD *h, WORD length);

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


// Equivalent to z = x .* y in Matlab
#if defined(PLATFORM_HW)
    #pragma CODE_SECTION(DSPMult, RAM_CODE_SECTION)
#endif
void DSPMult(const SWORD *x, const SWORD *y, SWORD *z, WORD length, WORD shift)
{
    WORD i;

    for(i = 0; i < length; ++i) {
        z[i] = (SWORD)((SDWORD)x[i] * (SDWORD)y[i] >> shift);
    }
}


// Discrete time convolution
#if defined(PLATFORM_HW)
    #pragma CODE_SECTION(DSPConv, RAM_CODE_SECTION)
#endif
void DSPConv(const SWORD *x, const SWORD *h, WORD impulseLength, SWORD *z, WORD outputCount, WORD shift)
{
#ifndef PLATFORM_HW
    SDWORD accum;
    WORD j;
#endif
    WORD i;
    const SWORD *hPtr = h + impulseLength - 1;
   
    for(i = 0; i < outputCount; ++i) {
#if defined PLATFORM_HW
        z[i] = (SWORD)(ConvASM(x + i, hPtr, impulseLength) >> (shift - 6));
#else
        accum = 0; 
        for(j = 0; j < impulseLength; ++j) {
            accum += (SDWORD)x[i + j] * (SDWORD)h[impulseLength - j - 1] >> shift;
        }

        // No saturation!
        z[i] = (SWORD)accum;
#endif
    }
}


// Magnitude squared of complex number
#if defined(PLATFORM_HW)
    #pragma CODE_SECTION(DSPMagSq, RAM_CODE_SECTION)
#endif
void DSPMagSq(const SWORD *xi, const SWORD *xq, SWORD *z, WORD length, WORD shift)
{
    WORD i;

    for(i = 0; i < length; ++i) {
        z[i] = (SWORD)(((SDWORD)xi[i] * (SDWORD)xi[i] + (SDWORD)xq[i] * (SDWORD)xq[i]) >> shift);
    }
}


// Find maximum value of vector of numbers
#if defined(PLATFORM_HW)
    #pragma CODE_SECTION(DSPMax, RAM_CODE_SECTION)
#endif
SWORD DSPMax(const SWORD *x, WORD length, WORD *index)
{
    SWORD maxValue;
    WORD maxIndex;
    WORD i;

    maxIndex = 0;
    maxValue = x[0];
    for(i = 1; i < length; ++i) {
        if(x[i] > maxValue) {
            maxValue = x[i];
            maxIndex = i;
        }
    }

    if(index != 0) {
        *index = maxIndex;
    }

    return maxValue;
}


/***********************************  END  ************************************/
