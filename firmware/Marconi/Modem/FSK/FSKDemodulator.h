//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Interface for FSK demodulator.
//
//******************************************************************************


#ifndef __FSK_DEMODULATOR_H__
#define __FSK_DEMODULATOR_H__

#include "DSP.h"
#include "FSKCommon.h"

typedef struct {
    WORD                vcoOffset;
    WORD                symbolHist[2 * FSK_SAMPLES_PER_SYMBOL][FSK_NUM_TONES];
    IIR_BIQUAD_TYPE     levelFilter;
    IIR_BIQUAD_TYPE     phaseFilter[FSK_SAMPLES_PER_SYMBOL];
    IIR_BIQUAD_TYPE     softBitFilter[FSK_SAMPLES_PER_SYMBOL];
    SWORD               phaseAccum;
    WORD                bitBuffer;
    WORD                bitCount;
} FSK_DEMODULATOR_STATE_TYPE;


void FSKDemodulatorInit(BYTE *blockInitStruct, BYTE *blockObjStruct);
WORD FSKDemodulator(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *blockObjStruct);
WORD FSKDemodulatorReady(WORD *inSize, WORD *outSize, BYTE *blockObjStruct);


#endif
