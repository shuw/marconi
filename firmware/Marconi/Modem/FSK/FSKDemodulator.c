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


#include <string.h>
#include "All.h"
#include "DSP.h"
#include "FSKCommon.h"


#define SAMPLES_REQUIRED 60
#define SYMBOL_LENGTH_FACTOR 819
#define AMPLITUDE_SET_LEVEL 11585
#define GAIN_MIN 64
#define GAIN_MAX 25600

#define NUM_SOFT_BITS           4
#define SOFT_BIT_MASK           0x000F

#define THIS_SYMBOL             0
#define NEXT_SYMBOL             1

#define PLL_GAIN_FACTOR 683


const SWORD LEVEL_FILT[] = {
    63,     63,      0,   -130,      0,      8,      0,      0,      0,      0, 
};
const SWORD PHASE_FILT[] = {
     1,      2,      1,   -467,    214,      8,      0,      0,      0,      0, 
};
const SWORD SOFT_BIT_FILT[] = {
     3,      5,      3,   -433,    188,      8,      0,      0,      0,      0, 
};


// Temp buffers 
static SWORD bufferI[SAMPLES_REQUIRED];
static SWORD bufferQ[SAMPLES_REQUIRED];
static SWORD softBits[FSK_SAMPLES_PER_SYMBOL][FSK_BITS_PER_SYMBOL];
static SWORD softBitsMag[FSK_BITS_PER_SYMBOL];
static SWORD phase[FSK_SAMPLES_PER_SYMBOL];
static SWORD softBitAvg[FSK_SAMPLES_PER_SYMBOL];


//#include "TargetFile.h"
//FILE *fp;


void FSKDemodulatorInit(BYTE *blockInitStruct, BYTE *blockObjStruct)
{
    FSK_DEMODULATOR_STATE_TYPE *state = (FSK_DEMODULATOR_STATE_TYPE*)blockObjStruct;

    memcpy(&state->levelFilter, LEVEL_FILT, sizeof(state->levelFilter));
    memcpy(&state->phaseFilter[0], PHASE_FILT, sizeof(state->phaseFilter[0]));
    memcpy(&state->phaseFilter[1], PHASE_FILT, sizeof(state->phaseFilter[1]));
    memcpy(&state->softBitFilter[0], SOFT_BIT_FILT, sizeof(state->softBitFilter[0]));
    memcpy(&state->softBitFilter[1], SOFT_BIT_FILT, sizeof(state->softBitFilter[1]));

    state->vcoOffset = 0;
    state->bitBuffer = 0;
    state->bitCount = 0;
    state->phaseAccum = 0;
    memset(state->symbolHist, 0, sizeof(state->symbolHist));

    //fp = fopen("test_points.dat", "wb");
}


#if defined(PLATFORM_HW)
    #pragma CODE_SECTION(FSKDemodulator, RAM_CODE_SECTION)
