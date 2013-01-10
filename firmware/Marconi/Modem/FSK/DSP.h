//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Interface to DSP routines. 
//
//******************************************************************************

#ifndef __DSP_H__
#define __DSP_H__

#include "GlobalDefs.h"

typedef struct {
    SWORD b1;       // bn's and an's are same convention as Matlab filter(...)
    SWORD b2;
    SWORD b3;
    SWORD a2;
    SWORD a3;
    WORD shift;     // Indicates fractional bits used for all coefficients
    SWORD dx1;      // Delays stored with same precision as samples
    SWORD dx2;
    SWORD dy1;
    SWORD dy2;
} IIR_BIQUAD_TYPE;


SWORD IirBiquad(IIR_BIQUAD_TYPE *s, SWORD in);
SWORD IntegrateAndDump(const SWORD *in, WORD length, WORD shift, SWORD factor);
WORD Sqrt(DWORD x);
WORD SignificantBits(SWORD x);
WORD Mag(SWORD i, SWORD q);
SWORD Max(const SWORD *x, WORD length, WORD *index);
void Abs(const SWORD *x, SWORD *y, WORD length);


// Extract with shift and round towards zero
static SWORD Extract(SDWORD x, WORD shift) {
    SWORD sign = x < 0 ? -1 : 1;

    return (SWORD)((x * sign) >> shift) * sign;
}


// Extract with shift and round towards zero
static SDWORD Extract32(SDWORD x, WORD shift) {
    SWORD sign = x < 0 ? -1 : 1;

    return ((x * sign) >> shift) * sign;
}


// Fixed-point multiply: cleaner code and safe for TI compiler
// TODO: should we inline this?
static SWORD Mult(SWORD x, SWORD y, WORD shift) {
    return Extract((SDWORD)x * (SDWORD)y, shift);
}


// Fixed-point multiply: cleaner code and safe for TI compiler
// TODO: should we inline this?
static SDWORD Mult32(SDWORD x, SDWORD y, WORD shift) {
    return Extract32(x * y, shift);
}

#endif
