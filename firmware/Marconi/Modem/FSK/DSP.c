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


#include "All.h"
#include "DSP.h"


// Direct-form I biquad section with one output
#if defined(PLATFORM_HW)
    #pragma CODE_SECTION(IirBiquad, RAM_CODE_SECTION)
#endif
SWORD IirBiquad(IIR_BIQUAD_TYPE *s, SWORD in)
{
    SDWORD acc;
    SWORD out;

    // Overflow is fine as long as final result is within numerical range
    acc = 
        (SDWORD)s->b1 * (SDWORD)in + 
        (SDWORD)s->b2 * (SDWORD)s->dx1 + 
        (SDWORD)s->b3 * (SDWORD)s->dx2 -
        (SDWORD)s->a2 * (SDWORD)s->dy1 -
        (SDWORD)s->a3 * (SDWORD)s->dy2;
    out = Extract(acc, s->shift);

    s->dx2 = s->dx1;
    s->dx1 = in;
    s->dy2 = s->dy1;
    s->dy1 = out;

    return out;
}


// Integrate and dump filter with one output
#if defined(PLATFORM_HW)
    #pragma CODE_SECTION(IntegrateAndDump, RAM_CODE_SECTION)
#endif
SWORD IntegrateAndDump(const SWORD *in, WORD length, WORD shift, SWORD factor)
{
    WORD i;
    SWORD acc = 0;

    for(i = 0; i < length; ++i) {
        acc += (SWORD)(((SDWORD)in[i] * (SDWORD)factor) >> shift);
    }

    return acc;
}


// Binary search, fixed point square root
#if defined(PLATFORM_HW)
    #pragma CODE_SECTION(Sqrt, RAM_CODE_SECTION)
#endif
WORD Sqrt(DWORD x)
{
    WORD i;
    WORD y;
    WORD delta;
    DWORD y2;

    // Only search for square roots less than sqrt(2^32 - 1) 
    y = 1U << 15;
    delta = y >> 1;

    if(x == 0) {
        y = 0;
    }
    else {
        for(i = 0; i < 15; ++i) {
            // We can't overflow since y cannot grow bigger than 2^16 - 1
            y2 = (DWORD)y * (DWORD)y;
            if(y2 < x) {
                y += delta;
                delta >>= 1;
            }
            else if(y2 > x) {
                y -= delta;
                delta >>= 1;
            }
        }
    }

    return y;
}


// Determines the total number of significant bits used to represent the given value
#if defined(PLATFORM_HW)
    #pragma CODE_SECTION(SignificantBits, RAM_CODE_SECTION)
#endif
WORD SignificantBits(SWORD x)
{
    WORD i;

    x = x < 0 ? -x : x;
    for(i = 0; i < 15; ++i) {
        if(x == 0) {
            break;
        }
        x >>= 1;
    }

    return i;
}


// Magnitude of complex value
WORD Mag(SWORD i, SWORD q)
{
    return Sqrt((SDWORD)i * (SDWORD)i + (SDWORD)q * (SDWORD)q); 
}


// Absolute value of vector
void Abs(const SWORD *x, SWORD *y, WORD length)
{
    WORD i;
    
    for(i = 0; i < length; ++i) {
        y[i] = x[i] < 0 ? -x[i] : x[i];
    }
}


// Find maximum value of vector of numbers
#if defined(PLATFORM_HW)
    #pragma CODE_SECTION(Max, RAM_CODE_SECTION)
#endif
SWORD Max(const SWORD *x, WORD length, WORD *index)
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
