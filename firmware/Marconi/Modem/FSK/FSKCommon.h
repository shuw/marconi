//******************************************************************************
//
//              Copyright (C) 2005.  The Marconi Project
//      All rights reserved.  No part of this program may be reproduced.
//
//     http://www.sgsolutions.ca/marconi
//
//==============================================================================
//
// Shared constants for FSK modulator and demodulator
//
//******************************************************************************

#ifndef __FSKCOMMON_H__
#define __FSKCOMMON_H__


#define FSK_SYMBOL_LENGTH           40
#define FSK_NUM_TONES               4
#define FSK_BITS_PER_SYMBOL         2
#define FSK_BITS_PER_SYMBOL_MASK    0x03
#define FSK_SAMPLES_PER_SYMBOL      2

#define SHIFT_S1_15                 15
#define SHIFT_S2_14                 14
#define SHIFT_S3_13                 13
#define SHIFT_S8_8                  8

#define TONE_1                      0
#define TONE_2                      1
#define TONE_3                      2
#define TONE_4                      3

extern const WORD FSK_DIBITS_TO_SYMBOL[];

// Oscillator look-up table
extern const SWORD FSK_VCO_I_SET[FSK_NUM_TONES][FSK_SYMBOL_LENGTH];
extern const SWORD FSK_VCO_Q_SET[FSK_NUM_TONES][FSK_SYMBOL_LENGTH];

#endif 
