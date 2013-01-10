//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Performs 4-FSK demodulation on a stream of samples to produce soft bits.
//
//******************************************************************************


//******************************************************************************
//  I N C L U D E    F I L E S
//******************************************************************************


#include <string.h>
#include "All.h"
#include "DSP.h"
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

#define THIS_INSTANT            (FSK_SYMBOL_LENGTH / 2)
#define NEXT_INSTANT            (FSK_SYMBOL_LENGTH + THIS_INSTANT - 1)
#define SAMPLES_REQUIRED        (FSK_RECEIVER_FILTER_LENGTH - 1 + FSK_SYMBOL_LENGTH + THIS_INSTANT)
#define SAMPLES_PROCESSED       (FSK_SYMBOL_LENGTH + THIS_INSTANT)
#define NUM_SOFT_BITS           4
#define SOFT_BIT_MASK           0x000F

#define THIS_SYMBOL             0
#define NEXT_SYMBOL             1

#define TONE_1                  0
#define TONE_2                  1
#define TONE_3                  2
#define TONE_4                  3


// Temp buffers 
static SWORD buffer1[SAMPLES_REQUIRED];
static SWORD buffer2[SAMPLES_REQUIRED];
static SWORD symbolMax[FSK_NUM_TONES];
static SWORD samples[2][FSK_NUM_TONES];


void FSKDemodulatorInit(BYTE *blockInitStruct, BYTE *blockObjStruct)
{
    FSK_DEMODULATOR_STATE_TYPE *state = (FSK_DEMODULATOR_STATE_TYPE*)blockObjStruct;

    state->vcoOffset = 0;
    state->bitBuffer = 0;
    state->bitCount = 0;
    state->prevDecision = 0;
}

#if defined(PLATFORM_HW)
    #pragma CODE_SECTION(FSKDemodulator, RAM_CODE_SECTION)
#endif
WORD FSKDemodulator(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *blockObjStruct)
{
    FSK_DEMODULATOR_STATE_TYPE *state = (FSK_DEMODULATOR_STATE_TYPE*)blockObjStruct;
    WORD i;
    SWORD *s;
    WORD tone;
    WORD vcoOffset;
    WORD samplingUpdate;
    WORD thisDecision;
    WORD nextDecision;
    SDWORD sum;
    SDWORD softBit0;
    SDWORD softBit1;
    SWORD *input = (SWORD*)inBuffer;

    // TODO: we must still address the saturation issues to avoid overflow

    for(tone = 0; tone < FSK_NUM_TONES; ++tone) {
        // Complex-baseband demodulation (buffer1 => I, buffer2 => Q)
        vcoOffset = state->vcoOffset;
        for(i = 0; i < SAMPLES_REQUIRED; ++i) {
            buffer1[i] = (SWORD)((SDWORD)input[i] * 
                (SDWORD)FSK_VCO_I_SET[tone][vcoOffset] >> FSK_FIXED_POINT);
            buffer2[i] = (SWORD)((SDWORD)input[i] * 
                (SDWORD)FSK_VCO_Q_SET[tone][vcoOffset] >> FSK_FIXED_POINT);
            if(++vcoOffset == FSK_SYMBOL_LENGTH) {
                vcoOffset = 0;
            }
        }

        // Filter and measure squared energy
        DSPConv(buffer1, FSK_RECEIVER_FILTER, FSK_RECEIVER_FILTER_LENGTH, buffer1, 
            SAMPLES_PROCESSED, FSK_FIXED_POINT);
        DSPConv(buffer2, FSK_RECEIVER_FILTER, FSK_RECEIVER_FILTER_LENGTH, buffer2, 
            SAMPLES_PROCESSED, FSK_FIXED_POINT);
        DSPMagSq(buffer1, buffer2, buffer1, SAMPLES_PROCESSED, FSK_FIXED_POINT);

        // Symbol max and two sampling instants
        DSPMax(buffer1, FSK_SYMBOL_LENGTH, (WORD *)&symbolMax[tone]);
        samples[THIS_SYMBOL][tone] = buffer1[THIS_INSTANT];
        samples[NEXT_SYMBOL][tone] = buffer1[NEXT_INSTANT];
    }

    // Hard decision decoding
    DSPMax(samples[THIS_SYMBOL], FSK_NUM_TONES, &thisDecision);
    DSPMax(samples[NEXT_SYMBOL], FSK_NUM_TONES, &nextDecision);

    // Soft-bit generation. Adding 1 to the sum serves two purposes:
    // division by zero is avoided and the effective range of the
    // soft bits is symmetric over [-15, 15] instead of [-15, 16].
    s = samples[THIS_SYMBOL];
    sum = s[TONE_1] + s[TONE_2] + s[TONE_3] + s[TONE_4] + 1;
    softBit0 = s[TONE_2] + s[TONE_3] - s[TONE_1] - s[TONE_4];
    softBit0 = (softBit0 << (NUM_SOFT_BITS - 1)) / sum;
    softBit0 &= SOFT_BIT_MASK;
    softBit1 = s[TONE_4] + s[TONE_3] - s[TONE_1] - s[TONE_2];
    softBit1 = (softBit1 << (NUM_SOFT_BITS - 1)) / sum;
    softBit1 &= SOFT_BIT_MASK;

    if(thisDecision != nextDecision && thisDecision != state->prevDecision) {
        SWORD delta;
        delta = symbolMax[thisDecision] - THIS_INSTANT;
        delta = MIN(MAX(delta, -3), 3);
        samplingUpdate = FSK_SYMBOL_LENGTH + delta;
    }
    else {
        samplingUpdate = FSK_SYMBOL_LENGTH;
    }

    state->prevDecision = thisDecision;
    state->vcoOffset = (state->vcoOffset + samplingUpdate) % FSK_SYMBOL_LENGTH;
    state->bitBuffer |= ((softBit1 << NUM_SOFT_BITS) + softBit0) << state->bitCount;
    state->bitCount += 2 * NUM_SOFT_BITS;

    if(state->bitCount == BITS_IN_WORD) {
        *outBuffer = state->bitBuffer;
        *outSize = 1;
        state->bitBuffer = 0;
        state->bitCount = 0;
    }
    else {
        *outSize = 0;
    }

    *inSize = samplingUpdate;

    return 0;
}


WORD FSKDemodulatorReady(WORD *inSize, WORD *outSize, BYTE *blockObjStruct)
{
    if(*inSize >= SAMPLES_REQUIRED && *outSize >= 1) {
        return TRUE;
    }

    return FALSE;
}


/***********************************  END  ************************************/
