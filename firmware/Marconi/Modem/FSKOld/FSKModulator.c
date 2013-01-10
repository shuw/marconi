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


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************


#include <string.h>
#include "All.h"
#include "ModulationParameters.h"


//******************************************************************************
//  L O C A L    D E F I N I T I O N S
//******************************************************************************


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


void FSKModulatorInit(BYTE *blockInitStruct, BYTE *blockObjStruct)
{
    FSK_MODULATOR_STATE_TYPE *state = (FSK_MODULATOR_STATE_TYPE *)blockObjStruct;

    //printf("Initializing FSKModulator\n");

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
        memcpy(outBuffer, FSK_TONE_SET[symbol], FSK_SYMBOL_LENGTH * sizeof(WORD));

        outBuffer += FSK_SYMBOL_LENGTH;
        outputAvail -= FSK_SYMBOL_LENGTH;
    }

    *inSize -= inputAvail;
    *outSize -= outputAvail;

    //printf("Firing FSKModulator\n");
    //printf("    inSize = %d, outSize = %d\n", *inSize, *outSize);

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
