//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Performs 4-FSK modulation on a stream of WORD-packed dibits (bit pairs).
//
//******************************************************************************


#include <string.h>
#include "All.h"
#include "FSKCommon.h"


void FSKModulatorInit(BYTE *blockInitStruct, BYTE *blockObjStruct)
{
    FSK_MODULATOR_STATE_TYPE *state = (FSK_MODULATOR_STATE_TYPE *)blockObjStruct;

    state->savedBits = 0;
    state->bitBuffer = 0;
}


WORD FSKModulator(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *blockObjStruct)
{
    FSK_MODULATOR_STATE_TYPE *state = (FSK_MODULATOR_STATE_TYPE *)blockObjStruct;
    WORD outputAvail = *outSize;
    WORD inputAvail = *inSize;
    WORD symbol;


    while((inputAvail > 0 || state->savedBits > 0) && outputAvail >= FSK_SYMBOL_LENGTH) {
        if(state->savedBits == 0) {
            // Enqueue a WORD
            state->bitBuffer = *inBuffer++;
            state->savedBits = BITS_IN_WORD;
            inputAvail--;
        }

        // Dequeue a bit pair
        symbol = FSK_DIBITS_TO_SYMBOL[state->bitBuffer & FSK_BITS_PER_SYMBOL_MASK];
        state->bitBuffer >>= FSK_BITS_PER_SYMBOL;
        state->savedBits -= FSK_BITS_PER_SYMBOL;
        
        // Output modulated symbol
        memcpy(outBuffer, FSK_VCO_I_SET[symbol], FSK_SYMBOL_LENGTH * sizeof(WORD));

        outBuffer += FSK_SYMBOL_LENGTH;
        outputAvail -= FSK_SYMBOL_LENGTH;
    }

    *inSize -= inputAvail;
    *outSize -= outputAvail;

    return 0;
}


WORD FSKModulatorReady(WORD *inSize, WORD *outSize, BYTE *blockObjStruct)
{
    FSK_MODULATOR_STATE_TYPE *state = (FSK_MODULATOR_STATE_TYPE *)blockObjStruct;

    if((*inSize >= 1 || state->savedBits > 0) && *outSize >= FSK_SYMBOL_LENGTH) {
        return TRUE;
    }
    else {
        return FALSE;
    }
}