#endif
WORD FSKDemodulator(WORD *inBuffer, WORD *inSize, WORD *outBuffer, WORD *outSize, BYTE *blockObjStruct)
{
    FSK_DEMODULATOR_STATE_TYPE *state = (FSK_DEMODULATOR_STATE_TYPE*)blockObjStruct;
    SWORD *input = (SWORD*)inBuffer;
    WORD i;
    WORD tone;
    WORD vcoOffset;
    SWORD rmsLevel;
    SWORD gain;
    WORD maxTone;
    SWORD *s;
    SWORD samplingUpdate;
    WORD pick;
    WORD sigBits;

    // Measure RMS level of rectified signal and update gain
    Abs(input, bufferI, FSK_SYMBOL_LENGTH);
    rmsLevel = IntegrateAndDump(bufferI, FSK_SYMBOL_LENGTH, SHIFT_S1_15, SYMBOL_LENGTH_FACTOR);
    rmsLevel = IirBiquad(&state->levelFilter, rmsLevel);
    rmsLevel = Extract(rmsLevel, SHIFT_S8_8);
    if(rmsLevel == 0) {
        gain = GAIN_MAX;
    }
    else {
        gain = (SWORD)(((SDWORD)AMPLITUDE_SET_LEVEL) / rmsLevel);
        gain = MAX(gain, GAIN_MIN);
        gain = MIN(gain, GAIN_MAX);
    }

    // Testpoint: gain
    //fwrite(&gain, 1, sizeof(gain), fp);
    //TargetFileWrite(&gain, 1, sizeof(gain), fp);

    for(tone = 0; tone < FSK_NUM_TONES; ++tone) {
        // Complex-baseband demodulation (bufferI => in phase, bufferQ => quadrature phase)
        vcoOffset = state->vcoOffset;
        for(i = 0; i < SAMPLES_REQUIRED; ++i) {
            bufferI[i] = Mult(input[i], FSK_VCO_I_SET[tone][vcoOffset], SHIFT_S1_15);
            bufferQ[i] = Mult(input[i], FSK_VCO_Q_SET[tone][vcoOffset], SHIFT_S1_15);
            if(++vcoOffset == FSK_SYMBOL_LENGTH) {
                vcoOffset = 0;
            }
        }

        // Matched filter I and Q at baseband
        bufferI[0] = IntegrateAndDump(
            bufferI, FSK_SYMBOL_LENGTH, SHIFT_S1_15, SYMBOL_LENGTH_FACTOR);
        bufferI[1] = IntegrateAndDump(
            bufferI + (FSK_SYMBOL_LENGTH / 2), FSK_SYMBOL_LENGTH, SHIFT_S1_15, SYMBOL_LENGTH_FACTOR);
        bufferQ[0] = IntegrateAndDump(
            bufferQ, FSK_SYMBOL_LENGTH, SHIFT_S1_15, SYMBOL_LENGTH_FACTOR);
        bufferQ[1] = IntegrateAndDump(
            bufferQ + (FSK_SYMBOL_LENGTH / 2), FSK_SYMBOL_LENGTH, SHIFT_S1_15, SYMBOL_LENGTH_FACTOR);
        
        // Magnitude of complex envelope
        state->symbolHist[2][tone] = Mag(bufferI[0], bufferQ[0]);
        state->symbolHist[3][tone] = Mag(bufferI[1], bufferQ[1]);
    }

    // Testpoint: symbol history
    //fwrite(state->symbolHist, 1, sizeof(state->symbolHist), fp); 
    //TargetFileWrite(state->symbolHist, 1, sizeof(state->symbolHist), fp); 

    for(i = 0; i < FSK_SAMPLES_PER_SYMBOL; ++i) {
        SDWORD phase32;
        // Calculate symbol phase
        Max((SWORD *)state->symbolHist[1 + i], FSK_NUM_TONES, &maxTone);
        phase32 = (SDWORD)state->symbolHist[2 + i][maxTone] - (SDWORD)state->symbolHist[i][maxTone];
        phase32 = Mult32(phase32, gain, SHIFT_S8_8);
        phase32 = Mult32(phase32, PLL_GAIN_FACTOR, SHIFT_S8_8);
        phase[i] = Extract(phase32, SHIFT_S1_15 - SHIFT_S2_14);
        phase[i] = IirBiquad(&state->phaseFilter[i], phase[i]);

        // Generate soft bits
        s = (SWORD *)state->symbolHist[1 + i];
        softBits[i][0] = s[TONE_2] + s[TONE_3] - s[TONE_1] - s[TONE_4];
        softBits[i][1] = s[TONE_3] + s[TONE_4] - s[TONE_1] - s[TONE_2];
        softBits[i][0] = Mult(softBits[i][0], gain, SHIFT_S8_8);
        softBits[i][1] = Mult(softBits[i][1], gain, SHIFT_S8_8);

        // Generate filtered average softbits
        Abs(softBits[i], softBitsMag, FSK_BITS_PER_SYMBOL);
        softBitAvg[i] = (softBitsMag[0] + softBitsMag[1]) >> 1;
        softBitAvg[i] = IirBiquad(&state->softBitFilter[i], softBitAvg[i]);
    }

    // Testpoint: symbol phase
    //fwrite(phase, 1, sizeof(phase), fp);
    //TargetFileWrite(phase, 1, sizeof(phase), fp);
    // Testpoint: filtered soft bits
    //fwrite(softBitAvg, 1, sizeof(softBitAvg), fp);
    //TargetFileWrite(softBitAvg, 1, sizeof(softBitAvg), fp);

    // Pick best symbol phase for phase recovery and soft bit output
    Max(softBitAvg, FSK_SAMPLES_PER_SYMBOL, &pick);
    state->phaseAccum += phase[pick];
    samplingUpdate = Extract(state->phaseAccum, SHIFT_S2_14);
    state->phaseAccum -= samplingUpdate << SHIFT_S2_14;
    samplingUpdate += FSK_SYMBOL_LENGTH;

    // Produce softbits in the range [-7, 7]
    sigBits = SignificantBits(softBits[pick][0]) >> 1;
    softBits[pick][0] = softBits[pick][0] < 0 ? -sigBits & SOFT_BIT_MASK : sigBits & SOFT_BIT_MASK;
    sigBits = SignificantBits(softBits[pick][1]) >> 1;
    softBits[pick][1] = softBits[pick][1] < 0 ? -sigBits & SOFT_BIT_MASK : sigBits & SOFT_BIT_MASK;

    // Update state
    memcpy(state->symbolHist[0], (WORD *)state->symbolHist[0] + FSK_SAMPLES_PER_SYMBOL * FSK_NUM_TONES,
        sizeof(state->symbolHist[0][0]) * FSK_SAMPLES_PER_SYMBOL * FSK_NUM_TONES);
    state->vcoOffset = (state->vcoOffset + samplingUpdate) % FSK_SYMBOL_LENGTH;
    state->bitBuffer |= ((softBits[pick][1] << NUM_SOFT_BITS) + softBits[pick][0]) << state->bitCount;
    state->bitCount += 2 * NUM_SOFT_BITS;

    // Testpoint: sampling update
    //fwrite(&samplingUpdate, 1, sizeof(samplingUpdate), fp);
    //TargetFileWrite(&samplingUpdate, 1, sizeof(samplingUpdate), fp);

    if(state->bitCount == BITS_IN_WORD) {
        // Output one word of soft bits (4 soft bits)
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
